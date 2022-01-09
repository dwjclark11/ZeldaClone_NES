#include "../GameSystems/KeyboardControlSystem.h"

directionE KeyboardControlSystem::dir = NONE;
bool KeyboardControlSystem::keyDown = false;

void KeyboardControlSystem::UpdatePlayer()
{
	auto player = Registry::Instance()->GetEntityByTag("player");
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& playerCollider = player.GetComponent<BoxColliderComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	auto& playerSprite = player.GetComponent<SpriteComponent>();
	auto& playerControl = player.GetComponent<KeyboardControlComponent>();

	auto shield = Registry::Instance()->GetEntityByTag("the_shield");
	auto& shieldTransform = shield.GetComponent<TransformComponent>();
	auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
	auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

	auto sword = Registry::Instance()->GetEntityByTag("the_sword");
	auto& swordTransform = sword.GetComponent<TransformComponent>();
	auto& swordCollider = sword.GetComponent<BoxColliderComponent>();
	auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

	// reset the sword
	swordRigidbody = glm::vec2(0);
	swordCollider.offset = glm::vec2(64, 60);
	swordCollider.height = 4;
	swordCollider.width = 4;
	
	// Reset Player velocity and box collider
	playerRigidbody = glm::vec2(0);
	playerCollider.offset = glm::vec2(45, 45);

	if (dir == UP)
	{
		playerSprite.srcRect.x = playerSprite.width * 2;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == RIGHT)
	{
		playerSprite.srcRect.x = playerSprite.width * 3;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == DOWN)
	{
		playerSprite.srcRect.x = playerSprite.width * 0;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == LEFT)
	{
		playerSprite.srcRect.x = playerSprite.width * 1;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
			
	// Reset the Shield based on player direction
	shieldRigidbody = glm::vec2(0);
	if (dir == UP)
	{
		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(48, 32);
	}
	else if (dir == RIGHT)
	{
		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(90, 56);
	}
	else if (dir == DOWN)
	{
		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(40, 84);
	}
	else if (dir == LEFT)
	{
		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(30, 50);
	}
}
