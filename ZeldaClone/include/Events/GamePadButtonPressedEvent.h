#pragma once
#include "ECS/ECS.h"
#include "EventManager.h"
#include <SDL.h>

struct GamePadButtonPressedEvent : public Event
{
	Uint8 button{0};
};
