#pragma once

#include <glm/glm.hpp>

using glm::vec2;

struct KeyboardControllerComponent
{
    
        vec2 upVelocity;
        vec2 downVelocity;    

        vec2 rightVelocity;
        vec2 leftVelocity;
        

        KeyboardControllerComponent(vec2 upVelocity = vec2(0), vec2 downVelocity = vec2(0), vec2 leftVelocity = vec2(0), vec2 rightVelocity = vec2(0))
        {
            this->upVelocity = upVelocity;
            this->downVelocity = downVelocity;

            this->leftVelocity = leftVelocity;
            this->rightVelocity = rightVelocity;
            

        }


};