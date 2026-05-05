#pragma once

#include <ECS.hpp>
#include <GlobalPointerAlias.hpp>

#include <ProjectileEmitterComponent.hpp>
#include <TransformComponent.hpp>
#include <RigidBodyComponent.hpp>
#include <SpriteComponent.hpp>
#include <BoxColliderComponent.hpp>
#include <ProjectileComponent.hpp>
#include <GlobalSDLKeysAlias.hpp>

#include <EventBridge.hpp>
#include <KeyPressedEvent.hpp>
#include <KeyBoardControllerComponent.hpp>

#include <ShootProjectileEvent.hpp>

#include <glm/glm.hpp>

using glm::vec2;

class ProjectileEmitSystem : public System
{

public:
    bool isShooting;

    ECSBridge &ecsBridge;

    ProjectileEmitSystem(Unique<ECSBridge> &ecsBridge) : ecsBridge(*ecsBridge)
    {
        SetRequiredComponent<ProjectileEmitterComponent>();
        SetRequiredComponent<TransformComponent>();

        isShooting = false;
    }

    void SubscribeToEvents(Unique<EventBridge> &eventBridge)
    {
        eventBridge->Subscribe<KeyPressedEvent>(this, &ProjectileEmitSystem::OnShoot);
    }

    void OnShoot(KeyPressedEvent &event)
    {
        if (event.symbol == key::space)
        {
            Log::Custom<false>("Is Shooting", Color_Magenta, "Keyboard Control System");
            for (Entity entity : GetEntities())
            {
                if (entity.HasComponent<CameraFollowComponent>())
                {
                    ProjectileEmitterComponent &projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                    const TransformComponent transform = entity.GetComponent<TransformComponent>();
                    const RigidBodyComponent rb = entity.GetComponent<RigidBodyComponent>();

                    vec2 projectilePosition = transform.position;

                    if (entity.HasComponent<SpriteComponent>())
                    {
                        const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width / 2);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }

                    vec2 projectileVelocity = projectileEmitter.projectileVelocity;

                    int directionX = 0;
                    int directionY = 0;

                    if (rb.velocity.x > 0)
                        directionX = +1;

                    if (rb.velocity.x < 0)
                        directionX = -1;

                    if (rb.velocity.y > 0)
                        directionY = +1;

                    if (rb.velocity.y < 0)
                        directionY = -1;

                    projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
                    projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

                    Entity projectile = ecsBridge.CreateEntity();
                    projectile.AddComponent<TransformComponent>(projectilePosition, vec2(1.0, 1.0));
                    projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                    projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                    projectile.AddComponent<BoxColliderComponent>(4, 4);
                    projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
                }
            }
        }

        Log::Custom<false>("Is Shooting" + isShooting, Color_Magenta, "Projectile Emit System");
    }

    void Update()
    {

        for (Entity entity : GetEntities())
        {
            ProjectileEmitterComponent &projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
            const TransformComponent transform = entity.GetComponent<TransformComponent>();

            if (projectileEmitter.spawnFrequency == 0)
            {
                continue;
            }

            if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.spawnFrequency)
            {
                vec2 projectilePosition = transform.position;

                if (entity.HasComponent<SpriteComponent>())
                {
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width / 2);
                    projectilePosition.y += (transform.scale.y * sprite.height / 2);
                }

                Entity projectile = ecsBridge.CreateEntity();
                projectile.AddComponent<TransformComponent>(projectilePosition, vec2(1.0, 1.0));
                projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                projectile.AddComponent<BoxColliderComponent>(4, 4);
                projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

                // Update last emitted projectile to current milliseconds
                projectileEmitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};