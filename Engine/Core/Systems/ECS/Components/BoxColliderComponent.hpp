#pragma once

#include <glm/glm.hpp>

#include <SDL3/SDL.h>

using glm::vec2;

struct BoxColliderComponent
{
    uint16_t width;
    uint16_t height;
    vec2 offset;

    // Constructor
    BoxColliderComponent
    (
        uint16_t width = 0.0,
        uint16_t height = 0.0,
        vec2 offset = vec2(0)
    )
    {
        this->width = width;
        this->height = height;
        this->offset = offset;
    }
};
