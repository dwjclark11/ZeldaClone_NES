#pragma once
#include <SDL.h>
#include "../Utilities/Timer.h"

struct ProjectileComponent
{
	// Component Variables
	bool isFriendly;
	int hitPercentDamage;
	int duration;
	int startTime;
	Timer boomTimer;

	ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 0, int duration = 0)
	{
		this->isFriendly 			= isFriendly;
		this->hitPercentDamage 		= hitPercentDamage;
		this->duration 				= duration;
		this->startTime 			= SDL_GetTicks();
	}
};