#pragma once
#include "../ECS/ECS.h"
#include "EventManager.h"
#include <SDL.h>

class KeyPressedEvent : public Event
{
public:
	SDL_Keycode symbol;

	KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};