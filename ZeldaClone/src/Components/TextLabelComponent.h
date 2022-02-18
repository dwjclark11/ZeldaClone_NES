#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL.h>

class TextLabelComponent
{
public:
	glm::vec2 position;
	std::string text; 
	std::string assetID;
	SDL_Color color;
	bool isFixed;

	TextLabelComponent(glm::vec2 position = glm::vec2(0), std::string text = "", std::string assetID = "", const SDL_Color color = { 255, 255, 255, 255}, bool isFixed = true)
	{
		this->position = position;
		this->text = text;
		this->assetID = assetID;
		this->color = color;
		this->isFixed = isFixed;
	}
};