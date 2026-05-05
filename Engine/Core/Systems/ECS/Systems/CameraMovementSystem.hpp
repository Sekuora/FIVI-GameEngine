#pragma once

#include <CameraFollowComponent.hpp>
#include <TransformComponent.hpp>
#include <Log.hpp>
#include <ECS.hpp>
#include <SDL3/SDL.h>

using std::to_string;

class CameraMovementSystem : public System
{
    public:
    CameraMovementSystem()
    {
        SetRequiredComponent<CameraFollowComponent>();

        SetRequiredComponent<TransformComponent>();
    }

        void Update(SDL_FRect& camera)
        {
            for(auto entity: GetEntities())
            {
                TransformComponent transform = entity.GetComponent<TransformComponent>();
            
                
                if (transform.position.x + (camera.w / 2) < Game::mapWidth)
                {
                    camera.x = transform.position.x - (Game::windowWidth / 2);
                    
                }
                if (transform.position.y + (camera.h / 2) < Game::mapHeight)
                {
                    camera.y = transform.position.y - (Game::windowHeight / 2);
                }

                camera.x = camera.x < 0 ? 0 : camera.x;

                camera.y = camera.y < 0 ? 0 : camera.y;

                camera.x = camera.x > camera.w ? camera.w : camera.x;
                
                camera.y = camera.y > camera.h ? camera.h : camera.y;
                

                // Log::Custom<false>("Camera position: " + to_string(camera.x) + ", " + to_string(camera.y), Color_Blue, "Camera Movement System");
                

            }
        }


};