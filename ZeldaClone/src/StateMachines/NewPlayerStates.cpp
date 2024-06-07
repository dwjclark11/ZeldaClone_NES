#include "StateMachines/NewPlayerStates.h"
#include "ECS/ECS.h"

#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/Timer.h"
#include "Systems/SoundFXSystem.h"
#include "Systems/GameSystems/RenderHealthSystem.h"
#include "Systems/GameSystems/RenderHUDSystem.h"
#include "Components/TransformComponent.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/GameComponent.h"
#include "Components/SceneChangeComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ItemComponent.h"

#include "Components/AIComponent.h"
#include "States/GameOverState.h"
#include "States/GameState.h"
#include "States/GameStateMachine.h"
#include "Utilities/GameData.h"
#include "Utilities/Camera.h"

Timer timer;
Game& game = Game::Instance();

void SetSpecialItem( SpecialItemType special, Entity& player )
{
	auto& health = player.GetComponent<HealthComponent>();
	auto& gameData = GameData::GetInstance();

	switch ( special )
	{
	case SpecialItemType::NOT_SPECIAL: break;

	case SpecialItemType::WOOD_SWORD: gameData.AddItem( GameData::GameItems::WOOD_SWORD ); break;

	case SpecialItemType::STEEL_SWORD: gameData.AddItem( GameData::GameItems::SILVER_SWORD ); break;

	case SpecialItemType::MAGIC_SWORD: gameData.AddItem( GameData::GameItems::MAGIC_SWORD ); break;

	case SpecialItemType::FULL_HEART: health.bAddHeart = true; break;

	case SpecialItemType::RAFT: gameData.AddItem( GameData::GameItems::RAFT ); break;
	case SpecialItemType::POWER_BRACLET: gameData.AddItem( GameData::GameItems::POWER_BRACLET ); break;
	case SpecialItemType::RED_CANDLE: gameData.AddItem( GameData::GameItems::CANDLE ); break;
	case SpecialItemType::WOOD_BOOMERANG: gameData.AddItem( GameData::GameItems::BOOMERANG ); break;
	case SpecialItemType::MAGIC_BOOMERANG: gameData.AddItem( GameData::GameItems::MAGIC_BOOMERANG ); break;
	case SpecialItemType::LADDER: gameData.AddItem( GameData::GameItems::LADDER ); break;
	case SpecialItemType::ARROWS: gameData.AddItem( GameData::GameItems::WOOD_ARROWS ); break;
	case SpecialItemType::MAGIC_ARROWS: gameData.AddItem( GameData::GameItems::MAGIC_ARROWS ); break;
	case SpecialItemType::WOOD_BOW: gameData.AddItem( GameData::GameItems::BOW ); break;
	case SpecialItemType::TRIFORCE_PIECE: {
		gameData.AddTriforcePieces( 1 );
		break;
	}
	default: break;
	}
}

void SetItemCollect( ItemCollectType type )
{
	auto& gameData = GameData::GetInstance();
	switch ( type )
	{
	case ItemCollectType::BOMBS: gameData.AddBombs( 4 ); break;
	}
}

void CheckItem( ItemCollectType type )
{
}

// Player Idle State definitions
void IdleState::OnEnter( Entity& entity )
{
}

void IdleState::OnExit( Entity& entity )
{
}

void IdleState::Update( Entity& entity )
{
	const auto& player = entity;
	auto& sm = game.GetPlayer()->GetPlayerStateMachine();
	auto& playerHealth = player.GetComponent<HealthComponent>();

	// If the player is hurt while in Idle state --> Switch to hurt state
	if ( playerHealth.bIsHurt )
	{
		sm.AddState( std::make_unique<PlayerHurtState>() );
		sm.ChangeState( entity );
	}

	auto& rigidbody = player.GetComponent<RigidBodyComponent>();

	if ( rigidbody.velocity.x != 0 || rigidbody.velocity.y != 0 )
	{
		sm.AddState( std::make_unique<MoveState>() );
		sm.ChangeState( entity );
	}
}

void AttackState::OnEnter( Entity& entity )
{
}

void AttackState::OnExit( Entity& entity )
{
}

void AttackState::Update( Entity& entity )
{
}

void MoveState::OnEnter( Entity& entity )
{
}

void MoveState::OnExit( Entity& entity )
{
}

void MoveState::Update( Entity& entity )
{
	glm::vec2 playerSpeed = glm::vec2( 0 );
	auto player = entity;

	auto& playerHealth = player.GetComponent<HealthComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& sm = game.GetPlayer()->GetPlayerStateMachine();
	const auto& camera = game.GetCamera();

	if ( player.GetComponent<RigidBodyComponent>().velocity.x == 0 &&
		 player.GetComponent<RigidBodyComponent>().velocity.y == 0 )
	{
		sm.AddState( std::make_unique<IdleState>() );
		sm.ChangeState( entity );
	}

	// If the player is collecting a special Item --> Move to the Collect Item State
	if ( game.GetPlayer()->GetPlayerItem() )
	{
		sm.AddState( std::make_unique<CollectItemState>() );
		sm.ChangeState( entity );
	}

	// If the player is hurt --> Move to the HurtState
	if ( playerHealth.bIsHurt )
	{
		// pOwner->ChangeState(pOwner->hurtState, entity);
		sm.AddState( std::make_unique<PlayerHurtState>() );
		sm.ChangeState( entity );
	}

	if ( game.GetPlayer()->GetPlayerOnStairs() )
	{
		sm.AddState( std::make_unique<PlayerStairsState>() );
		sm.ChangeState( entity );
	}
}

