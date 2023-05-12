#pragma once
#include <string>
#include <glm/glm.hpp>

class SecretComponent
{
public:
	std::string locationID, newTrigger, newSpriteAssetID;
	int spriteWidth, spriteHeight, spriteSrcX, spriteSrcY;
	
	glm::vec2 startPos;
	bool found;
	// This is for the rock push and Raft Secrets/triggers
	bool moveDown;
	bool moveUp;

	SecretComponent(std::string locationID = "none", std::string newTrigger = "none", std::string newSpriteAssetID = "none", int spriteWidth = 0, int spriteHeight = 0, int spriteSrcX = 0, int spriteSrcY = 0, glm::vec2 startPos = glm::vec2(0), bool moveUp = false, bool moveDown = false)
		: locationID{locationID}, newTrigger{newTrigger}, newSpriteAssetID{newSpriteAssetID}
		, spriteWidth{spriteWidth}, spriteHeight{spriteHeight}, spriteSrcX{spriteSrcX}, spriteSrcY{spriteSrcY}
		, startPos{startPos}, found{false}, moveUp{moveUp}, moveDown{moveDown}
	{
		
	}
};