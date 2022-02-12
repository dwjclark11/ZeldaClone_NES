#pragma once
#include "../Utilities/Timer.h"

class HealthComponent
{
public:
	int healthPercentage;
	int maxHearts;
	bool isHurt;
	bool addHeart;

	Timer hurtTimer;
	Timer lowHeathTimer;
	Timer deathTimer;
	
	HealthComponent(int healthPercentage = 6, int maxHearts = 3, bool addHeart = false, bool isHurt = false)
	{
		this->healthPercentage = healthPercentage;
		this->maxHearts = maxHearts;
		this->addHeart = addHeart;
		this->isHurt = isHurt;
	}
};