#pragma once

#include <ECSDefinitions.hpp>
#include <Log.hpp>

#include <GlobalPointerAlias.hpp>

#include <GlobalCastAlias.hpp>

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <set>

#include <deque>

class EventBridge;

using std::to_string;
using std::forward;
using std::make_pair;
using std::set;

using std::deque;

using std::type_index;
using std::unordered_map;
using std::vector;

using std::static_pointer_cast;


/**
 *  @brief Entity implementation for ECS
 *
 */
class Entity
{
private:
    /** @brief Entity ID */

    uint16_t id;

public:
    Entity(uint16_t id) : id(id) {};

    /** @brief Entity ID Getter */
    inline uint16_t GetId() const { return id; };

    /** @brief Add Component Template */
    template <typename T, typename ...TArgs> void AddComponent(TArgs&& ...args);

    /** @brief Remove Component Template */
    template <typename T> void RemoveComponent();

    /** @brief Has Component Template */
    template <typename T> bool HasComponent() const;

    /** @brief Get Component Template */
    template <typename T> T& GetComponent() const;

    /** @brief Remove */
    void Remove();


    // Pointer to bridge - forward declared
    class ECSBridge* bridge;


    // Operators Overload //
    auto operator<=>(const Entity &) const = default;
    Entity &operator=(const Entity &other) = default;
    bool operator==(const Entity &other) const { return id == other.id; }
    bool operator!=(const Entity &other) const { return id != other.id; }

  
};

struct PrimalComponent
{
protected:
    /**
     * @brief Auto Increment Component ID
     * @details Assign a different ID per component type
     */
    static int nextId;
};

/**
 *  @brief Component implementation for ECS
 */
template <typename T>
class Component : public PrimalComponent
{
public:
    /** @brief Component ID Getter */
    static inline uint16_t GetId()
    {
        static uint16_t id = nextId++;
        return id;
    };
};

/**
 *  @brief System implementation for ECS
 *  @details Process entities dependening on their component signature
 */
class System
{
private:
    Signature componentSignature;

    vector<Entity> entities;

public:
    System() = default;
    ~System() = default;

    /** @brief Add Entity to System */
    void AddEntity(Entity entity);

    /** @brief Remove Entity from System */
    void RemoveEntity(Entity entity);

    /** @brief Getter : returns entities pertaining to this system */
    inline vector<Entity> GetEntities() const { return entities; };

    /** @brief Signature getter */
    inline const Signature &GetComponentSignature() const { return componentSignature; };

    /** @brief Define a component type required by entities belonging to this system to be considered for processing */
    template <typename T>
    void SetRequiredComponent();

    // virtual void SubscribeToEvents(Unique<EventBridge>& eventBridge) = 0;
};

/**
 * @brief Base Pool class from which pool template inherits to generalize the type of any pool.
 * @details This pattern helps avoid the need to specify a pool type everrytime it is used.
 */
class PrimalPool
{
public:
    virtual ~PrimalPool() {}
};

/**
 * @brief Pool is a vector array of component properties data.
 * @details The pool wraps around standard vector functionalitty and builds
 * component data specific functionality
 */
template <typename T>
class Pool : public PrimalPool
{
private:
    vector<T> data;

public:
    Pool(uint16_t size = 100) { data.resize(size); }

    virtual ~Pool() = default;

    /** @brief Check if pool is empty */
    bool isEmpty() const { return data.empty(); }

    /** @brief Get pool data size */
    uint16_t GetSize() const { return data.size(); }

    /** @brief Resize pool to a new size */
    void Resize(uint16_t newSize) { data.resize(newSize); }

    /** @brief Empty out pool data */
    void Clear() { data.clear(); }

    /** @brief Add new generic object to pool data */
    void Add(T object) { data.push_back(object); }

    /** @brief Set object at specified pool data index */
    void Set(uint16_t index, T object) { data[index] = object; }

    /** @brief Get object at specified pool data index */
    T &Get(uint16_t index) { return static_cast<T &>(data[index]); }

    /** @brief Operator overload : [] returns data at specified index */
    T &operator[](uint16_t index) { return data[index]; }
};

/**
 * @brief The central manager class for Entities, Components, Systems
 * @details Bridge functionality of the ECS
 */
class ECSBridge
{

public:
    ECSBridge() { Log::Default("Registry Constructor Called!"); };
    ~ECSBridge() { Log::Default("Registry Destructor Called!"); }

