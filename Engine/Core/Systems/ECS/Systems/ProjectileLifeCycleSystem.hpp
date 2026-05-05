#pragma once

#include <ECS.hpp>

#include <Log.hpp>

#include <ProjectileComponent.hpp>

using std::to_string;

class ProjectileLifeCycleSystem : public System
{
    public: ProjectileLifeCycleSystem()
    {
        SetRequiredComponent<ProjectileComponent>();
    }

    void Update()
    {
        for (Entity entity: GetEntities())
        {
            ProjectileComponent projectile = entity.GetComponent<ProjectileComponent>();

            if (SDL_GetTicks() - projectile.startTime > projectile.duration)
            {
                Log::Custom<false>("Projectile: " + to_string(entity.GetId()) + " removed", Color_Blue, "Projectile LCS");
                
                entity.Remove();
                
            }

        }
    }


};