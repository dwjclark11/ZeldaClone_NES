#include "Systems/GameSystems/TriggerSystem.h"
#include "Components/TransformComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/GameComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SceneChangeComponent.h"
#include "Components/SecretComponent.h"
#include "Systems/GameSystems/RenderTileSystem.h"
#include "Systems/RenderTextSystem.h"
#include "Events/CollisionEvent.h"
#include "Events/TriggerEvent.h"
#include "Events/EventManager.h"
#include "Game/Game.h"
#include "Game/Player.h"

#include "Utilities/Timer.h"
#include "Utilities/GameData.h"
#include "Utilities/Camera.h"
#include "Utilities/Utility.h"
#include "Systems/SoundFXSystem.h"
#include "States/GameState.h"

constexpr float HeartOffset = 0.75f;
constexpr float AngleIncrease = 0.05f;

bool TriggerSystem::CheckInventory( SpecialItemType& item )
{
	// Check to see the item is in the inventory
	switch ( item )
	{
	case SpecialItemType::WOOD_SWORD: return m_GameData.HasItem( GameData::GameItems::WOOD_SWORD );
	case SpecialItemType::STEEL_SWORD: return m_GameData.HasItem( GameData::GameItems::SILVER_SWORD );
	case SpecialItemType::MAGIC_SWORD: return m_GameData.HasItem( GameData::GameItems::MAGIC_SWORD );
	case SpecialItemType::RED_CANDLE: return m_GameData.HasItem( GameData::GameItems::CANDLE );
	case SpecialItemType::WOOD_BOW: return m_GameData.HasItem( GameData::GameItems::BOW );
	case SpecialItemType::ARROWS: return m_GameData.HasItem( GameData::GameItems::WOOD_ARROWS );
	case SpecialItemType::MAGIC_ARROWS: return m_GameData.HasItem( GameData::GameItems::MAGIC_ARROWS );
	default: return false; break;
	}

	// If the item is not in the inventory return false
	return false;
}

void TriggerSystem::SetInventory( SpecialItemType& item )
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& playerHealth = playerEnt.GetComponent<HealthComponent>();

	switch ( item )
	{
	case SpecialItemType::NOT_SPECIAL: break;

	case SpecialItemType::WOOD_SWORD: m_GameData.AddItem( GameData::GameItems::WOOD_SWORD ); break;

	case SpecialItemType::STEEL_SWORD: m_GameData.AddItem( GameData::GameItems::SILVER_SWORD ); break;

	case SpecialItemType::MAGIC_SWORD: m_GameData.AddItem( GameData::GameItems::MAGIC_SWORD ); break;

	case SpecialItemType::FULL_HEART: {
		playerHealth.bAddHeart = true;
		break;
	}
	case SpecialItemType::RAFT: m_GameData.AddItem( GameData::GameItems::RAFT ); break;
	case SpecialItemType::POWER_BRACLET: m_GameData.AddItem( GameData::GameItems::POWER_BRACLET ); break;
	case SpecialItemType::RED_CANDLE: m_GameData.AddItem( GameData::GameItems::CANDLE ); break;
	case SpecialItemType::WOOD_BOOMERANG: m_GameData.AddItem( GameData::GameItems::BOOMERANG ); break;
	case SpecialItemType::MAGIC_BOOMERANG: m_GameData.AddItem( GameData::GameItems::MAGIC_BOOMERANG ); break;
	case SpecialItemType::LADDER: m_GameData.AddItem( GameData::GameItems::LADDER ); break;
	case SpecialItemType::WOOD_BOW: m_GameData.AddItem( GameData::GameItems::BOW ); break;
	default: break;
	}
}

