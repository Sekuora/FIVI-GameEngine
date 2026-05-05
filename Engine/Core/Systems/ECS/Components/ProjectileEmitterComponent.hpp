#pragma once


#include <glm/glm.hpp>

using glm::vec2;

struct ProjectileEmitterComponent
{
    vec2 projectileVelocity;
    
    uint64_t spawnFrequency;
    uint64_t projectileDuration;
    uint64_t hitPercentDamage;
    uint64_t lastEmissionTime;

    bool isFriendly;

    uint64_t secondsFactor = 1000;

    ProjectileEmitterComponent(vec2 projectileVelocity = vec2(0), uint64_t spawnFrequency = 0, uint64_t projectileDuration = 1000, uint64_t hitPercentDamage = 10, bool isFriendly = false)
    {
        this->projectileVelocity = projectileVelocity;
        this->spawnFrequency = spawnFrequency;
        this->projectileDuration = projectileDuration;
        this->hitPercentDamage = hitPercentDamage;

        this->isFriendly = isFriendly;
        
        this->lastEmissionTime = SDL_GetTicks();

    }




};