#pragma once
#include <glm/glm.hpp>

struct BoxColliderComponent
{
	int width{ 0 }, height{ 0 };
	glm::vec2 offset{ 0.f };
	bool colliding{ false };
};