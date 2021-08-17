#pragma once
#include <string>

struct SoundComponent
{
	std::string attackSound;
	std::string hitSound;
	std::string deathSound;
	std::string movementSound;

	SoundComponent(std::string attackSound = "", std::string hitSound = "", std::string deathSound = "", std::string movementSound = "")
	{
		this->attackSound = attackSound;
		this->hitSound = hitSound;
		this->deathSound = deathSound;
		this->movementSound = movementSound;
	}
};
