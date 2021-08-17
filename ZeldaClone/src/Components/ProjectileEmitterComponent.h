#pragma once
#include <glm/glm.hpp>
#include <SDL.h>


struct ProjectileEmitterComponent
{
	glm::vec2 projectileVelocity;
	int repeatFrequency;
	int projectileDuration;
	int hitPercentDamage;
	int lastEmissionTime;
	bool isFriendly;

	ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), int repeatFrequency = 0, int projectileDuration = 10000, int hitPercentDamage = 10, bool isFriendly = false)
	{
		this->projectileVelocity = projectileVelocity;
		this->repeatFrequency = repeatFrequency;
		this->projectileDuration = projectileDuration;
		this->hitPercentDamage = hitPercentDamage;
		this->lastEmissionTime = SDL_GetTicks();
		this->isFriendly = isFriendly;
	}
};