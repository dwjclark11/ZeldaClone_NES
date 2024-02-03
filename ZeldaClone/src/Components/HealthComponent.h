#pragma once
#include "../Utilities/Timer.h"

struct HealthComponent
{
	int healthPercentage{ 6 }, maxHearts{3};
	bool isHurt{false}, addHeart{false};
	Timer hurtTimer{}, lowHeathTimer{}, deathTimer{};
};