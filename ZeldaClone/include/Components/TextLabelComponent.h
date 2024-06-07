#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL.h>

struct TextLabelComponent
{
	glm::vec2 position{ 0.f };
	std::string text{ "" }, assetID{ "" };
	SDL_Color color{ 255, 255, 255, 255 };
	bool bIsFixed{ true };
};