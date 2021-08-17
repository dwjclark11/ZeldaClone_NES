#pragma once

struct HealthComponent
{
	int healthPercentage;
	int maxHearts;

	HealthComponent(int healthPercentage = 9, int maxHearts = 3)
	{
		this->healthPercentage = healthPercentage;
		this->maxHearts = maxHearts;
	}
};