#pragma once

#include <glm/glm.hpp>

using glm::vec2;

struct RigidBodyComponent
{

    vec2 velocity;

    RigidBodyComponent(vec2 velocity = vec2(0.0, 0.0))
    {
        this->velocity = velocity;
    };
};
 