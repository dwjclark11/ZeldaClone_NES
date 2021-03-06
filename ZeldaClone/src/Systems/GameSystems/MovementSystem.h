#pragma once
#include "../../ECS/ECS.h"

const int MAX_WORLD_WIDTH = 16;
const int MAX_WORLD_HEIGHT = 8;

class MovementSystem : public System
{
private:
	class Game& game;
public:
	MovementSystem();

	void Update(const double& deltaTime);
	void SubscribeToEvents(const std::unique_ptr<class EventManager>& eventManager);
	void OnCollision(class CollisionEvent& colEvent);
	void OnEnemyHitsObstacle(Entity enemy, Entity obstacle);
	void OnPlayerHitsObstacle(Entity obstacle, Entity player);
};