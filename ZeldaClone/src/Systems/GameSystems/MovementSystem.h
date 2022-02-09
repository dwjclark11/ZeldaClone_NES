#pragma once
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../Game/Game.h"
#include "../../ECS/ECS.h"
#include "KeyboardControlSystem.h"

const int MAX_WORLD_WIDTH = 16;
const int MAX_WORLD_HEIGHT = 8;

class MovementSystem : public System
{
private:
	class Game& game;
public:
	MovementSystem();

	void Update(const double& deltaTime);
	void SubscribeToEvents(const std::unique_ptr<EventManager>& eventManager);
	void OnCollision(CollisionEvent& colEvent);
	void OnEnemyHitsObstacle(Entity enemy, Entity obstacle);
	void OnPlayerHitsObstacle(Entity obstacle, Entity player);
};