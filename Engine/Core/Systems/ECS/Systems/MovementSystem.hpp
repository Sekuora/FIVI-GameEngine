#pragma once

#include <ECS.hpp>
#include <TransformComponent.hpp>
#include <RigidBodyComponent.hpp>

#include <Log.hpp>

class MovementSystem: public System
{
    public:
    MovementSystem()
    {
        SetRequiredComponent<TransformComponent>();
        SetRequiredComponent<RigidBodyComponent>();
    }

    void Update(double deltaTime)
    {
        for (auto entity: GetEntities())
        {
            auto& transform = entity.GetComponent<TransformComponent>();
            const auto rigidBody = entity.GetComponent<RigidBodyComponent>();

            transform.position.x += rigidBody.velocity.x * deltaTime;
            transform.position.y += rigidBody.velocity.y * deltaTime;
            
        }
    }

};