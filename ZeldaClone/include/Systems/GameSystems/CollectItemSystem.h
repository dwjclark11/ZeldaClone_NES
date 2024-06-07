#pragma once
#include "../../ECS/ECS.h"

class CollisionEvent;
class EventManager;

class CollectItemSystem : public System
{
  private:
	class Game& m_Game;
	class GameData& m_GameData;
	class Registry& m_Registry;

  private:
	void OnPlayerGetsItem( Entity item );
	void OnBoomerangGetsItem( Entity& item, Entity& boomerang );
	void OnCollision( CollisionEvent& event );

  public:
	CollectItemSystem();
	~CollectItemSystem() = default;

	void SubscribeToEvents( EventManager& eventManager );
	void Update();
};
