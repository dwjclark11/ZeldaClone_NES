#pragma once
#include <glm/glm.hpp>

class RigidBodyComponent
{
public:
	// Component Variables
	glm::vec2 velocity;
	enum class Dir { UP = 0, RIGHT, DOWN, LEFT, NO_DIR };
	Dir dir;

	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0f, 0.0f), Dir dir = Dir::NO_DIR)
		: velocity{velocity}, dir{dir}
	{

	}
};
