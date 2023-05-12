#pragma once
#include <glm/glm.hpp>

class TransformComponent
{
public:
	glm::vec2 position, scale;
	double rotation;
	bool collision;

	TransformComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0f)
		: position{position}, scale{scale}, rotation{rotation}, collision{false}
	{

	}
};