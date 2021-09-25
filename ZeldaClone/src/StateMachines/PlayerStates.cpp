#include "PlayerStates.h"
#include "../Game/Game.h"
#include "../Utilities/Timer.h"
#include "../Systems/SoundFXSystem.h"
#include "../Components/TriggerBoxComponent.h"

Timer timer;

// Player Idle State definitions
void IdleState::OnEnter(PlayerStateMachine* pOwner)
{
	//Logger::Log("Enter Idle State");
	Game::Instance()->GetPlayerItem() = false;
}

void IdleState::OnExit(PlayerStateMachine* pOwner)
{
	//Logger::Log("Exit Idle State");
}

void IdleState::Execute(PlayerStateMachine* pOwner)
{
	glm::vec2 playerSpeed = glm::vec2(0);
	auto player = Registry::Instance()->GetEntityByTag("player");
	if (player.GetComponent<RigidBodyComponent>().velocity.x != 0)
		pOwner->ChangeState(pOwner->moveState);
	else if (player.GetComponent<RigidBodyComponent>().velocity.y != 0)
		pOwner->ChangeState(pOwner->moveState);
	
}

void AttackState::OnEnter(PlayerStateMachine* pOwner)
{
	//Logger::Log("Enter Attack State");
}

void AttackState::OnExit(PlayerStateMachine* pOwner)
{
	//Logger::Log("Enter Exit State");
}

void AttackState::Execute(PlayerStateMachine* pOwner)
{

}

void MoveState::OnEnter(PlayerStateMachine* pOwner)
{
	//Logger::Log("Enter Move State");
}

void MoveState::OnExit(PlayerStateMachine* pOwner)
{
	//Logger::Log("Exit Move State");
}

void MoveState::Execute(PlayerStateMachine* pOwner)
{
	glm::vec2 playerSpeed = glm::vec2(0);
	auto player = Registry::Instance()->GetEntityByTag("player");
	if (player.GetComponent<RigidBodyComponent>().velocity.x != 0)
	{
		//Logger::Log("Player Moving");
	}	
	else if (player.GetComponent<RigidBodyComponent>().velocity.y != 0)
	{
		//Logger::Log("Player Moving");
	}
	else
		pOwner->ChangeState(pOwner->idleState);
	
	if (Game::Instance()->GetPlayerItem())
	{
		pOwner->ChangeState(pOwner->collectItemState);
	}
}

void CollectItemState::OnEnter(PlayerStateMachine* pOwner)
{
	timer.Start();
	trigItem = nullptr;
}

void CollectItemState::OnExit(PlayerStateMachine* pOwner)
{
	//Logger::Log("Exit Collect Item State");
	//trigItem = nullptr;
}

void CollectItemState::Execute(PlayerStateMachine* pOwner)
{
	// Get the player components that are needed
	auto& playerSprite = Registry::Instance()->GetEntityByTag("player").GetComponent<SpriteComponent>();
	auto& playerRigidBody = Registry::Instance()->GetEntityByTag("player").GetComponent<RigidBodyComponent>();
	auto& playerTransform = Registry::Instance()->GetEntityByTag("player").GetComponent<TransformComponent>();
	
	// Stop player movement!
	playerRigidBody = glm::vec2(0);

	// Set the sprite to the proper position based on the item collected
	// If the green tunic
	if (!Game::Instance()->GetGameItems().blueRing && !Game::Instance()->GetGameItems().redRing)
	{
		playerSprite.srcRect.x = playerSprite.width * 0;
		playerSprite.srcRect.y = playerSprite.height * 5;
	}
	else if (Game::Instance()->GetGameItems().blueRing && !Game::Instance()->GetGameItems().redRing) // Blue Tunic
	{
		playerSprite.srcRect.x = playerSprite.width * 4;
		playerSprite.srcRect.y = playerSprite.height * 5;
	}
	else if (Game::Instance()->GetGameItems().redRing) // Red Tunic
	{
		playerSprite.srcRect.x = playerSprite.width * 8;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}

	// Loop through the triggers and set the trigItem* to the active triggered item
	if (!itemCollected)
	{
		for (auto& trigger : Registry::Instance()->GetEntitiesByGroup("trigger"))
		{
			if (trigger.GetComponent<TriggerBoxComponent>().active)
			{
				trigItem = &trigger;
				itemCollected = true;
				break;
			}
		}
	}

	// Set the Triggered Item to the player position - the height of the player
	if (trigItem)
	{
		trigItem->GetComponent<TransformComponent>().position.x = playerTransform.position.x;
		trigItem->GetComponent<TransformComponent>().position.y - playerSprite.height;
	}
	
	// Wait for 1 second  then change back to idle State
	if (timer.GetTicks() > 1000)
	{
		//Logger::Log("Item Has Been Collected");
		itemCollected = false;
		trigItem->Kill();
		pOwner->ChangeState(pOwner->idleState);
	}
}