void TriggerSystem::StopPlayerMovement( Entity& player, Entity& trigger )
{
	if ( player.HasComponent<TransformComponent>() && player.HasComponent<BoxColliderComponent>() )
	{
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();

		auto& obstacleTransform = trigger.GetComponent<TransformComponent>();
		auto& obstacleCollider = trigger.GetComponent<BoxColliderComponent>();

		switch ( playerRigidbody.dir )
		{
		case RigidBodyDir::UP:
			playerTransform.bCollision = true;
			playerRigidbody.velocity.y = 0;
			playerTransform.position.y =
				( obstacleTransform.position.y - playerCollider.offset.y + obstacleCollider.offset.y ) +
				( obstacleCollider.height * obstacleTransform.scale.y );
			playerTransform.position.x = playerTransform.position.x;
			break;
		case RigidBodyDir::RIGHT:
			playerTransform.bCollision = true;
			playerRigidbody.velocity.x = 0;
			playerTransform.position.x =
				( obstacleTransform.position.x - playerCollider.offset.x + obstacleCollider.offset.x ) -
				( playerCollider.width * playerTransform.scale.x );
			playerTransform.position.y = playerTransform.position.y;
			break;
		case RigidBodyDir::DOWN:
			playerTransform.bCollision = true;
			playerRigidbody.velocity.y = 0;
			playerTransform.position.y =
				( obstacleTransform.position.y - playerCollider.offset.y + obstacleCollider.offset.y ) -
				( playerCollider.height * playerTransform.scale.y );
			playerTransform.position.x = playerTransform.position.x;
			break;
		case RigidBodyDir::LEFT:
			playerTransform.bCollision = true;
			playerRigidbody.velocity.x = 0;
			playerTransform.position.x =
				( obstacleTransform.position.x - playerCollider.offset.x + obstacleCollider.offset.x ) +
				( obstacleCollider.width * obstacleTransform.scale.x );
			playerTransform.position.y = playerTransform.position.y;
			break;
		}
	}
}

void TriggerSystem::SecretTrigger( Entity& trigger, bool startup )
{
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& secretCollider = trigger.GetComponent<BoxColliderComponent>();
	auto& secretTrigger = trigger.GetComponent<TriggerBoxComponent>();
	auto& secretTransform = trigger.GetComponent<TransformComponent>();

	if ( !secret.bFound && !startup )
	{
		TriggerType trigType = m_Loader.ConvertStringToTriggerType( secret.newTrigger );

		auto secretArea = m_Registry.CreateEntity();
		secretArea.Group( "trigger" );
		secretArea.AddComponent<BoxColliderComponent>(
			secretCollider.width, secretCollider.height, secretCollider.offset );

		secretArea.AddComponent<TriggerBoxComponent>(
			TriggerBoxComponent{ .triggerType = trigType,
								 .transportOffset = secretTrigger.transportOffset,
								 .cameraOffset = secretTrigger.cameraOffset } );

		if ( trigger.HasComponent<SceneChangeComponent>() )
		{
			const auto& scene = trigger.GetComponent<SceneChangeComponent>();
			secretArea.AddComponent<SceneChangeComponent>( SceneChangeComponent{ .levelMusic = scene.levelMusic,
																				 .assetFile = scene.assetFile,
																				 .enemyFile = scene.enemyFile,
																				 .colliderFile = scene.colliderFile,
																				 .tileMapName = scene.tileMapName,
																				 .tileImageName = scene.tileImageName,
																				 .mapImageName = scene.mapImageName,
																				 .entityFileName = scene.entityFileName,
																				 .triggerFile = scene.triggerFile,
																				 .imageHeight = scene.imageHeight,
																				 .imageWidth = scene.imageWidth } );
		}

		secretArea.AddComponent<TransformComponent>(
			secretTransform.position, secretTransform.scale, secretTransform.rotation );
		secretArea.AddComponent<SpriteComponent>( SpriteComponent{
			.assetID = secret.newSpriteAssetID,
			.width = secret.spriteWidth,
			.height = secret.spriteHeight,
			.layer = 1,
			.bIsFixed = false,
			.srcRect = SDL_Rect{ secret.spriteSrcX, secret.spriteSrcY, secret.spriteWidth, secret.spriteHeight } } );

		secretArea.AddComponent<GameComponent>();

		secret.bFound = true;
		m_GameData.SetSecretFound( secret.locationID, true );
		m_Game.GetSoundPlayer().PlaySoundFX( "secret", 0, SoundChannel::ANY );
	}
	else if ( secret.bFound && startup )
	{
		TriggerType trigType = m_Loader.ConvertStringToTriggerType( secret.newTrigger );

		auto secretArea = m_Registry.CreateEntity();
		secretArea.Group( "trigger" );
		secretArea.AddComponent<BoxColliderComponent>(
			secretCollider.width, secretCollider.height, secretCollider.offset );
		secretArea.AddComponent<TriggerBoxComponent>(
			TriggerBoxComponent{ .triggerType = trigType,
								 .transportOffset = secretTrigger.transportOffset,
								 .cameraOffset = secretTrigger.cameraOffset } );

		if ( trigger.HasComponent<SceneChangeComponent>() )
		{
			const auto& scene = trigger.GetComponent<SceneChangeComponent>();
			secretArea.AddComponent<SceneChangeComponent>( SceneChangeComponent{ .levelMusic = scene.levelMusic,
																				 .assetFile = scene.assetFile,
																				 .enemyFile = scene.enemyFile,
																				 .colliderFile = scene.colliderFile,
																				 .tileMapName = scene.tileMapName,
																				 .tileImageName = scene.tileImageName,
																				 .mapImageName = scene.mapImageName,
																				 .entityFileName = scene.entityFileName,
																				 .triggerFile = scene.triggerFile,
																				 .imageHeight = scene.imageHeight,
																				 .imageWidth = scene.imageWidth } );
		}

		secretArea.AddComponent<TransformComponent>(
			secretTransform.position, secretTransform.scale, secretTransform.rotation );
		secretArea.AddComponent<SpriteComponent>( SpriteComponent{
			.assetID = secret.newSpriteAssetID,
			.width = secret.spriteWidth,
			.height = secret.spriteHeight,
			.layer = 1,
			.bIsFixed = false,
			.srcRect = SDL_Rect{ secret.spriteSrcX, secret.spriteSrcY, secret.spriteWidth, secret.spriteHeight } } );

		secretArea.AddComponent<GameComponent>();
	}

	trigger.Kill();
}

