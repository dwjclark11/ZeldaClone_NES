#include "PlayerStates.h"
#include "../Game/Game.h"
#include "../Utilities/Timer.h"
#include "../Systems/SoundFXSystem.h"
#include "../Systems/GameSystems/RenderHealthSystem.h"
#include "../Systems/GameSystems/RenderHUDSystem.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/ItemComponent.h"

#include "../Components/AIComponent.h"
#include "../States/GameOverState.h" 

Timer timer;
Game& game = *Game::Instance();

void SetSpecialItem(SpecialItemType special, Entity& player)
{
	auto& health = player.GetComponent<HealthComponent>();
	
	switch (special)
	{
	case NOT_SPECIAL:
		break;

	case WOOD_SWORD:
		game.GetGameItems().woodSword = true;
		break;

	case STEEL_SWORD:
		game.GetGameItems().steelSword = true;
		break;

	case MAGIC_SWORD:
		game.GetGameItems().magicSword = true;
		break;

	case FULL_HEART:
	{
		health.addHeart = true;
		break;
	}
	case RAFT:
		game.GetGameItems().raft = true;
		break;
	case POWER_BRACLET:
		game.GetGameItems().powerBraclet = true;
		break;
	case RED_CANDLE:
		game.GetGameItems().candle = true;
		break;
	case WOOD_BOOMERANG:
		game.GetGameItems().woodBoomerang = true;
		break;
	case MAGIC_BOOMERANG:
		game.GetGameItems().magicBoomerang = true;
		break;
	case LADDER:
		game.GetGameItems().ladder = true;
		break;
	default:
		break;
	}
}

// Player Idle State definitions
void IdleState::OnEnter(PlayerStateMachine* pOwner, Entity& entity)
{
	//Logger::Log("Enter Idle State");
	game.GetPlayerItem() = false;
}

void IdleState::OnExit(PlayerStateMachine* pOwner, Entity& entity)
{
	//Logger::Log("Exit Idle State");
}

void IdleState::Execute(PlayerStateMachine* pOwner, Entity& entity)
{
	glm::vec2 playerSpeed = glm::vec2(0);
	auto player = entity;
	
	auto& playerHealth = player.GetComponent<HealthComponent>();

	if (player.GetComponent<RigidBodyComponent>().velocity.x != 0)
		pOwner->ChangeState(pOwner->moveState, entity);
	else if (player.GetComponent<RigidBodyComponent>().velocity.y != 0)
		pOwner->ChangeState(pOwner->moveState, entity);

	// If the player is hurt while in Idle state --> Switch to hurt state
	if (playerHealth.isHurt)
		pOwner->ChangeState(pOwner->hurtState, entity);

}

void AttackState::OnEnter(PlayerStateMachine* pOwner, Entity& entity)
{
	//Logger::Log("Enter Attack State");
}

void AttackState::OnExit(PlayerStateMachine* pOwner, Entity& entity)
{
	//Logger::Log("Enter Exit State");
}

void AttackState::Execute(PlayerStateMachine* pOwner, Entity& entity)
{

}

void MoveState::OnEnter(PlayerStateMachine* pOwner, Entity& entity)
{
	//Logger::Log("Enter Move State");
}

void MoveState::OnExit(PlayerStateMachine* pOwner, Entity& entity)
{
	//Logger::Log("Exit Move State");
}

void MoveState::Execute(PlayerStateMachine* pOwner, Entity& entity)
{
	glm::vec2 playerSpeed = glm::vec2(0);
	auto player = entity;// Registry::Instance()->GetEntityByTag("player");
	
	auto& playerHealth = player.GetComponent<HealthComponent>();
	
	if (player.GetComponent<RigidBodyComponent>().velocity.x != 0)
	{
		//Logger::Log("Player Moving");
	}	
	else if (player.GetComponent<RigidBodyComponent>().velocity.y != 0)
	{
		//Logger::Log("Player Moving");
	}
	else
		pOwner->ChangeState(pOwner->idleState, entity);
	
	// If the player is collecting a special Item --> Move to the Collect Item State
	if (game.GetPlayerItem())
	{
		pOwner->ChangeState(pOwner->collectItemState, entity);
	}
	
	// If the player is hurt --> Move to the HurtState
	if (playerHealth.isHurt)
		pOwner->ChangeState(pOwner->hurtState, entity);
	
}

void CollectItemState::OnEnter(PlayerStateMachine* pOwner, Entity& entity)
{
	timer.Start();
}

