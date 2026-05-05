#pragma once

#include <ECS.hpp>

#include <SpriteComponent.hpp>
#include <AnimationComponent.hpp>

#include <SDL3/SDL.h>

class AnimationSystem: public System
{
    public: AnimationSystem()
    {
        SetRequiredComponent<SpriteComponent>();
        SetRequiredComponent<AnimationComponent>();
    }

    void Update()
    {
        for(auto entity: GetEntities())
        {
            AnimationComponent& animation = entity.GetComponent<AnimationComponent>();
            SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();


            animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;

            // Push source rect to the next frame
            sprite.srcRect.x = animation.currentFrame * sprite.width;

        }
    }
};