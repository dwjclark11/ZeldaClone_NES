#pragma once
#include "ECS/ECS.h"
#include "Events/EventManager.h"
#include <SDL.h>

struct KeyPressedEvent : public Event
{
	SDL_Keycode symbol{};
};
