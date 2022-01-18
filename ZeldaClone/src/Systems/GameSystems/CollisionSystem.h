#pragma once
#include "../../ECS/ECS.h"

class EventManager;

class CollisionSystem : public System
{
public:

	CollisionSystem();

	void Update(std::unique_ptr<EventManager>& eventManager);
	
private:

	bool CheckAABBCollision(double aX, double aY, double aWidth, double aHeight,
		double bX, double bY, double bWidth, double bHeight);

};
