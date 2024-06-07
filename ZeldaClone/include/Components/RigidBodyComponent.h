#pragma once
#include <glm/glm.hpp>

enum class RigidBodyDir
{
	UP = 0,
	RIGHT,
	DOWN,
	LEFT,
	NO_DIR
};

struct RigidBodyComponent
{
	glm::vec2 velocity{ 0.f };
	RigidBodyDir dir{ RigidBodyDir::NO_DIR };
};
