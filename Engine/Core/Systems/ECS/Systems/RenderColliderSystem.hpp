#pragma once


#include <ECS.hpp>

#include <SDL3/SDL.h>

#include <GlobalCastAlias.hpp>

#include <TransformComponent.hpp>

#include <BoxColliderComponent.hpp>

class RenderColliderSystem: public System
{

    public:
        RenderColliderSystem()
        {

            SetRequiredComponent<TransformComponent>();
            SetRequiredComponent<BoxColliderComponent>();
        }

        void Update(SDL_Renderer* renderer, SDL_FRect &camera)
        {

            for (auto entity: GetEntities())
            {
                const auto transform = entity.GetComponent<TransformComponent>();

                const auto collider = entity.GetComponent<BoxColliderComponent>();

                SDL_FRect colliderRect =
                {
                    scast<float>(transform.position.x + collider.offset.x - camera.x),
                    scast<float>(transform.position.y + collider.offset.y - camera.y),
                    scast<float>(collider.width * transform.scale.x),
                    scast<float>(collider.height * transform.scale.y)
                };

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderRect(renderer, &colliderRect);
                
            }

        }

};