void TriggerSystem::ChangeScene( Entity& player, Entity& trigger )
{
	const auto& player1 = m_Game.GetPlayer();

	if ( player1->GetStairsFinished() || m_Game.GetCamera().CurtainClosed() )
	{
		const auto& playerEnt = player1->GetPlayer();
		auto& playerTransform = playerEnt.GetComponent<TransformComponent>();
		auto& rigidbody = playerEnt.GetComponent<RigidBodyComponent>();
		auto& sprite = playerEnt.GetComponent<SpriteComponent>();
		auto& scene = trigger.GetComponent<SceneChangeComponent>();
		auto& camera = m_Game.GetCamera();

		// Stop the player movement during the scene transition
		rigidbody.velocity = glm::vec2( 0 );
		// Get needed componenets from the triger
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		float x = trig.transportOffset.x;
		float y = trig.transportOffset.y;

		if ( trig.collectedTimer.GetTicks() == 0 )
		{
			trig.collectedTimer.Start();
		}

		// If the Scene has not faded, start the fade transition
		if ( camera.GetFadeAlpha() > 0 && !camera.CurtainClosed() )
		{
			if ( !camera.FadeOutStarted() )
				camera.StartFadeOut( true );
		}
		else
		{
			// Set player Position
			playerTransform.position.x = x;
			playerTransform.position.y = y;

			// Remove all the prior assets/entities from the current scene
			m_Registry.GetSystem<RenderSystem>().OnExit();
			m_Registry.GetSystem<RenderTileSystem>().OnExit();
			m_Registry.GetSystem<RenderCollisionSystem>().OnExit();
			m_Registry.GetSystem<RenderTextSystem>().OnExit();

			// Check to see if the trigger has "no_file" assiged if it has a file load the assets for the scene
			if ( scene.assetFile != "no_file" )
				m_Loader.LoadAssetsFromLuaTable( m_Game.GetLuaState(), scene.assetFile );

			// load the tilemap only if there is an image and a corresponding map
			if ( scene.tileMapName != "no_file" && scene.mapImageName != "no_file" )
			{
				std::string mapFile = "assets/Tilemaps/Maps/" + scene.tileMapName + ".map";
				m_Loader.LoadTilemap( mapFile, scene.mapImageName );
			}

			// If there is only an image name than it is a full map image, not tiles --> Just load the map
			if ( scene.tileImageName != "no_file" )
			{
				m_Loader.LoadMap( scene.tileImageName, scene.imageWidth, scene.imageHeight );
			}

			// Start the new scene's music || stop the music
			if ( scene.levelMusic != "stop" )
				m_Game.GetMusicPlayer().PlayMusic( scene.levelMusic, -1 );
			else
				m_Game.GetMusicPlayer().StopMusic();

			// If there is an entity file, NPCs, Store Items, etc. Load it
			if ( scene.entityFileName != "no_file" )
				m_Loader.LoadEntitiesFromLuaTable( m_Game.GetLuaState(), scene.entityFileName );

			// Adjust the camera location to the trigger offset
			camera.SetCameraPosition( trig.cameraOffset.x, trig.cameraOffset.y );
			// camera.SetCameraPosFromSceneChange(trig.cameraOffset.x, trig.cameraOffset.y);
			camera.SetPrevCamPosition( trig.cameraOffset.x, trig.cameraOffset.y );

			// Check to see if there is a trigger file
			if ( scene.triggerFile != "no_file" )
				m_Loader.LoadTriggers( m_Game.GetLuaState(), scene.triggerFile );

			// Check to see if there is an enemy file
			if ( scene.enemyFile != "no_file" )
				m_Loader.LoadEnemiesFromLuaTable( m_Game.GetLuaState(), scene.enemyFile );

			// Load the secrets of the scene that have been bFound
			m_Loader.ReadInSecrets( m_Game.GetLuaState() );

			// Check to see if there is a collider file
			if ( scene.colliderFile != "no_file" )
				m_Loader.LoadColliders( scene.colliderFile );

			// Finish the fade screen
			if ( !camera.CurtainClosed() )
			{
				camera.StartFadeOut( false );
				camera.StartFadeIn( true );
			}

			// Set player sprite back to start
			sprite.srcRect.y = 0;

			if ( camera.CurtainClosed() )
				camera.StartCurtainOpen();
			else
				m_Game.GetPlayer()->SetStairsFinished( false );
		}
	}
	else
	{
		m_Game.GetPlayer()->SetPlayerOnStairs( true );
	}
}

