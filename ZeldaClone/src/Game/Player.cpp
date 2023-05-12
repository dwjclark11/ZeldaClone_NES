#include "Player.h"
#include "../Utilities/GameData.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../inputs/InputManager.h"
#include "../inputs/Keyboard.h"
#include "../inputs/Gamepad.h"

Player::Player(Entity player, Entity sword, Entity shield)
	: m_Player(player), m_Sword(sword), m_Shield(shield)
	, m_bPlayerCreated{true}, m_bPlayerItem{false}, m_bPlayerDead{false}
	, m_bOnStairs{false}, m_bOnRaft{false}, m_bStairsFinished{false}, m_bGetTriforcePiece{false}
	, m_PlayerPos{glm::vec2(0)}
	, m_GameData(GameData::GetInstance())
	, m_AttackValue{0}
{

}

Player::~Player()
{

}

void Player::UpdateStateMachine()
{
	m_PSM.GetCurrentState()->Update(m_Player);
}
void Player::UpdatePlayerColliders()
{
	const auto& playerEnt = GetPlayer();

	auto& playerTransform = playerEnt.GetComponent<TransformComponent>();
	auto& playerCollider = playerEnt.GetComponent<BoxColliderComponent>();
	auto& playerRigidbody = playerEnt.GetComponent<RigidBodyComponent>();
	auto& playerSprite = playerEnt.GetComponent<SpriteComponent>();
	auto& playerControl = playerEnt.GetComponent<KeyboardControlComponent>();

	const auto& shield = GetShield();
	auto& shieldTransform = shield.GetComponent<TransformComponent>();
	auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
	auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

	const auto& sword = GetSword();
	auto& swordTransform = sword.GetComponent<TransformComponent>();
	auto& swordCollider = sword.GetComponent<BoxColliderComponent>();
	auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

	// reset the sword
	swordRigidbody = glm::vec2(0);
	swordCollider.offset = glm::vec2(64, 60);
	swordCollider.height = 4;
	swordCollider.width = 4;
	
	// Reset Player velocity and box collider
	playerRigidbody.velocity = glm::vec2(0);
	playerCollider.offset = glm::vec2(45, 45);

	// Reset the Shield based on player direction
	shieldRigidbody.velocity = glm::vec2(0);

	switch (playerRigidbody.dir)
	{
	case RigidBodyComponent::Dir::UP:
		playerSprite.srcRect.x = playerSprite.width * 2;
		playerSprite.srcRect.y = playerSprite.height * 0;

		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(48, 32);
		break;
	case RigidBodyComponent::Dir::RIGHT:
		playerSprite.srcRect.x = playerSprite.width * 3;
		playerSprite.srcRect.y = playerSprite.height * 0;

		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(90, 56);
		break;
	case RigidBodyComponent::Dir::DOWN:
		playerSprite.srcRect.x = playerSprite.width * 0;
		playerSprite.srcRect.y = playerSprite.height * 0;

		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(40, 84);
		break;
	case RigidBodyComponent::Dir::LEFT:
		playerSprite.srcRect.x = playerSprite.width * 1;
		playerSprite.srcRect.y = playerSprite.height * 0;

		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(30, 50);
		break;
	}
}


void Player::UpdatePlayer()
{
	// Update player attack 
	if (m_GameData.HasItem(GameData::GameItems::MAGIC_SWORD))
	{
		m_AttackValue = 6;
	}
	else if (m_GameData.HasItem(GameData::GameItems::SILVER_SWORD))
	{
		m_AttackValue = 3;
	}
	else if (m_GameData.HasItem(GameData::GameItems::WOOD_SWORD))
	{
		m_AttackValue = 1;
	}


}