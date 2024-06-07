#pragma once
#include <glm/glm.hpp>

class TransformComponent
{
  public:
	glm::vec2 position{ 0.f }, scale{ 1.f };
	double rotation{ 0.0 };
	bool bCollision{ false };
};