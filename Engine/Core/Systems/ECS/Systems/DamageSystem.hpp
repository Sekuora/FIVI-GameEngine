#pragma once

#include <ECS.hpp>

#include <BoxColliderComponent.hpp>
#include <Log.hpp>



#include <EventBridge.hpp>
#include <CollisionEvent.hpp>


using std::to_string;

class DamageSystem : public System
{
    public:
        DamageSystem()
        {
             
            SetRequiredComponent<BoxColliderComponent>();

        }

        void SubscribeToEvents(Unique<EventBridge>& eventBridge)
        {
            eventBridge->Subscribe<CollisionEvent>(this, &DamageSystem::OnCollision);
            
        
        }

        void OnCollision(CollisionEvent& event)
        {
            // Log::Custom<false>("On Collision Event: " + to_string(event.a.GetId()) + " and " + to_string(event.b.GetId()), Color_White, "Damage System");

            // event.a.Remove();
            // event.b.Remove();

        }

        void Update()
        {

        }

};