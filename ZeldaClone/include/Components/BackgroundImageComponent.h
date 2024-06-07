#pragma once
#include <glm/glm.hpp>

struct BackgroundImageComponent
{
	glm::vec2 position{ 0.f };
	glm::vec2 scale{ 1.f };
	double rotation{ 0.f };
};