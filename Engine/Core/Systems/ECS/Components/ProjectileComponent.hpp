#pragma once

#include <sdl3/SDL.h>


struct ProjectileComponent
{
    bool isFriendly;

    int hitePercentDamage;

    int duration;

    int startTime;

    ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 0, int duration = 0)
    {

        this->isFriendly = isFriendly;
        this->hitePercentDamage = hitePercentDamage;
        this->duration = duration;
        this->startTime = SDL_GetTicks();

    }

};