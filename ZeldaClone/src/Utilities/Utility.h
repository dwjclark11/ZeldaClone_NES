#pragma once
#include <string>
#include <glm/glm.hpp>

void ConvertNumberParser(std::string group, int num, int power);

void ChangePlayerAttrib(const glm::vec2& velocity, const glm::vec2& shieldOffset, 
	const glm::vec2& swordOffset, const int& shieldWidth, 
	const int& shieldHeight, const int& spriteWidth,  const int& swordWidth, const int& swordHeight);
