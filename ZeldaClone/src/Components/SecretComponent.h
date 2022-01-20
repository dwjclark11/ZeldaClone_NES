#pragma once
#include <string>

struct SecretComponent
{
	std::string locationID;
	std::string newTrigger;
	std::string newSpriteAssetID;
	int spriteWidth;
	int spriteHeight;
	int spriteSrcX;
	int spriteSrcY;
	
	bool found;

	SecretComponent(std::string locationID = "none", std::string newTrigger = "none", std::string newSpriteAssetID = "none", int spriteWidth = 0, int spriteHeight = 0, int spriteSrcX = 0, int spriteSrcY = 0)
	{
		this->locationID = locationID;
		this->newTrigger = newTrigger;
		this->newSpriteAssetID = newSpriteAssetID;
		this->spriteWidth = spriteWidth;
		this->spriteHeight = spriteHeight;
		this->spriteSrcX = spriteSrcX;
		this->spriteSrcY = spriteSrcY;
		found = false;
	}
};