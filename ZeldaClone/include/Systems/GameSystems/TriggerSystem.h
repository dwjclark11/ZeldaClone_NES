#pragma once
#include "ECS/ECS.h"
#include "Game/LevelLoader.h"

class EventManager;
class TriggerSystem : public System
{
  private:
	class Game& m_Game;
	class GameData& m_GameData;
	Registry& m_Registry;
	LevelLoader m_Loader;
	float m_Angle, m_HeartOffset;

  private:
	bool CheckInventory( SpecialItemType& item );
	void SetInventory( SpecialItemType& item );
	void StopPlayerMovement( Entity& player, Entity& trigger );

	void ChangeScene( Entity& player, Entity& trigger );
	void PushRocks( Entity& player, Entity& trigger );
	void RideRaft( Entity& player, Entity& trigger );
	void Transport( Entity& player, Entity& trigger );
	void UseLadder( Entity& player, Entity& trigger );
	void BombSecret( Entity& trigger );
	void UnlockDoor( Entity& trigger );
	void BuyShopItem( Entity& trigger );
	void EnterFairyCircle( Entity& player, Entity& trigger );
	void UpdateFairyCircle( Entity& player, Entity& trigger );
	void UpdateRaft( Entity& player, Entity& trigger );
	void UpdateMovingRocks( Entity& player, Entity& trigger );
	void UpdateLadder( Entity& player, Entity& trigger );

  public:
	TriggerSystem();
	~TriggerSystem() = default;

	void SubscribeToEvents( EventManager& eventManager );
	void OnTrigger( class CollisionEvent& event );
	void OnEnterTrigger( Entity& player, Entity& trigger );
	void Update( const float& dt );
	void SecretTrigger( Entity& trigger, bool startup = false );
};