#pragma once

#include <Event.hpp>

#include <Log.hpp>

using std::to_string;

class ShootProjectileEvent : public Event
{
public:
    bool isShooting;

    ShootProjectileEvent(bool isShooting) : isShooting(isShooting) {}


};