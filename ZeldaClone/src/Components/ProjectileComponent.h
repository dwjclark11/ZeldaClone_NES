#pragma once
#include <SDL.h>
#include "../Utilities/Timer.h"

enum class ProjectileDir { UP = 0, RIGHT, DOWN, LEFT, NO_DIR };

struct ProjectileComponent
{
	bool isFriendly{ false };
	int hitPercentDamage{ 1 }, duration{ 0 }, startTime{ static_cast<int>(SDL_GetTicks()) };
	ProjectileDir dir{ ProjectileDir::NO_DIR };
	Timer boomTimer{};
};