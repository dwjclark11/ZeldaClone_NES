#pragma once
#include "../../ECS/ECS.h"

class EventManager;
class CollisionEvent;

class DamageSystem : public System
{
private:
	class Game& game;
public:
	
	DamageSystem();

	void SubscribeToEvents(EventManager& eventManager);

	void OnCollision(CollisionEvent& event);

	void OnPlayerHitsProjectile(Entity projectile, Entity player);

	void OnPlayerSwordHitsEnemy(Entity sword, Entity enemy);
	
	void OnEnemyHitsPlayerProjectile(Entity enemy, Entity projectile);
	
	void OnEnemyHitsPlayer(Entity enemy, Entity player);

	void OnEnemyHitsBoomerang(Entity enemy, Entity boomerang);
};