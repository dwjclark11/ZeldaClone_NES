#pragma once
#include <glm/glm.hpp>

struct KeyboardControlComponent
{
	glm::vec2 upVelocity{ 0.f }, rightVelocity{ 0.f };
	glm::vec2 downVelocity{ 0.f }, leftVelocity{ 0.f };
};
