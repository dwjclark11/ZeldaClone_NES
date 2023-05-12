#pragma once
#include <SDL.h>
#include "../Utilities/Timer.h"

class ProjectileComponent
{
public:
	// Component Variables
	bool isFriendly;
	int hitPercentDamage;
	int duration;
	int startTime;
	enum class Dir{ UP = 0, RIGHT, DOWN, LEFT, NO_DIR };
	Dir dir;

	Timer boomTimer;

	ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 1, int duration = 0, Dir dir = Dir::NO_DIR)
		: isFriendly{isFriendly}, hitPercentDamage{hitPercentDamage}, duration{duration}
		, startTime{static_cast<int>(SDL_GetTicks())}, dir{dir}
	{

	}
};