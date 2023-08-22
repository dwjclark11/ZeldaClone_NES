#pragma once
#include "../../ECS/ECS.h"

class EventManager;

class CollisionSystem : public System
{
private:
	class Game& m_Game;
	EventManager& m_EventManager;

	bool CheckAABBCollision(double aX, double aY, double aWidth, double aHeight,
		double bX, double bY, double bWidth, double bHeight);
public:
	CollisionSystem(EventManager& eventManager);
	void Update();
};
