#pragma once
#include <SDL.h>
#include <string>
#include <glm/glm.hpp>

class SpriteComponent
{
public:
	// Component Variables
	std::string assetID;
	int width;
	int height;
	int layer; // ZIndex;
	bool isFixed; // Non Moving object;
	SDL_RendererFlip flip;
	SDL_Rect srcRect;
	glm::vec2 offset;

	SpriteComponent(std::string assetID = "", int width = 0, int height = 0, int layer = 0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0, glm::vec2 offset = glm::vec2(0,0))
	{
		this->assetID 		= assetID;
		this->width 		= width;
		this->height 		= height;
		this->layer 		= layer;
		this->isFixed 		= isFixed;
		this->flip 			= SDL_FLIP_NONE;
		this->srcRect 		= { srcRectX, srcRectY, width, height };
		this->offset 		= offset;
	}
};

