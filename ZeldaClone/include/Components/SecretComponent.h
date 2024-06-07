#pragma once
#include <string>
#include <glm/glm.hpp>

struct SecretComponent
{
	std::string locationID{ "none" }, newTrigger{ "none" }, newSpriteAssetID{ "none" };
	int spriteWidth{ 0 }, spriteHeight{ 0 }, spriteSrcX{ 0 }, spriteSrcY{ 0 };
	glm::vec2 startPos{ 0.f };
	bool bFound{ false }, bMoveUp{ false }, bMoveDown{ false };
};