#pragma once
#include <glm/glm.hpp>

class RigidBodyComponent
{
public:
	// Component Variables
	glm::vec2 velocity;
	bool up;
	bool down;
	bool left;
	bool right;
	
	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0f, 0.0f), bool up = false, bool down = false, bool left = false, bool right = false)
	{
		this->velocity 	= velocity;
		this->up 		= up;
		this->down 		= down;
		this->left 		= left;
		this->right 	= right;
	}
};
