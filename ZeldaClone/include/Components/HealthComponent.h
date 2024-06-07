#pragma once
#include "Utilities/Timer.h"

struct HealthComponent
{
	int healthPercentage{ 6 }, maxHearts{ 3 };
	bool bIsHurt{ false }, bAddHeart{ false };
	Timer hurtTimer{}, lowHeathTimer{}, deathTimer{};
};