void TriggerSystem::PushRocks( Entity& player, Entity& trigger )
{
	auto& playerRigidBody = player.GetComponent<RigidBodyComponent>();
	auto& rockTransform = trigger.GetComponent<TransformComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();

	if ( !trig.bActive && !secret.bFound )
	{
		if ( playerRigidBody.dir == RigidBodyDir::UP )
		{
			trig.bActive = true;
			m_Game.GetSoundPlayer().PlaySoundFX( "secret", 0, SoundChannel::ANY );
			secret.bMoveUp = true;
		}
		else if ( playerRigidBody.dir == RigidBodyDir::DOWN )
		{
			trig.bActive = true;
			m_Game.GetSoundPlayer().PlaySoundFX( "secret", 0, SoundChannel::ANY );
			secret.bMoveDown = true;
		}
	}
}

void TriggerSystem::RideRaft( Entity& player, Entity& trigger )
{
	auto& trigs = trigger.GetComponent<TriggerBoxComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	const auto& pTran = player.GetComponent<TransformComponent>();

	if ( m_GameData.HasItem( GameData::GameItems::RAFT ) )
	{
		if ( ( secret.bMoveDown && playerRigidbody.dir == RigidBodyDir::DOWN ) ||
			 ( secret.bMoveUp && playerRigidbody.dir == RigidBodyDir::UP ) )
		{
			if ( !trigs.bActive )
			{
				StopPlayerMovement( player, trigger );
				m_Game.GetSoundPlayer().PlaySoundFX( "secret", 0, SoundChannel::ANY );
				auto raft = m_Registry.CreateEntity();
				raft.Tag( "raft" );
				raft.AddComponent<TransformComponent>( pTran );
				raft.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
																	 .width = 16,
																	 .height = 16,
																	 .layer = 1,
																	 .bIsFixed = false,
																	 .srcRect = SDL_Rect{ 0, 16, 16, 16 } } );

				raft.AddComponent<GameComponent>();
				raft.GetComponent<TransformComponent>().position.x += 32;
				raft.GetComponent<TransformComponent>().position.y += 32;
			}

			trigs.bActive = true;
			m_Game.GetPlayer()->SetOnRaft( true );
		}
	}
	else
	{
		StopPlayerMovement( player, trigger );
	}
}

void TriggerSystem::Transport( Entity& player, Entity& trigger )
{
	if ( m_Game.GetCamera().GetFadeAlpha() > 0 )
	{
		m_Game.GetCamera().StartFadeOut( true );
		return;
	}

	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	playerTransform.position = trig.transportOffset;
	m_Game.GetCamera().SetCameraPosition( trig.cameraOffset.x, trig.cameraOffset.y );

	m_Game.GetSoundPlayer().PlaySoundFX( "stairs", 0, SoundChannel::ANY );

	// Finish the fade screen
	m_Game.GetCamera().StartFadeOut( false );
	m_Game.GetCamera().StartFadeIn( true );
}

