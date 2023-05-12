#pragma once
#include <glm/glm.hpp>
#include <SDL.h>
#include "../Utilities/Timer.h"

class ProjectileEmitterComponent
{
public:
	// Component Variables
	glm::vec2 projectileVelocity;
	int repeatFrequency;
	int projectileDuration;
	int hitPercentDamage;
	int lastEmissionTime;
	bool isFriendly;
	bool shotTriggered;
	bool shotFired;

	Timer timer;


	ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(200), int repeatFrequency = 0,
		int projectileDuration = 10000, int hitPercentDamage = 1, bool isFriendly = false)
		: projectileVelocity{projectileVelocity}, repeatFrequency{repeatFrequency}
		, projectileDuration{projectileDuration}, hitPercentDamage{ hitPercentDamage }
		, lastEmissionTime{static_cast<int>(SDL_GetTicks())}, isFriendly{isFriendly}
		, shotTriggered{false}, shotFired{false}
	{
		
	}
};