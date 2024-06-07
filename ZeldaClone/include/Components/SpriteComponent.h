#pragma once
#include <SDL.h>
#include <string>
#include <glm/glm.hpp>

struct SpriteComponent
{
	std::string assetID{ "" };
	int width{ 0 }, height{ 0 }, layer{ 0 };
	bool bIsFixed{ false };
	SDL_Rect srcRect{ 0, 0, 16, 16 };
	glm::vec2 offset{ 0.f };
	SDL_RendererFlip flip{ SDL_FLIP_NONE };
};
