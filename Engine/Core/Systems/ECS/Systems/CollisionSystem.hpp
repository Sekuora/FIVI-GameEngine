#pragma once

#include <ECS.hpp>

#include <Log.hpp>

#include <BoxColliderComponent.hpp>
#include <TransformComponent.hpp>

#include <GlobalPointerAlias.hpp>

#include <CollisionEvent.hpp>

#include <SDL3/SDL.h>

using std::to_string;

class CollisionSystem: public System
{
    public: CollisionSystem()
    {
        SetRequiredComponent<BoxColliderComponent>();
        SetRequiredComponent<TransformComponent>();
    }

    void Update(Unique<EventBridge>& eventBridge)
    {
        auto entities = GetEntities();

        // Initial entities to check
        for
        (
            auto entity = entities.begin(); 
            entity != entities.end(); 
            entity++ 
        )
        {
            Entity a = *entity;

            auto aTransform = a.GetComponent<TransformComponent>();
            auto aCollider = a.GetComponent<BoxColliderComponent>();
            
            // Other entities to check
            for 
            (
                auto other = entity;
                other != entities.end();
                other++
            )
            {
                Entity b = *other;

                if (a == b) {continue; }

                auto bTransform = b.GetComponent<TransformComponent>();
                auto bCollider = b.GetComponent<BoxColliderComponent>();

                bool collisionWithOther = CheckAABBCollision
                (
                    aTransform.position.x + aCollider.offset.x,
                    aTransform.position.y + aCollider.offset.y,
                    aCollider.width,
                    aCollider.height,
                    bTransform.position.x + bCollider.offset.x,
                    bTransform.position.y + bCollider.offset.y,
                    bCollider.width,
                    bCollider.height
                );

                if (collisionWithOther)
                {
                    // Log::Custom<false>(to_string(a.GetId()) + " : Entity is Colliding with Other: " + to_string(b.GetId()), Color_White, "Collision System");

                    eventBridge->Broadcast<CollisionEvent>(a, b);

                }
            }
        }
    }

    bool CheckAABBCollision
    (
        double entityPosX, double entityPosY, double entityWidth, double entityHeight,
        double otherPosX, double otherPosY, double otherWidth, double otherHeight
    )
    {
        return 
        (
            entityPosX < otherPosX + otherWidth &&
            entityPosX + entityWidth > otherPosX &&
            entityPosY < otherPosY + otherHeight &&
            entityPosY + entityHeight > otherPosY
        );
    }
};