void CollectItemState::OnEnter( Entity& entity )
{
	timer.Start();
	if ( game.GetPlayer()->ObtainedTriforcePiece() )
	{
		game.GetCamera().StartScreenFlash();
	}
}

void CollectItemState::Update( Entity& entity )
{
	// Get the Game data
	auto& gameData = GameData::GetInstance();
	// Get the player components that are needed
	const auto& player = entity;
	auto& playerSprite = player.GetComponent<SpriteComponent>();
	auto& playerRigidBody = player.GetComponent<RigidBodyComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& sm = game.GetPlayer()->GetPlayerStateMachine();
	auto& camera = game.GetCamera();
	// Stop player movement!
	playerRigidBody.velocity = glm::vec2( 0 );
	bool obtained_triforce = game.GetPlayer()->ObtainedTriforcePiece();
	int time = obtained_triforce ? 9000 : 2000;

	for ( auto& trigger : Registry::Instance().GetEntitiesByGroup( "trigger" ) )
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();

		if ( !trig.bActive || !trigger.HasComponent<ItemComponent>() )
			continue;

		const auto& special = trigger.GetComponent<ItemComponent>();
		if ( special.special != SpecialItemType::NOT_SPECIAL )
		{
			SetSpecialItem( special.special, entity );
		}

		if ( special.type != ItemCollectType::DEFAULT )
			SetItemCollect( special.type );

		auto& transform = trigger.GetComponent<TransformComponent>();

		trig.collectedTimer.Start();
		transform.position.x = playerTransform.position.x + 32;
		transform.position.y = playerTransform.position.y - 64;
		trig.bActive = false;
		trig.bCollected = true;

		if ( obtained_triforce )
		{
			gameData.SetTriggerBoxValues( trig );
			if ( trigger.HasComponent<SceneChangeComponent>() )
				gameData.SetScengeChangeValues( trigger.GetComponent<SceneChangeComponent>() );
		}
		break;
	}

	// Set the sprite to the proper position based on the item collected
	// If the green tunic
	if ( !gameData.HasItem( GameData::GameItems::BLUE_RING ) && !gameData.HasItem( GameData::GameItems::RED_RING ) )
	{
		playerSprite.srcRect.x = playerSprite.width * ( obtained_triforce ? 1 : 0 );
		playerSprite.srcRect.y = playerSprite.height * 8;
	}
	else if ( gameData.HasItem( GameData::GameItems::RED_RING ) )
	{
		playerSprite.srcRect.x = playerSprite.width * ( obtained_triforce ? 9 : 8 );
		playerSprite.srcRect.y = playerSprite.height * 8;
	}
	else if ( gameData.HasItem( GameData::GameItems::BLUE_RING ) )
	{
		playerSprite.srcRect.x = playerSprite.width * ( obtained_triforce ? 5 : 4 );
		playerSprite.srcRect.y = playerSprite.height * 8;
	}

	// Wait for 2 seconds then change back to idle State
	if ( timer.GetTicks() > time )
	{
		if ( !obtained_triforce )
		{
			sm.AddState( std::make_unique<IdleState>() );
			sm.ChangeState( entity );
			return;
		}

		if ( camera.CurtainClosed() )
		{
			const auto& trigger_component = gameData.GetTrigger();
			const auto& scene_component = gameData.GetScene();
			auto newSceneTrigger = Registry::Instance().CreateEntity();
			newSceneTrigger.Group( "trigger" );
			newSceneTrigger.AddComponent<BoxColliderComponent>( 16, 16 );

			newSceneTrigger.AddComponent<TransformComponent>(
				glm::vec2{ playerTransform.position.x, playerTransform.position.y + 80 },
				glm::vec2{ playerTransform.scale.x, playerTransform.scale.y } );

			newSceneTrigger.AddComponent<TriggerBoxComponent>( TriggerBoxComponent{
				.triggerType = TriggerType::SCENE_CHANGE,
				.transportOffset =
					glm::vec2{ trigger_component.transportOffset.x, trigger_component.transportOffset.y },
				.cameraOffset = glm::vec2{ trigger_component.cameraOffset.x, trigger_component.cameraOffset.x } } );

			newSceneTrigger.AddComponent<SceneChangeComponent>(
				SceneChangeComponent{ .levelMusic = scene_component.levelMusic,
									  .assetFile = scene_component.assetFile,
									  .enemyFile = scene_component.enemyFile,
									  .colliderFile = scene_component.colliderFile,
									  .tileMapName = scene_component.tileMapName,
									  .tileImageName = scene_component.tileImageName,
									  .mapImageName = scene_component.mapImageName,
									  .entityFileName = scene_component.entityFileName,
									  .triggerFile = scene_component.triggerFile,
									  .imageHeight = scene_component.imageHeight,
									  .imageWidth = scene_component.imageWidth } );

			newSceneTrigger.AddComponent<GameComponent>();

			sm.AddState( std::make_unique<IdleState>() );
			sm.ChangeState( entity );
		}
		else
		{
			camera.StartCurtainClose();
		}
	}
}

