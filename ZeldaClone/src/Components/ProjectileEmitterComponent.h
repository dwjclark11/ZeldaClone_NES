#pragma once
#include <glm/glm.hpp>
#include <SDL.h>
#include "../Utilities/Timer.h"

struct ProjectileEmitterComponent
{
	// Component Variables
	glm::vec2 projectileVelocity;
	int repeatFrequency;
	int projectileDuration;
	int hitPercentDamage;
	int lastEmissionTime;
	bool isFriendly;
	bool shootUp;
	bool shootRight;
	bool shootDown;
	bool shootLeft;
	bool shotTriggered;
	bool shotFired;
	Timer timer;

	ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), int repeatFrequency = 0,
		int projectileDuration = 10000, int hitPercentDamage = 1, bool isFriendly = false, bool shootUp = false,
		bool shootDown = false, bool shootLeft = false, bool shootRight = false, bool shotTriggered = false, bool shotFired = false)
	{
		this->projectileVelocity 	= projectileVelocity;
		this->repeatFrequency 		= repeatFrequency;
		this->projectileDuration 	= projectileDuration;
		this->hitPercentDamage 		= hitPercentDamage;
		this->lastEmissionTime 		= SDL_GetTicks();
		this->isFriendly 			= isFriendly;
		this->shootUp				= shootUp;
		this->shootRight			= shootRight;
		this->shootDown				= shootDown;
		this->shootLeft				= shootLeft;
		this->shotTriggered			= shotTriggered;
		this->shotFired				= shotFired;
	}
};