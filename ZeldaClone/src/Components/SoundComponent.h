#pragma once
#include <string>

class SoundComponent
{
public:
	std::string attackSound, hitSound, deathSound, movementSound;

	SoundComponent(std::string attackSound = "", std::string hitSound = "", std::string deathSound = "", std::string movementSound = "")
		: attackSound{attackSound}, hitSound{hitSound}, deathSound{deathSound}, movementSound{movementSound}
	{ }
};
