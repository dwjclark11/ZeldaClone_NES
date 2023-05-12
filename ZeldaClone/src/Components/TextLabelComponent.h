#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL.h>

class TextLabelComponent
{
public:
	glm::vec2 position;
	std::string text, assetID;
	SDL_Color color;
	bool isFixed;

	TextLabelComponent(glm::vec2 position = glm::vec2(0), const std::string& text = "", const std::string& assetID = "", const SDL_Color color = { 255, 255, 255, 255}, bool isFixed = true)
		: position{position}, text{text}, assetID{assetID}, color{color}, isFixed{isFixed}
	{
		
	}
};