#pragma once
#include "ECS/ECS.h"

class EventManager;
class CollisionEvent;

class DamageSystem : public System
{
  private:
	class Game& m_Game;

  private:
	void OnPlayerHitsProjectile( Entity projectile, Entity player );
	void OnPlayerSwordHitsEnemy( Entity sword, Entity enemy );
	void OnEnemyHitsPlayerProjectile( Entity enemy, Entity projectile );
	void OnEnemyHitsPlayer( Entity enemy, Entity player );
	void OnEnemyHitsBoomerang( Entity enemy, Entity boomerang );

  public:
	DamageSystem();
	~DamageSystem() = default;

	void SubscribeToEvents( EventManager& eventManager );
	void OnCollision( CollisionEvent& event );
};