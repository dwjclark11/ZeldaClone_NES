#pragma once
#include <glm/glm.hpp>

class BoxColliderComponent
{
public:
	int width, height;
	glm::vec2 offset;
	bool colliding;
	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0))
		: width{width}, height{height}, offset{offset}, colliding{false}
	{
		
	}
};