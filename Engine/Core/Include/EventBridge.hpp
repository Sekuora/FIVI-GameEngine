#pragma once

#include <Log.hpp>

#include <map>

#include <typeindex>

#include <GlobalPointerAlias.hpp>

#include <GlobalCastAlias.hpp>

#include <Event.hpp>

#include <list>

#include <SDL3/SDL.h>

using std::map;

using std::type_index;

using std::list;

using std::invoke;

using std::move;

using std::forward;

class PrimalEventCallback
{
private:
    /** @brief Pass Event Function Reference as parameter for callback execution  */
    virtual void Call(Event &event) = 0;

public:
    virtual ~PrimalEventCallback() = default;

    void Execute(Event &event)
    {
        Call(event);
    }
};

/** @brief The event to callback  */
template <typename TOwner, typename TEvent>
class EventCallback : public PrimalEventCallback
{
    private:
    //  The Callback Function passed through owner namespace and Event to Execute on Callback
    typedef void (TOwner::*CallbackFunction)(TEvent &);

    TOwner *ownerInstance;
    CallbackFunction callbackFunction;

    virtual void Call(Event &event) override
    {
        invoke(callbackFunction, ownerInstance, scast<TEvent &>(event));
    }
public:
    EventCallback(TOwner *ownerInstance, CallbackFunction callbackFunction)
    {
        this->ownerInstance = ownerInstance;
        this->callbackFunction = callbackFunction;
    }

    virtual ~EventCallback() override = default;


};

// Define the list of callback pointers
typedef list<Unique<PrimalEventCallback>> Actions;

/** @brief The events manager  */
class EventBridge
{

public:
    EventBridge()
    {
        Log::Custom<false>("Constructor Called", Color_White, "Event Bridge");
    }

    ~EventBridge()
    {
        Log::Custom<false>("Destructor Called", Color_White, "Event Bridge");
    }

    void Reset()
    {
        subscribers.clear();
    }

    /** @brief Subscribe to Generic Type Event */
    template <typename TEvent, typename TOwner>
    void Subscribe(TOwner *ownerInstance, void (TOwner::*callbackFunction)(TEvent &))
    {
        if (!subscribers[typeid(TEvent)].get())
        {
            subscribers[typeid(TEvent)] = newU<Actions>();
        }

        auto subscriber = newU<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);

        subscribers[typeid(TEvent)]->push_back(move(subscriber));
    };


    /** @brief Broadcast Generic Type Event with variadic Args */
    template <typename TEvent, typename... TArgs>
    void Broadcast(TArgs &&...args)
    {
        auto actions = subscribers[typeid(TEvent)].get();
        if (actions)
        {
            for (auto actionIt = actions->begin(); actionIt != actions->end(); actionIt++)
            {
                auto selectedAction = actionIt->get();

                TEvent event(forward<TArgs>(args)...);
                selectedAction->Execute(event);
            }
        }
    };

private:
    map<type_index, Unique<Actions>> subscribers;
};