#include "ECS.hpp"
#include <Log.hpp>
#include <iostream>

using std::find;
using std::remove_if;

int PrimalComponent::nextId = 0;

void System::AddEntity(Entity entity)
{
    // Add entity at vector's end
    entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{

    // Lambda condition check : Entity == Other
    auto entityEqualsOther = [&entity](Entity other)
    { return entity == other; };

    // Check all matching entities through their id's operator override
    vector<Entity>::iterator removeEntity = remove_if(
        entities.begin(),
        entities.end(),
        entityEqualsOther);

    entities.erase(removeEntity, entities.end());
}

void ECSBridge::Update()
{
    // Add queued entities
    for (auto entity : entitiesToAdd)
    {
        AddEntityToSystems(entity);
    }

    // Clear queue
    entitiesToAdd.clear();

    for (auto entity : entitiesToRemove)
    {
        RemoveEntityFromSystems(entity);

        entityComponentSignatures[entity.GetId()].reset();

        freeIDs.push_back(entity.GetId());
    }

    entitiesToRemove.clear();
}

void ECSBridge::QueueRemoveEntity(Entity entity)
{
    entitiesToRemove.insert(entity);
}

void ECSBridge::AddEntityToSystems(Entity entity)
{

    const auto entityId = entity.GetId();

    // Get entity component signature at the requested entityId position
    const auto &entityComponentSignature = entityComponentSignatures[entityId];

    // Loop all the systems
    for (auto &system : systems)
    {
        // Assign signature of the system value
        const auto &systemComponentSignature = system.second->GetComponentSignature();

        // Bitset comparison with &
        // When comparison matches systemComponentSignature is a signature match
        bool isSignatureMatch = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (isSignatureMatch)
        {
            // Add entity to system
            system.second->AddEntity(entity);
        }
    }
}

void ECSBridge::RemoveEntityFromSystems(Entity entity)
{
    for (auto system : systems)
    {
        system.second->RemoveEntity(entity);
    }
}

Entity ECSBridge::CreateEntity()
{
    int entityId;

    if (freeIDs.empty())
    {
        entityId = numEntities++;

        if (entityId >= entityComponentSignatures.size())
        {
            entityComponentSignatures.resize(entityId + 1);
        }
    }
    else
    {
        entityId = freeIDs.front();
        freeIDs.pop_front();
    }

    Entity entity(entityId);

    // Assign this bridge to entity
    entity.bridge = this;

    entitiesToAdd.insert(entity);

    // Log::Default("Entity Created with id = " + std::to_string(entityId));
    return entity;
}

void Entity::Remove()
{

    bridge->QueueRemoveEntity(*this);
}