void TriggerSystem::UseLadder( Entity& player, Entity& trigger )
{
	if ( !m_GameData.HasItem( GameData::GameItems::LADDER ) )
	{
		StopPlayerMovement( player, trigger );
		return;
	}

	auto& trigs = trigger.GetComponent<TriggerBoxComponent>();
	auto& trigTransform = trigger.GetComponent<TransformComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();

	if ( !trigs.bActive )
	{
		auto xOffset = 64;
		auto yOffset = 64;
		switch ( playerRigidbody.dir )
		{
		case RigidBodyDir::UP:
			xOffset = 32;
			yOffset = -64;
			break;
		case RigidBodyDir::RIGHT:
			xOffset = 64;
			yOffset = 32;
			break;
		case RigidBodyDir::DOWN:
			xOffset = 32;
			yOffset = 64;
			break;
		case RigidBodyDir::LEFT:
			xOffset = -16;
			yOffset = 32;
			break;
		}

		// Create a temporary ladder entity
		auto ladder = m_Registry.CreateEntity();
		ladder.Tag( "ladder" );
		ladder.AddComponent<TransformComponent>( playerTransform );
		ladder.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															   .width = 16,
															   .height = 16,
															   .layer = 1,
															   .bIsFixed = false,
															   .srcRect = SDL_Rect{ 112, 48, 16, 16 } } );

		ladder.AddComponent<GameComponent>();
		ladder.GetComponent<TransformComponent>().position.x += xOffset;
		ladder.GetComponent<TransformComponent>().position.y += yOffset;
		trigs.bActive = true;
	}
}

void TriggerSystem::BombSecret( Entity& trigger )
{
	if ( trigger.HasComponent<SecretComponent>() )
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		trig.bActive = true;
		SecretTrigger( trigger );

		// Check to see if there is a Trap door or something else to remove
		const auto& removeTag = trig.entityRemoveTag;
		if ( removeTag != "" )
		{
			if ( m_Registry.DoesTagExist( removeTag ) )
			{
				auto removedEntity = m_Registry.GetEntityByTag( removeTag );

				SecretTrigger( removedEntity );
				// Remove the entity
				removedEntity.Kill();
			}
		}

		// After the new trigger is created, remove the current trigger!
		trigger.Kill();
	}
}

void TriggerSystem::UnlockDoor( Entity& trigger )
{
	// Unlock the door if keys are available
	if ( m_GameData.GetTotalKeys() > 0 )
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		trig.bActive = true;

		// Play door open sound
		m_Game.GetSoundPlayer().PlaySoundFX( "door_unlock", 0, SoundChannel::ANY );
		// Decrement total number of keys
		m_GameData.UseKey();

		// Check to see if there is a sister door to remove
		const auto& removeTag = trig.entityRemoveTag;
		if ( removeTag != "" )
		{
			auto removedEntity = m_Registry.GetEntityByTag( removeTag );

			// Remove/Kill the sister door
			removedEntity.Kill();
		}

		// Remove the keyed door
		trigger.Kill();
	}
}

void TriggerSystem::BuyShopItem( Entity& trigger )
{
	// Check to see if the trigger has an Item Components
	if ( !trigger.HasComponent<ItemComponent>() || m_GameData.RupeeScrollActive() )
		return;

	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	auto& shopItem = trigger.GetComponent<ItemComponent>();

	// Check to see if the player has enough rupees
	if ( m_GameData.GetTotalRupees() < shopItem.price )
		return;

	// Check to see if key item and if it is already in the inventory
	if ( shopItem.special != SpecialItemType::NOT_SPECIAL )
	{
		// Check to see if the item is in the inventory
		if ( CheckInventory( shopItem.special ) )
			return;
	}

	// Collect the item
	if ( !m_GameData.BuyItem( shopItem.price ) )
		return;

	m_Game.GetSoundPlayer().PlaySoundFX( "fanfare", 0, SoundChannel::ANY );
	m_Game.GetPlayer()->SetPlayerItem( true );
	trig.bActive = true;
}

