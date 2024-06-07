#pragma once
#include "ECS/ECS.h"
#include "StateMachines/NewStateMachine.h"
#include <glm/glm.hpp>

class Player
{
  private:
	class GameData& m_GameData;
	Entity m_Player, m_Sword, m_Shield;
	bool m_bPlayerCreated, m_bPlayerItem, m_bPlayerDead, m_bAttacking, m_bHoldPlayer;
	bool m_bOnStairs, m_bOnRaft, m_bStairsFinished, m_bGetTriforcePiece;
	StateMachine m_PSM;
	glm::vec2 m_PlayerPos;
	int m_AttackValue, m_PrevAttackValue;

  public:
	Player( Entity player, Entity sword, Entity shield );
	~Player();

	Entity& GetPlayer() { return m_Player; }
	Entity& GetSword() { return m_Sword; }
	Entity& GetShield() { return m_Shield; }

	void UpdateStateMachine();
	void UpdatePlayer( class Game& game, class InputManager& inputManager );
	void UpdatePlayerColliders();

	inline const bool GetPlayerCreated() const { return m_bPlayerCreated; }
	inline void SetPlayerCreated( bool created ) { m_bPlayerCreated = created; }

	inline const bool GetPlayerItem() const { return m_bPlayerItem; }
	inline void SetPlayerItem( bool item ) { m_bPlayerItem = item; }

	inline const bool GetPlayerDead() const { return m_bPlayerDead; }
	inline void SetPlayerDead( bool dead ) { m_bPlayerDead = dead; }

	inline const bool GetPlayerOnStairs() const { return m_bOnStairs; }
	inline void SetPlayerOnStairs( bool stairs ) { m_bOnStairs = stairs; }

	inline const bool GetStairsFinished() const { return m_bStairsFinished; }
	inline void SetStairsFinished( bool finished ) { m_bStairsFinished = finished; }

	inline void SetOnRaft( bool raft ) { m_bOnRaft = raft; }
	inline const bool GetRaft() const { return m_bOnRaft; }

	inline void SetObtainedTriforce( bool obtain ) { m_bGetTriforcePiece = obtain; }
	inline const bool ObtainedTriforcePiece() const { return m_bGetTriforcePiece; }

	StateMachine& GetPlayerStateMachine() { return m_PSM; }

	inline const glm::vec2& GetPlayerPos() const { return m_PlayerPos; }
	inline void SetPlayerPos( glm::vec2 pos ) { m_PlayerPos = pos; }

	const int GetAttackValue() const { return m_AttackValue; }
	inline const bool IsAttacking() const { return m_bAttacking; }
	inline void SetAttacking( bool bAttacking ) { m_bAttacking = bAttacking; }
	inline void HoldPlayer() { m_bHoldPlayer = true; }
	inline void ReleasePlayer() { m_bHoldPlayer = false; }
	inline const bool PlayerHeld() { return m_bHoldPlayer; }
};