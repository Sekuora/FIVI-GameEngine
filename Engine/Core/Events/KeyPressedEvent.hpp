#pragma once

#include <SDL3/SDL.h>
#include <Event.hpp>


class KeyPressedEvent : public Event
{

public:

    SDL_Keycode symbol;

    KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};