void TriggerSystem::EnterFairyCircle( Entity& player, Entity& trigger )
{
	auto& playerHealth = player.GetComponent<HealthComponent>();
	// If the player's health is already at max, no need to start the fairy circle trigger
	if ( playerHealth.healthPercentage != playerHealth.maxHearts * 2 )
	{
		m_Game.GetPlayer()->HoldPlayer();
		for ( const auto& fairy : m_Registry.GetEntitiesByGroup( "fairy" ) )
		{
			// Get the fairy transform. It will be used to check against player position and
			// to position the circling hearts
			auto& fairyTransform = fairy.GetComponent<TransformComponent>();
			const int checkX = fairyTransform.position.x / PANEL_WIDTH;
			const int checkY = fairyTransform.position.y / PANEL_HEIGHT;

			// Check to see if the fairy is in the same panel as the player
			if ( checkX != m_Game.GetPlayer()->GetPlayerPos().x || checkY != m_Game.GetPlayer()->GetPlayerPos().y )
				continue;

			auto& fairyTrig = trigger.GetComponent<TriggerBoxComponent>();
			// If the trigger is already bActive, the hearts would have already been created
			if ( fairyTrig.bActive )
				continue;

			// Set the fairy trigger to bActive
			fairyTrig.bActive = true;

			// There are Eight Hearts that Circle the fairy
			for ( int i = 0; i < 8; i++ )
			{
				// Create a new Heart entity to circle the fairy
				auto fairyHeart = m_Registry.CreateEntity();
				;
				fairyHeart.Group( "fairy_hearts" );
				fairyHeart.AddComponent<TransformComponent>( fairyTransform.position, fairyTransform.scale );
				fairyHeart.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hearts",
																		   .width = 16,
																		   .height = 16,
																		   .layer = 2,
																		   .bIsFixed = false,
																		   .srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

				fairyHeart.AddComponent<GameComponent>();
			}
		}
	}
	else
	{
		m_Game.GetPlayer()->ReleasePlayer();
	}
}

void TriggerSystem::UpdateFairyCircle( Entity& player, Entity& trigger )
{
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	auto& playerHealth = player.GetComponent<HealthComponent>();

	// Temp variable
	static int x = 0;

	// Check to see if the player has full health, if so, leave the loop
	if ( playerHealth.healthPercentage != playerHealth.maxHearts * 2 )
	{
		if ( !trig.collectedTimer.isStarted() )
			trig.collectedTimer.Start();

		// Increment the player health after a certain interval
		if ( trig.collectedTimer.GetTicks() > 1000 * x )
		{
			playerHealth.healthPercentage += 1;
			x++;
		}

		// Play the refill sound
		// if (Mix_Playing(6) == 0)
		m_Game.GetSoundPlayer().PlaySoundFX( "refill_hearts", 0, SoundChannel::ANY );
	}
	else
	{
		// Reset the trigger
		trig.bActive = false;
		// Reset the trigger timer
		trig.collectedTimer.Stop();
		// Remove All the created hearts
		for ( auto& heart : m_Registry.GetEntitiesByGroup( "fairy_hearts" ) )
		{
			heart.Kill();
		}

		x = 0;
		// Leave the loop
		return;
	}

	// Increment the angle for the circle motion calculation
	m_Angle += AngleIncrease;

	for ( const auto& fairy : m_Registry.GetEntitiesByGroup( "fairy" ) )
	{
		// Get the fairy transform. It will be used to check against player position and
		// to position the circling hearts
		auto& fairyTransform = fairy.GetComponent<TransformComponent>();
		const int checkX = fairyTransform.position.x / PANEL_WIDTH;
		const int checkY = fairyTransform.position.y / PANEL_HEIGHT;

		// Check to see if the fairy is in the same panel as the player
		if ( checkX != m_Game.GetPlayer()->GetPlayerPos().x || checkY != m_Game.GetPlayer()->GetPlayerPos().y )
			continue;

		// Loop through the 8 Circling hearts and change their position around the fairy,
		// The fairy's position is the center of the circle
		for ( auto& heart : m_Registry.GetEntitiesByGroup( "fairy_hearts" ) )
		{
			auto& heartsTransform = heart.GetComponent<TransformComponent>();
			// Move the heart in a circular motion
			heartsTransform.position =
				glm::vec2( ( fairyTransform.position.x ) + ( cos( m_Angle + m_HeartOffset ) * 288 ),
						   ( fairyTransform.position.y ) + ( sin( m_Angle + m_HeartOffset ) * 288 ) );

			// This is for offsetting each heart
			m_HeartOffset += HeartOffset;
		}
		// Reset the heart offset back to zero so we can restart we we come back into the for loop
		m_HeartOffset = 0.f;
	}
}

