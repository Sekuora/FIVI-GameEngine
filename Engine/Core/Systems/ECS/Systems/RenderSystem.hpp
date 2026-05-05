#pragma once

#include <GlobalPointerAlias.hpp>
#include <GlobalCastAlias.hpp>

#include <ECS.hpp>
#include <AssetBridge.hpp>

#include <TransformComponent.hpp>
#include <SpriteComponent.hpp>


#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include<algorithm>

#include <vector>

using std::vector;
using std::sort;


class RenderSystem: public System
{
    public:
    RenderSystem()
    {
        SetRequiredComponent<TransformComponent>();
        SetRequiredComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer, Unique<AssetBridge>& assetBridge, SDL_FRect& camera)
    {

        // Z Index Sorting
        struct EntityToRender
        {
            TransformComponent transformComponent;
            SpriteComponent spriteComponent;
        };

        vector<EntityToRender> entitiesToRender;

        for (auto entity: GetEntities())
        {
            // Get Components
            EntityToRender entityToRender;

            // Shorthand to components
            entityToRender.spriteComponent = entity.GetComponent<SpriteComponent>();
            entityToRender.transformComponent = entity.GetComponent<TransformComponent>();
            
            entitiesToRender.emplace_back(entityToRender);
        }

        sort(
            // Range
            entitiesToRender.begin(), entitiesToRender.end(),
            
            // Lambda Sorting Condition
            [](const EntityToRender& a, const EntityToRender& b)
            {
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
            }
        );


        // Get z index sorted entities
        for (auto entity: entitiesToRender)
        {
            const TransformComponent transform= entity.transformComponent;
            const SpriteComponent sprite = entity.spriteComponent;

            // Source Rect
            SDL_FRect srcRect = sprite.srcRect;

            // Destination Rect
            SDL_FRect dstRect =
            {
                scast<float>(transform.position.x - (sprite.isFixed ? 0.0f : camera.x)),
                scast<float>(transform.position.y - (sprite.isFixed ? 0.0f : camera.y)), 
                scast<float>(sprite.width * transform.scale.x),
                scast<float>(sprite.height * transform.scale.y)
            };

            // Render Texture
            SDL_RenderTextureRotated(renderer, assetBridge->GetTexture(sprite.assetId), &srcRect, &dstRect, transform.rotation, NULL, SDL_FLIP_NONE);
            
        }
    }
};