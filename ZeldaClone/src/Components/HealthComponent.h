#pragma once
#include "../Utilities/Timer.h"

class HealthComponent
{
public:
	int healthPercentage, maxHearts;
	bool isHurt, addHeart;

	Timer hurtTimer, lowHeathTimer, deathTimer;

	HealthComponent(int healthPercentage = 6, int maxHearts = 3)
		: healthPercentage{ healthPercentage }, maxHearts{ maxHearts }
		, isHurt{ false }, addHeart{ false }
		, hurtTimer{}, lowHeathTimer{}, deathTimer{}
	{
		
	}
};