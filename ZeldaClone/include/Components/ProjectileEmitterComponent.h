#pragma once
#include "Utilities/Timer.h"
#include <glm/glm.hpp>
#include <SDL.h>

struct ProjectileEmitterComponent
{
	glm::vec2 projectileVelocity{ 200.f };
	int repeatFrequency{ 0 }, projectileDuration{ 10000 };
	int hitPercentDamage{ 1 }, lastEmissionTime{ static_cast<int>( SDL_GetTicks() ) };
	bool bIsFriendly{ false }, bShotTriggered{ false }, bShotFired{ false };
	Timer timer{};
};
