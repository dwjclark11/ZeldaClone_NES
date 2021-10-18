#pragma once
#include "../Utilities/Timer.h"

struct HealthComponent
{
	int healthPercentage;
	int maxHearts;
	bool isHurt;
	Timer hurtTimer;
	Timer lowHeathTimer;
	
	
	HealthComponent(int healthPercentage = 9, int maxHearts = 3, bool isHurt = false)
	{
		this->healthPercentage = healthPercentage;
		this->maxHearts = maxHearts;
		this->isHurt = isHurt;
	}
};