void TriggerSystem::UpdateRaft( Entity& player, Entity& trigger )
{
	auto raft = m_Registry.GetEntityByTag( "raft" );
	auto& raftTransform = raft.GetComponent<TransformComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();

	if ( secret.bMoveUp && playerRigidbody.dir == RigidBodyDir::UP )
	{
		playerTransform.position.y -= 200 * m_Game.GetDeltaTime();
		raftTransform.position.y -= 200 * m_Game.GetDeltaTime();
		if ( playerTransform.position.y <= trig.transportOffset.y )
		{
			trig.bActive = false;
			m_Game.GetPlayer()->SetOnRaft( false );
			raft.Kill();
		}
	}
	else if ( secret.bMoveDown && playerRigidbody.dir == RigidBodyDir::DOWN )
	{
		playerTransform.position.y += 200 * m_Game.GetDeltaTime();
		raftTransform.position.y += 200 * m_Game.GetDeltaTime();

		if ( playerTransform.position.y >= trig.transportOffset.y )
		{
			trig.bActive = false;
			m_Game.GetPlayer()->SetOnRaft( false );
			raft.Kill();
		}
	}
}

void TriggerSystem::UpdateMovingRocks( Entity& player, Entity& trigger )
{
	auto& transform = trigger.GetComponent<TransformComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	auto& sprite = trigger.GetComponent<SpriteComponent>();

	// Move the Block/Rock Up until it reaches its max position
	if ( transform.position.y > secret.startPos.y - sprite.width * m_Game.GetGameScale() && secret.bMoveUp )
	{
		transform.position.y--;
	}
	// Move the Block/Rock Down until it reaches its max position
	else if ( transform.position.y < secret.startPos.y + sprite.width * m_Game.GetGameScale() && secret.bMoveDown )
	{
		transform.position.y++;
	}
	else
	{
		secret.bFound = true;

		// Check to see if there is a Trap door or something else to remove
		const auto& removeTag = trig.entityRemoveTag;
		if ( removeTag != "" )
		{
			if ( m_Registry.DoesTagExist( removeTag ) )
			{
				auto removedEntity = m_Registry.GetEntityByTag( removeTag );

				if ( removedEntity.HasComponent<TriggerBoxComponent>() )
				{
					const auto& triggerType = removedEntity.GetComponent<TriggerBoxComponent>().triggerType;
					if ( triggerType == TriggerType::TRAP_DOOR )
					{
						m_Game.GetSoundPlayer().PlaySoundFX( "door_unlock", 0, SoundChannel::ANY );
					}
				}
				// Remove the entity
				removedEntity.Kill();
			}
		}
	}
}

void TriggerSystem::UpdateLadder( Entity& player, Entity& trigger )
{
	auto& transform = trigger.GetComponent<TransformComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	auto& sprite = trigger.GetComponent<SpriteComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();

	if ( playerTransform.position.x < transform.position.x - 64 ||
		 playerTransform.position.x > transform.position.x + 32 )
	{
		auto ladder = m_Registry.GetEntityByTag( "ladder" );
		trig.bActive = false;
		ladder.Kill();
	}
}

TriggerSystem::TriggerSystem()
	: m_Game{ Game::Instance() }
	, m_GameData{ GameData::GetInstance() }
	, m_Registry{ Registry::Instance() }
	, m_Loader{}
	, m_Angle{ 0.f }
	, m_HeartOffset{ 0.f }
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<BoxColliderComponent>();
	RequiredComponent<TriggerBoxComponent>();
}

void TriggerSystem::SubscribeToEvents( EventManager& eventManager )
{
	eventManager.SubscribeToEvent<CollisionEvent>( this, &TriggerSystem::OnTrigger );
}

