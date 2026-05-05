#pragma once

#include <GlobalCastAlias.hpp>

#include <string>
#include <SDL3/SDL.h>

using std::string;

struct SpriteComponent
{
    SpriteComponent() = default;

    string assetId;
    int width;
    int height;
    uint16_t zIndex;

    bool isFixed;

    SDL_FRect srcRect;

    SpriteComponent(string assetId, int width = 0, int height = 0, uint16_t zIndex = 0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0 )
    {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->isFixed = isFixed;
        this->srcRect = {scast<float>(srcRectX), scast<float>(srcRectY), scast<float>(width), scast<float>(height)};
    }
       
       
};
