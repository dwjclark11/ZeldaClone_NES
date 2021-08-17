#pragma once
#include <glm/glm.hpp>
struct BackgroundImageComponent
{
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;

	BackgroundImageComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0f)
	{
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
	}
};