void CollectItemState::Execute(PlayerStateMachine* pOwner, Entity& entity)
{
	// Get the player components that are needed
	auto& playerSprite = Registry::Instance()->GetEntityByTag("player").GetComponent<SpriteComponent>();
	auto& playerRigidBody = Registry::Instance()->GetEntityByTag("player").GetComponent<RigidBodyComponent>();
	auto& playerTransform = Registry::Instance()->GetEntityByTag("player").GetComponent<TransformComponent>();
	
	// Stop player movement!
	playerRigidBody = glm::vec2(0);

	// Set the sprite to the proper position based on the item collected
	// If the green tunic
	if (!game.GetGameItems().blueRing && !game.GetGameItems().redRing)
	{
		playerSprite.srcRect.x = playerSprite.width * 0;
		playerSprite.srcRect.y = playerSprite.height * 8;
	}
	else if (game.GetGameItems().blueRing && !game.GetGameItems().redRing) // Blue Tunic
	{
		playerSprite.srcRect.x = playerSprite.width * 4;
		playerSprite.srcRect.y = playerSprite.height * 8;
	}
	else if (game.GetGameItems().redRing) // Red Tunic
	{
		playerSprite.srcRect.x = playerSprite.width * 8;
		playerSprite.srcRect.y = playerSprite.height * 8;
	}

	// Loop through the triggers and set the trigItem* to the active triggered item
	if (!itemCollected)
	{
		for (auto& trigger : Registry::Instance()->GetEntitiesByGroup("trigger"))
		{
			auto& trig = trigger.GetComponent<TriggerBoxComponent>();
			if (trig.active)
			{
				
				itemCollected = true;
			}

			if (!movedTrigItem && itemCollected)
			{
				if (trigger.HasComponent<ItemComponent>())
				{
					const auto& special = trigger.GetComponent<ItemComponent>();

					SetSpecialItem(special.special, entity);

					auto& transform = trigger.GetComponent<TransformComponent>();

					trig.collectedTimer.Start();
					transform.position.x = playerTransform.position.x;
					transform.position.y = playerTransform.position.y - 64;
					trig.collected = true;
					movedTrigItem = true;
					break;
				}
			}
		}
	}
	
	// Wait for 1 second  then change back to idle State
	if (timer.GetTicks() > 2000)
	{
		pOwner->ChangeState(pOwner->idleState, entity);
	}
}

void CollectItemState::OnExit(PlayerStateMachine* pOwner, Entity& entity)
{
	
	movedTrigItem = false;
	itemCollected = false;
}

void PlayerHurtState::OnEnter(PlayerStateMachine* pOwner, Entity& entity) 
{
	auto player 			= entity;
	auto& playerHealth 		= player.GetComponent<HealthComponent>();
	auto& animation 		= player.GetComponent<AnimationComponent>();
	auto& sprite 			= player.GetComponent<SpriteComponent>();
	
	// Start hurt Invincibility Timer
	playerHealth.hurtTimer.Start();

	// Change the animation sprite offset 
	animation.numFrames = 3;
	animation.frameOffset = 64;
	animation.frameSpeedRate = 20;
}

void PlayerHurtState::Execute(PlayerStateMachine* pOwner, Entity& entity) 
{
	auto player 			= entity;
	auto& playerHealth 		= player.GetComponent<HealthComponent>();
	auto& animation 		= player.GetComponent<AnimationComponent>();
	auto& sprite 			= player.GetComponent<SpriteComponent>();
	
	// Check to see if the hurt timer is beyond the given setpoint
	if (playerHealth.hurtTimer.GetTicks() > 1000) // This time may need to be changed --> Create a constant?
	{
		playerHealth.isHurt = false;
		playerHealth.hurtTimer.Stop();
		pOwner->ChangeState(pOwner->idleState, entity);
	}

	if (playerHealth.healthPercentage <= 0)
	{
		pOwner->ChangeState(pOwner->deathState, entity);
	}
}

void PlayerHurtState::OnExit(PlayerStateMachine* pOwner, Entity& entity)
{
	// Change animation back to current sprites
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();

	animation.numFrames = 2;
	animation.frameOffset = 0;
	animation.frameSpeedRate = 10;
	sprite.srcRect.y = 0;
}

// Death State
void PlayerDeathState::OnEnter(PlayerStateMachine* pOwner, Entity& entity)
{
	// TODO: Start Death timer to run the death animation! 
	
	auto& health = entity.GetComponent <HealthComponent> ();
	auto& animation = entity.GetComponent<AnimationComponent>();
		
	health.deathTimer.Start();
	game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "link_die", 0, -1);
	animation.frameOffset = 0; 
	animation.numFrames = 4; 
	animation.frameSpeedRate = 10;
	animation.vertical = false;
}
void PlayerDeathState::Execute(PlayerStateMachine* pOwner, Entity& entity)
{
	auto& health = entity.GetComponent <HealthComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();

	game.GetPlayerDead() = true;

	if (health.deathTimer.GetTicks() > 3000)
	{
		sprite.assetID = "player_death";

		animation.frameOffset = 0;
		animation.numFrames = 8;
		animation.frameSpeedRate = 20;
		animation.isLooped = false;
		animation.vertical = false;

		if (health.deathTimer.GetTicks() > 3500)
		{
			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, -1);
			Registry::Instance()->GetSystem<RenderHUDSystem>().OnExit();
			Registry::Instance()->GetSystem<RenderHealthSystem>().OnExit();
			game.GetStateMachine()->PopState();
			game.GetStateMachine()->PushState(new GameOverState());
			pOwner->ChangeState(pOwner->idleState, entity);
		}
	}
}

void PlayerDeathState::OnExit(PlayerStateMachine* pOwner, Entity& entity)
{
	entity.Kill();
	// Animation does not have to return to regular because the entity is destroyed!
}