void TriggerSystem::OnTrigger( CollisionEvent& event )
{
	Entity& a = event.a;
	Entity& b = event.b;

	// Check to see if the player has activated a trigger
	if ( !a.HasComponent<SecretComponent>() && a.HasComponent<TriggerBoxComponent>() && b.HasTag( "player" ) )
	{
		OnEnterTrigger( b, a );
		return;
	}
	else if ( !b.HasComponent<SecretComponent>() && b.HasComponent<TriggerBoxComponent>() && a.HasTag( "player" ) )
	{
		OnEnterTrigger( a, b );
		return;
	}

	// Check for secret Component
	if ( b.HasComponent<SecretComponent>() )
	{
		auto& trig = b.GetComponent<TriggerBoxComponent>();
		if ( trig.triggerType == TriggerType::BOMB_SECRET && a.BelongsToGroup( "explode" ) )
		{
			OnEnterTrigger( a, b );
			return;
		}

		else if ( trig.triggerType == TriggerType::BURN_BUSHES && a.BelongsToGroup( "fire" ) )
		{
			OnEnterTrigger( a, b );
			return;
		}

		else if ( trig.triggerType == TriggerType::PUSH_ROCKS && a.HasTag( "player" ) )
		{
			OnEnterTrigger( a, b );
			return;
		}
		else if ( trig.triggerType == TriggerType::RAFT && a.HasTag( "player" ) )
		{
			OnEnterTrigger( a, b );
			return;
		}
	}
	else if ( a.HasComponent<SecretComponent>() )
	{
		auto& trig = a.GetComponent<TriggerBoxComponent>();
		if ( trig.triggerType == TriggerType::BOMB_SECRET && b.BelongsToGroup( "explode" ) )
		{
			OnEnterTrigger( b, a );
			return;
		}
		else if ( trig.triggerType == TriggerType::BURN_BUSHES && b.BelongsToGroup( "fire" ) )
		{
			OnEnterTrigger( b, a );
			return;
		}
		else if ( trig.triggerType == TriggerType::PUSH_ROCKS && b.HasTag( "player" ) )
		{
			OnEnterTrigger( b, a );
			return;
		}

		else if ( trig.triggerType == TriggerType::RAFT && b.HasTag( "player" ) )
		{
			OnEnterTrigger( b, a );
			return;
		}
	}
}

void TriggerSystem::OnEnterTrigger( Entity& player, Entity& trigger )
{
	// Get needed componenets from the triger
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	if ( trig.bActive )
		return;

	switch ( trig.triggerType )
	{
	case TriggerType::NO_TRIGGER:
		Logger::Err( "TRIGGER_SYSTEM: __LINE: 174 - There is not trigger set for this entity" );
		break;
	case TriggerType::SCENE_CHANGE: ChangeScene( player, trigger ); break;
	case TriggerType::TRANSPORT: Transport( player, trigger ); break;
	case TriggerType::BURN_BUSHES:

		if ( trigger.HasComponent<SecretComponent>() )
		{
			SecretTrigger( trigger );
			// After the new trigger is created, remove the current trigger!
			trigger.Kill();
		}
		break;
	case TriggerType::PUSH_ROCKS: PushRocks( player, trigger ); break;
	case TriggerType::COLLECT_ITEM: {
		auto& item = trigger.GetComponent<ItemComponent>();
		std::string soundID = "fanfare";
		if ( item.special == SpecialItemType::TRIFORCE_PIECE )
		{
			m_Game.GetPlayer()->SetObtainedTriforce( true );
			soundID = "triforce_fanfare";
			m_Game.GetMusicPlayer().PlayMusic( soundID, 0 );
		}
		else
		{
			m_Game.GetSoundPlayer().PlaySoundFX( soundID, 0, SoundChannel::ANY );
		}

		m_Game.GetPlayer()->SetPlayerItem( true );
		trig.bActive = true;

		break;
	}
	case TriggerType::BOMB_SECRET: BombSecret( trigger ); break;
	case TriggerType::LOCKED_DOOR: UnlockDoor( trigger ); break;
	case TriggerType::SHOP_ITEM: BuyShopItem( trigger ); break;
	case TriggerType::RAFT: RideRaft( player, trigger ); break;
	case TriggerType::LADDER: UseLadder( player, trigger ); break;
	case TriggerType::TRAP_DOOR:
		// TODO:
		break;
	case TriggerType::FAIRY_CIRCLE: EnterFairyCircle( player, trigger ); break;
	default: break;
	}
}

void TriggerSystem::Update( const float& dt )
{
	for ( auto& entity : GetSystemEntities() )
	{
		auto& trig = entity.GetComponent<TriggerBoxComponent>();

		if ( !trig.bActive )
			continue;

		// Get the player
		const auto& player = m_Game.GetPlayer();
		auto& playerEnt = player->GetPlayer();

		if ( entity.HasComponent<SecretComponent>() )
		{
			auto& secret = entity.GetComponent<SecretComponent>();
			// If the secret has already been bFound, nothing to do
			if ( secret.bFound )
				continue;

			switch ( trig.triggerType )
			{
			case TriggerType::PUSH_ROCKS: UpdateMovingRocks( playerEnt, entity ); break;
			case TriggerType::RAFT: UpdateRaft( playerEnt, entity ); break;
			case TriggerType::LADDER: UpdateLadder( playerEnt, entity ); break;
			}
		}
		else if ( trig.triggerType == TriggerType::FAIRY_CIRCLE )
		{
			UpdateFairyCircle( playerEnt, entity );
		}
	}
}
