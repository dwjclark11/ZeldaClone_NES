#pragma once
#include "../../ECS/ECS.h"
#include <glm/glm.hpp>

class EventManager;

class CollisionSystem : public System
{
private:
	class Game& m_Game;
	EventManager& m_EventManager;

private:
	bool CheckAABBCollision(double aX, double aY, double aWidth, double aHeight,
		double bX, double bY, double bWidth, double bHeight);

public:
	CollisionSystem(EventManager& eventManager);
	~CollisionSystem() = default;

	void Update();
};
