#pragma once
#include "../ECS/ECS.h"
#include "EventManager.h"
#include <SDL.h>

class GamePadButtonPressedEvent : public Event
{
public:
	
	Uint8 button;

	GamePadButtonPressedEvent(Uint8 button) : button(button) {}
};

