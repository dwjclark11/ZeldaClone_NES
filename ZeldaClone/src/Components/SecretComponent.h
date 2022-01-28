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
	
	glm::vec2 startPos;
	bool found;
	// This is for the rock push and Raft Secrets/triggers
	bool moveDown;
	bool moveUp;
	SecretComponent(std::string locationID = "none", std::string newTrigger = "none", std::string newSpriteAssetID = "none", int spriteWidth = 0, int spriteHeight = 0, int spriteSrcX = 0, int spriteSrcY = 0, glm::vec2 startPos = glm::vec2(0))
	{
		this->locationID = locationID;
		this->newTrigger = newTrigger;
		this->newSpriteAssetID = newSpriteAssetID;
		this->spriteWidth = spriteWidth;
		this->spriteHeight = spriteHeight;
		this->spriteSrcX = spriteSrcX;
		this->spriteSrcY = spriteSrcY;
		this->startPos = startPos;
		found = false;
		moveDown = false;
		moveUp = false;
	}
};