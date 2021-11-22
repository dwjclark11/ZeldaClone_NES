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

	
	// Calculate the distance before the entities collide? --> This is not used anywhere
	//glm::vec2 CalculateAABBDistance(Entity a, Entity b)
	//{
	//	auto& aPos = a.GetComponent<TransformComponent>();
	//	const auto& aBox = a.GetComponent<BoxColliderComponent>();
	//	auto& bPos = b.GetComponent<TransformComponent>();
	//	const auto& bBox = b.GetComponent<BoxColliderComponent>();
	//	
	//	glm::vec2 delta = glm::vec2(0, 0);
	//	
	//	if (aPos.position.x < bPos.position.x)
	//	{
	//		delta.x = bPos.position.x - (aPos.position.x + aBox.width);
	//	}
	//	else if (aPos.position.x > bPos.position.x)
	//	{
	//		delta.x = aPos.position.x - (bPos.position.x + bBox.width);
	//	}

	//	if (aPos.position.y < bPos.position.y)
	//	{
	//		delta.y = bPos.position.y - (aPos.position.y + aBox.height);
	//	}
	//	else if (aPos.position.y > bPos.position.y)
	//	{
	//		delta.y = aPos.position.y - (bPos.position.y + bBox.height);
	//		
	//	}
	//	return delta;
	//}
};