    /** @brief Add Component Template */
    template <typename T, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&...args);

    /** @brief Remove Component Template */
    template <typename T>
    void RemoveComponent(Entity entity);

    /** @brief Has Component Template */
    template <typename T>
    bool HasComponent(Entity entity);

    /** @brief Get Component Template */
    template <typename T> T& GetComponent(Entity entity) const;

    /** @brief Add System Template */
    template <typename T, typename... TArgs>
    void AddSystem(TArgs &&...args);

    /** @brief Remove System Template */
    template <typename T>
    void RemoveSystem();

    /** @brief Has System Template */
    template <typename T>
    bool HasSystem() const;

    /** @brief Get System Template */
    template <typename T>
    T &GetSystem() const;

    void Update();


    /** @brief And entity to remove queue to process in update */
    void QueueRemoveEntity(Entity entity);



    /**
     * @brief Add entity to a system.
     *
     * Adds entity to a system that said entity requires
     * by matching entity signatures to system signatures.
     */
    void AddEntityToSystems(Entity entity);

    
    /** @brief Remove entity from system, entity won't be processed by its relevant systems */
    void RemoveEntityFromSystems(Entity entity);


    /** @brief Create an entity. */
    Entity CreateEntity();


    ECSBridge &operator=(const ECSBridge &other) = default;

private:
    uint16_t numEntities = 0;

    /** @brief Vector of Pool types. */
    vector<Shared<PrimalPool>> componentPools;

    /** @brief Vector of component signatures reflecting "on" components per entity */
    vector<Signature> entityComponentSignatures;

    /** @brief Key, value map - used to map systems to registry */
    unordered_map<type_index, Shared<System>> systems;

    /** @brief Set of entities marked to be added or removed from the next bridge update  */
    set<Entity> entitiesToAdd;
    set<Entity> entitiesToRemove;

    /** @brief Double ended queue, store entity ids previously removed, to reuse  */
    deque<int> freeIDs;


};

template <typename T, typename... TArgs>
inline void ECSBridge::AddComponent(Entity entity, TArgs &&...args)
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    // Check pool has correct size
    if (componentId >= componentPools.size())
    {
        componentPools.resize(componentId + 1, nullptr);
    }

    // When a pool doesn't exist for a component type
    if (!componentPools[componentId])
    {
        // Create new pool for that component type
        Shared<Pool<T>> newComponentPool = newS<Pool<T>>();
        componentPools[componentId] = newComponentPool;
    }

    // Get the pool of component values for that component type
    Shared<Pool<T>> componentPool = static_pointer_cast<Pool<T>>(componentPools[componentId]);

    // When entity ID is bigger than current component pool size.
    if (entityId >= componentPool->GetSize())
    {
        componentPool->Resize(numEntities);
    }

    // Send args to component constructor
    T newComponent(forward<TArgs>(args)...);

    // Add the new component to the component pool list. Entity used as index.
    componentPool->Set(entityId, newComponent);

    // Activate the entity.
    // Set the component signature of the entity and set the component ID.
    entityComponentSignatures[entityId].set(componentId);

    // Log::Custom<false>("Component id = " + to_string(componentId) + " added to entity id " + to_string(entityId), Color_Yellow, "ECS");
}

/// Bridge Template Definitions ///

template <typename T>
inline void ECSBridge::RemoveComponent(Entity entity)
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);

    Log::Custom<false>("Component id = " + to_string(componentId) + " was removed from entity id " + to_string(entityId), Color_Yellow, "ECS");
}

template <typename T>
inline bool ECSBridge::HasComponent(Entity entity)
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignatures.at(entityId).test(componentId);
}

template <typename T>
inline T &ECSBridge::GetComponent(Entity entity) const
{
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    auto componentPool = static_pointer_cast<Pool<T>>(componentPools[componentId]);

    return componentPool->Get(entityId);
}

template <typename T, typename... TArgs>
inline void ECSBridge::AddSystem(TArgs &&...args)
{
    Shared<T> newSystem = newS<T>(forward<TArgs>(args)...);

    // Add new key, value to systems unordered map
    systems.insert(make_pair(type_index(typeid(T)), newSystem));
}

template <typename T>
inline void ECSBridge::RemoveSystem()
{
    // Get pointer of the type id of the system
    auto system = systems.find(type_index(typeid(T)));
    systems.erase(system);
}

template <typename T>
inline bool ECSBridge::HasSystem() const
{
    return systems.find(type_index(typeid(T))) != systems.end();
}

template <typename T>
inline T &ECSBridge::GetSystem() const
{
    auto system = systems.find(type_index(typeid(T)));

    // Return safe type pointer to system
    return *(static_pointer_cast<T>(system->second));
}

/// System Template Definitions ///

template <typename T>
inline void System::SetRequiredComponent()
{
    const auto componentId = Component<T>::GetId();

    componentSignature.set(componentId);
}

/// Entity Template Definitions ///

template <typename T, typename... TArgs>
inline void Entity::AddComponent(TArgs &&...args)
{
    bridge->AddComponent<T>(*this, forward<TArgs>(args)...);
}

template <typename T>
inline void Entity::RemoveComponent()
{
    bridge->RemoveComponent<T>(*this);
}

template <typename T>
inline bool Entity::HasComponent() const
{
    return bridge->HasComponent<T>(*this);
}

template <typename T>
inline T &Entity::GetComponent() const
{
    return bridge->GetComponent<T>(*this);
}
