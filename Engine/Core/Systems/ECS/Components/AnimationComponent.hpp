#pragma once

#include <stdint.h>

#include <SDL3/SDL.h>

struct AnimationComponent
{
    uint16_t numFrames;
    uint16_t currentFrame;
    uint16_t startTime;

    uint16_t frameSpeedRate;
    
    bool isLoop;

    // Constructor
    AnimationComponent(
        uint16_t numFrames = 1,
        
        uint16_t farmeSpeedRate = 1,

        bool isLoop = true
    )
    {
        this->numFrames = numFrames;
       
        this->frameSpeedRate = farmeSpeedRate;

        this->isLoop = isLoop;

        // No Parameter

        this->currentFrame = 1;

        this->startTime = SDL_GetTicks();

    }
};
