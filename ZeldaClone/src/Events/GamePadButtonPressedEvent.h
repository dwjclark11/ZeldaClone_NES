#pragma once
#include "../ECS/ECS.h"
#include "EventManager.h"
#include <SDL.h>

class GamePadButtonPressedEvent : public Event
{
public:
	
	//SDL_GameControllerButton button;

	GamePadButtonPressedEvent() {}
};