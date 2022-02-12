#pragma once
#include <glm/glm.hpp>

class BoxColliderComponent
{
public:
	int width;
	int height;
	glm::vec2 offset;
	bool colliding;
	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0), bool colliding = false)
	{
		this->width = width;
		this->height = height;
		this->offset = offset;
		this->colliding = colliding;
	}
};