void CollectItemState::OnExit( Entity& entity )
{
	timer.Stop();
}

void PlayerHurtState::OnEnter( Entity& entity )
{
	auto player = entity;
	auto& playerHealth = player.GetComponent<HealthComponent>();
	auto& animation = player.GetComponent<AnimationComponent>();
	auto& sprite = player.GetComponent<SpriteComponent>();
	auto& sm = game.GetPlayer()->GetPlayerStateMachine();

	// Start hurt Invincibility Timer
	playerHealth.hurtTimer.Start();

	// Change the animation sprite offset
	animation.numFrames = 3;
	animation.frameOffset = 64;
	animation.frameSpeedRate = 20;
}

void PlayerHurtState::Update( Entity& entity )
{
	auto player = entity;
	auto& playerHealth = player.GetComponent<HealthComponent>();
	auto& animation = player.GetComponent<AnimationComponent>();
	auto& sprite = player.GetComponent<SpriteComponent>();
	auto& sm = game.GetPlayer()->GetPlayerStateMachine();

	// Check to see if the hurt timer is beyond the given setpoint
	if ( playerHealth.hurtTimer.GetTicks() > 1000 ) // This time may need to be changed --> Create a constant?
	{
		playerHealth.bIsHurt = false;
		playerHealth.hurtTimer.Stop();

		sm.AddState( std::make_unique<IdleState>() );
		sm.ChangeState( entity );
	}

	if ( playerHealth.healthPercentage <= 0 )
	{
		sm.AddState( std::make_unique<PlayerDeathState>() );
		sm.ChangeState( entity );
	}
}

void PlayerHurtState::OnExit( Entity& entity )
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
void PlayerDeathState::OnEnter( Entity& entity )
{
	auto& health = entity.GetComponent<HealthComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();

	health.deathTimer.Start();
	Game::Instance().GetSoundPlayer().PlaySoundFX( "link_die", 0, SoundChannel::ANY );
	animation.frameOffset = 0;
	animation.numFrames = 4;
	animation.frameSpeedRate = 10;
	animation.vertical = false;
}
void PlayerDeathState::Update( Entity& entity )
{
	auto& health = entity.GetComponent<HealthComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& sm = game.GetPlayer()->GetPlayerStateMachine();

	game.GetPlayer()->SetPlayerDead( true );

	if ( health.deathTimer.GetTicks() > 3000 )
	{
		sprite.assetID = "player_death";

		animation.frameOffset = 0;
		animation.numFrames = 8;
		animation.frameSpeedRate = 20;
		animation.isLooped = false;
		animation.vertical = false;

		if ( health.deathTimer.GetTicks() > 3500 )
		{
			Game::Instance().GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::ANY );
			Registry::Instance().GetSystem<RenderHUDSystem>().OnExit();
			game.GetStateMachine()->PushState( std::make_unique<GameOverState>() );

			sm.AddState( std::make_unique<IdleState>() );
			sm.ChangeState( entity );
		}
	}
}

void PlayerDeathState::OnExit( Entity& entity )
{
	entity.Kill();
	// Animation does not have to return to regular because the entity is destroyed!
}

void PlayerStairsState::OnEnter( Entity& entity )
{
	// Check to see if the timer is already started, if so stop the timer
	if ( timer.isStarted() )
		timer.Stop();

	steps++;

	timer.Start();

	// Play the stairs soundFX
	Game::Instance().GetSoundPlayer().PlaySoundFX( "stairs", 0, SoundChannel::ANY );
}

void PlayerStairsState::Update( Entity& entity )
{
	auto& playerSprite = entity.GetComponent<SpriteComponent>();
	auto& sm = game.GetPlayer()->GetPlayerStateMachine();
	// increment the steps based on time
	if ( timer.GetTicks() >= 300 * steps )
	{
		playerSprite.srcRect.x = playerSprite.width * 2;
		playerSprite.srcRect.y = playerSprite.height * ( 8 + steps );
		steps++;
	}

	if ( steps >= 4 )
	{
		sm.AddState( std::make_unique<IdleState>() );
		sm.ChangeState( entity );
	}
}

void PlayerStairsState::OnExit( Entity& entity )
{
	steps = 0;
	game.GetPlayer()->SetPlayerOnStairs( false );
	game.GetPlayer()->SetStairsFinished( true );
}