#include "Systems/GameSystems/ProjectileEmitterSystem.h"
#include "Events/EventManager.h"
#include "Events/GamePadButtonPressedEvent.h"
#include "Components/HealthComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/GameComponent.h"
#include "Components/AIComponent.h"
#include "Systems/GameSystems/ProjectileLifeCycleSystem.h"
#include "States/GameState.h"
#include "Systems/SoundFXSystem.h"
#include "Utilities/GameData.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "inputs/InputManager.h"
#include "inputs/Gamepad.h"
#include <map>

#include <SDL.h>

#ifdef _DEBUG

void ProjectileEmitterSystem::ClearItemMap()
{
	if ( m_mapProjectileAttribs.size() > 0 )
		m_mapProjectileAttribs.clear();

	if ( m_mapProjectileAttribs.size() == 0 )
		Logger::Log( "Item Map Cleared" );
}

void ProjectileEmitterSystem::ReloadItemMap()
{
	if ( m_mapProjectileAttribs.size() == 0 )
	{
		LoadMapAttributes( Game::Instance().GetLuaState() );
		Logger::Log( "Item Map Reloaded!" );
	}
	else
		Logger::Err( "Please Empty the item Map" );
}

#endif

void ProjectileEmitterSystem::ItemUsed()
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();

	auto& rigid = playerEnt.GetComponent<RigidBodyComponent>();

	rigid.velocity = glm::vec2( 0 );
	switch ( m_GameData.GetSelectedItem() )
	{
	case GameData::ItemType::BOW:
		// Bow uses rupees, must be greater than zero!
		if ( m_GameData.GetTotalRupees() > 0 )
		{
			UseItem( m_mapProjectileAttribs.at( "bow" ) );

			// Play the arrow sound!
			m_Game.GetSoundPlayer().PlaySoundFX( "boomerang_arrow", 0, SoundChannel::ANY );
			// Subtract from the total rupees when using the bow!
			m_GameData.UseArrow();
		}
		break;
	case GameData::ItemType::BOMB:
		if ( m_GameData.GetTotalBombs() > 0 )
		{
			UseItem( m_mapProjectileAttribs.at( "bomb" ) );
			m_GameData.UseBomb();
			m_Game.GetSoundPlayer().PlaySoundFX( "bomb_drop", 0, SoundChannel::ANY );
		}
		break;
	case GameData::ItemType::CANDLE:
		UseItem( m_mapProjectileAttribs.at( "candle" ) );
		m_Game.GetSoundPlayer().PlaySoundFX( "candle", 0, SoundChannel::ANY );
		break;
	case GameData::ItemType::BOOMERANG:
		if ( Registry::Instance().DoesTagExist( "boomerang" ) )
			return;

		UseItem( m_mapProjectileAttribs.at( "boomerang" ) );
		m_Game.GetSoundPlayer().PlaySoundFX( "boomerang_arrow", 0, SoundChannel::ANY );
		break;
	case GameData::ItemType::MAGIC_ROD:
		UseMagicWand();
		UseItem( m_mapProjectileAttribs.at( "magic_rod" ) );
		m_Game.GetSoundPlayer().PlaySoundFX( "magic_rod", 0, SoundChannel::ANY );
		break;
	}
}

void ProjectileEmitterSystem::SwordUsed()
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& rigid = playerEnt.GetComponent<RigidBodyComponent>();

	rigid.velocity = glm::vec2( 0 );
	// Do not use the sword if we do not have a sword
	if ( m_GameData.HasSword() )
	{
		UseSword();
		// If the player life is full, allow sword beam projectile
		if ( m_bFullLife )
		{
			// Create Sword beam projectile
			UseItem( m_mapProjectileAttribs.at( "beam" ) );

			m_Game.GetSoundPlayer().PlaySoundFX( "sword_shoot", 0, SoundChannel::ANY );
		}
		else
			m_Game.GetSoundPlayer().PlaySoundFX( "sword_slash", 0, SoundChannel::ANY );

		m_SwordTimer.Start();
	}
}

ProjectileEmitterSystem::ProjectileEmitterSystem()
	: m_Game( Game::Instance() )
	, m_GameData( GameData::GetInstance() )
	, m_InputManager( InputManager::GetInstance() )
	, m_Registry( Registry::Instance() )
	, m_bFullLife{ false }
	, m_SwordTimer{}
{
	RequiredComponent<ProjectileEmitterComponent>();
	RequiredComponent<TransformComponent>();
	LoadMapAttributes( m_Game.GetLuaState(), "ProjectileAttributes" );
	m_SwordTimer.Stop();
}

void ProjectileEmitterSystem::SubscribeKeyToEvents( EventManager& eventManager )
{
	eventManager.SubscribeToEvent<KeyPressedEvent>( this, &ProjectileEmitterSystem::OnKeyPressed );
}
void ProjectileEmitterSystem::UseItem( ProjectileAttrib attrib )
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& projectileEmitter = playerEnt.GetComponent<ProjectileEmitterComponent>();
	const auto& transform = playerEnt.GetComponent<TransformComponent>();
	const auto& rigidbody = playerEnt.GetComponent<RigidBodyComponent>();
	const auto& sprite = playerEnt.GetComponent<SpriteComponent>();

	// If the entity has a sprite --> Position the projectile in the center
	glm::vec2 projectilePosition = transform.position;
	projectilePosition.x += ( transform.scale.x * sprite.width / 2 );
	projectilePosition.y += ( transform.scale.y * sprite.height / 2 );

	glm::vec2 projectileVelocity{ projectileEmitter.projectileVelocity };
	glm::vec2 BoxOffset = glm::vec2( 0, 0 );
	glm::vec2 BoxSize = glm::vec2( 0, 0 );

	// Create direction variables the change the velocity direction based on which way we are facing
	int directionX = 0;
	int directionY = 0;

	// Use the fire ahead of the player in the direction the player is facing
	switch ( rigidbody.dir )
	{
	case RigidBodyDir::UP:
		if ( attrib.numFrames == 1 )
		{
			projectilePosition.x = ( transform.position.x + attrib.transOffsetUp.x );
			projectilePosition.y = ( transform.position.y + attrib.transOffsetUp.x ) - sprite.height * 3;
		}
		else
		{
			projectilePosition.x = ( transform.position.x + attrib.transOffsetUp.x );
			projectilePosition.y = ( transform.position.y + attrib.transOffsetUp.x ) - sprite.height * 3;
			// attrib.srcRectX  += attrib.width * 1;
		}
		BoxSize = attrib.boxSizeUp;
		BoxOffset = attrib.upOffset;
		directionY = -1;
		break;
	case RigidBodyDir::RIGHT:
		if ( attrib.numFrames == 1 )
		{
			projectilePosition.x = ( ( transform.position.x + attrib.transOffsetRight.x ) + sprite.width * 3 );
			projectilePosition.y = ( transform.position.y + attrib.transOffsetRight.y );
		}
		else
		{
			projectilePosition.x = ( ( transform.position.x + attrib.transOffsetRight.x ) + sprite.width * 3 );
			projectilePosition.y = ( transform.position.y + attrib.transOffsetRight.y );
			attrib.srcRectX += attrib.width * 1;
		}
		BoxSize = attrib.boxSizeRight;
		BoxOffset = attrib.rightOffset;
		directionX = 1;
		break;
	case RigidBodyDir::DOWN:
		if ( attrib.numFrames == 1 )
		{
			projectilePosition.x = ( transform.position.x + attrib.transOffsetDown.x );
			projectilePosition.y = ( ( transform.position.y + attrib.transOffsetDown.y ) + sprite.height * 3 );
		}
		else
		{
			projectilePosition.x = ( transform.position.x + attrib.transOffsetDown.x );
			projectilePosition.y = ( ( transform.position.y + attrib.transOffsetDown.y ) + sprite.height * 3 );
			attrib.srcRectX += attrib.width * 2;
		}
		BoxSize = attrib.boxSizeDown;
		BoxOffset = attrib.downOffset;
		directionY = 1;
		break;
	case RigidBodyDir::LEFT:
		if ( attrib.numFrames == 1 )
		{
			projectilePosition.x = ( ( transform.position.x + attrib.transOffsetLeft.x ) - sprite.width * 3 );
			projectilePosition.y = ( transform.position.y + attrib.transOffsetLeft.y );
		}
		else
		{
			projectilePosition.x = ( ( transform.position.x + attrib.transOffsetLeft.x ) - sprite.width * 3 );
			projectilePosition.y = ( transform.position.y + attrib.transOffsetLeft.y );
			attrib.srcRectX += attrib.width * 3;
		}
		BoxSize = attrib.boxSizeLeft;
		BoxOffset = attrib.leftOffset;
		directionX = -1;
		break;
	}

	// Create new projectile entity and add it to the world
	Entity newItem = m_Registry.CreateEntity();
	newItem.AddComponent<TransformComponent>( projectilePosition, glm::vec2( attrib.scale.x, attrib.scale.y ), 0.0 );

	if ( attrib.group != "bomber" )
	{
		// Set the projectile velocity based on the direction
		projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
		projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
		newItem.AddComponent<RigidBodyComponent>( projectileVelocity, rigidbody.dir );
		newItem.AddComponent<BoxColliderComponent>( (int)BoxSize.x / transform.scale.x,
													(int)BoxSize.y / transform.scale.y,
													glm::vec2( BoxOffset.x, BoxOffset.y ) );
	}
	// Does the projectile have an animation component?
	if ( attrib.bAnimation )
	{
		// TODO: Change the frame speed from hard coded to individual --> attrib.frame_speed
		newItem.AddComponent<AnimationComponent>( AnimationComponent{
			.numFrames = attrib.numFrames, .frameSpeedRate = 15, .vertical = attrib.bVertical, .isLooped = true } );
	}
	projectileEmitter.bIsFriendly = true;
	newItem.AddComponent<SpriteComponent>(
		SpriteComponent{ .assetID = attrib.sprite_name,
						 .width = attrib.width,
						 .height = attrib.height,
						 .layer = 2,
						 .bIsFixed = false,
						 .srcRect = SDL_Rect{ attrib.srcRectX, attrib.srcRectY, attrib.width, attrib.height } } );

	newItem.AddComponent<ProjectileComponent>(
		ProjectileComponent{ .isFriendly = projectileEmitter.bIsFriendly,
							 .hitPercentDamage = 10,
							 .duration = attrib.duration,
							 .dir = static_cast<ProjectileDir>( rigidbody.dir ) } );

	newItem.AddComponent<GameComponent>();

	if ( attrib.group == "boomerang" )
	{
		auto& boomerTimer = newItem.GetComponent<ProjectileComponent>();
		boomerTimer.boomTimer.Start();
		newItem.Tag( attrib.group );
	}
	else if ( attrib.group == "beam" )
	{
		newItem.Group( "beam" );
		newItem.Group( "projectile" );
	}
	else
	{
		newItem.Group( attrib.group );
	}
}

void ProjectileEmitterSystem::UseSword()
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();

	auto& playerSprite = playerEnt.GetComponent<SpriteComponent>();
	auto& playerCollider = playerEnt.GetComponent<BoxColliderComponent>();
	const auto& playerHealth = playerEnt.GetComponent<HealthComponent>();
	const auto& rigidbody = playerEnt.GetComponent<RigidBodyComponent>();

	const auto& sword = player->GetSword();
	auto& swordCollider = sword.GetComponent<BoxColliderComponent>();

	const auto& shield = player->GetShield();
	auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();

	switch ( rigidbody.dir )
	{
	case RigidBodyDir::UP:
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.y = 60;

		swordCollider.height = 60;
		swordCollider.width = 20;
		swordCollider.offset.x = 60;
		swordCollider.offset.y = 12;

		shieldCollider.offset.y = 60;
		break;
	case RigidBodyDir::RIGHT:
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.x = 30;

		swordCollider.height = 20;
		swordCollider.width = 60;
		swordCollider.offset.x = 78;
		swordCollider.offset.y = 58;

		shieldCollider.offset.x = 70;
		break;
	case RigidBodyDir::DOWN:
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.y = 30;

		swordCollider.height = 60;
		swordCollider.width = 20;
		swordCollider.offset.x = 64;
		swordCollider.offset.y = 74;

		shieldCollider.offset.y = 60;
		break;
	case RigidBodyDir::LEFT:
		playerSprite.srcRect.y = playerSprite.height * 5;
		playerCollider.offset.x = 60;

		swordCollider.height = 20;
		swordCollider.width = 60;
		swordCollider.offset.x = 12;
		swordCollider.offset.y = 56;

		shieldCollider.offset.x = 64;
		break;
	}

	// Check to see if health is full for sword beam
	if ( playerHealth.healthPercentage == ( playerHealth.maxHearts * 2 ) )
		m_bFullLife = true;
	else
		m_bFullLife = false;
}

void ProjectileEmitterSystem::UseMagicWand()
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& sprite = playerEnt.GetComponent<SpriteComponent>();
	sprite.srcRect.y = sprite.height * 6;
}

void ProjectileEmitterSystem::LoadMapAttributes( sol::state& lua, const std::string& fileName )
{
	sol::load_result script = lua.load_file( "./assets/LuaFiles/" + fileName + ".lua" );
	// This checks the syntax of our script, but it does not execute the script
	if ( !script.valid() )
	{
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err( "Error loading the Lua Script: " + errorMessage );
		return;
	}

	// Executes the script using the sol State
	lua.script_file( "./assets/LuaFiles/" + fileName + ".lua" );

	sol::table atrib = lua[ "items" ];

	int i = 1;

	while ( true )
	{
		sol::optional<sol::table> hasAttribs = atrib[ i ];

		if ( hasAttribs == sol::nullopt )
			break;

		if ( hasAttribs != sol::nullopt )
		{
			sol::table item_attrib = atrib[ i ];

			ProjectileAttrib newItem;
			std::string name = item_attrib[ "name" ];

			newItem.group = item_attrib[ "group" ];
			newItem.sprite_name = item_attrib[ "asset_id" ];
			newItem.width = item_attrib[ "width" ].get_or( 16 );
			newItem.height = item_attrib[ "height" ].get_or( 16 );
			newItem.srcRectX = item_attrib[ "src_rect_x" ].get_or( 0 );
			newItem.srcRectY = item_attrib[ "src_rect_y" ].get_or( 0 );
			newItem.numFrames = item_attrib[ "num_frames" ].get_or( 1 );

			newItem.scale =
				glm::vec2( item_attrib[ "scale" ][ "x" ].get_or( 4 ), item_attrib[ "scale" ][ "y" ].get_or( 4 ) );

			newItem.transOffsetUp = glm::vec2( item_attrib[ "trans_offset_up" ][ "x" ].get_or( 0 ),
											   item_attrib[ "trans_offset_up" ][ "y" ].get_or( 0 ) );

			newItem.transOffsetDown = glm::vec2( item_attrib[ "trans_offset_down" ][ "x" ].get_or( 0 ),
												 item_attrib[ "trans_offset_down" ][ "y" ].get_or( 0 ) );

			newItem.transOffsetRight = glm::vec2( item_attrib[ "trans_offset_right" ][ "x" ].get_or( 0 ),
												  item_attrib[ "trans_offset_right" ][ "y" ].get_or( 0 ) );

			newItem.transOffsetLeft = glm::vec2( item_attrib[ "trans_offset_left" ][ "x" ].get_or( 0 ),
												 item_attrib[ "trans_offset_left" ][ "y" ].get_or( 0 ) );

			newItem.boxSizeUp = glm::vec2( item_attrib[ "box_size_up" ][ "x" ].get_or( 0 ),
										   item_attrib[ "box_size_up" ][ "y" ].get_or( 0 ) );

			newItem.boxSizeDown = glm::vec2( item_attrib[ "box_size_down" ][ "x" ].get_or( 0 ),
											 item_attrib[ "box_size_down" ][ "y" ].get_or( 0 ) );

			newItem.boxSizeRight = glm::vec2( item_attrib[ "box_size_right" ][ "x" ].get_or( 0 ),
											  item_attrib[ "box_size_right" ][ "y" ].get_or( 0 ) );

			newItem.boxSizeLeft = glm::vec2( item_attrib[ "box_size_left" ][ "x" ].get_or( 0 ),
											 item_attrib[ "box_size_left" ][ "y" ].get_or( 0 ) );

			newItem.upOffset = glm::vec2( item_attrib[ "up_offset" ][ "x" ].get_or( 0 ),
										  item_attrib[ "up_offset" ][ "y" ].get_or( 0 ) );

			newItem.downOffset = glm::vec2( item_attrib[ "down_offset" ][ "x" ].get_or( 0 ),
											item_attrib[ "down_offset" ][ "y" ].get_or( 0 ) );

			newItem.rightOffset = glm::vec2( item_attrib[ "right_offset" ][ "x" ].get_or( 0 ),
											 item_attrib[ "right_offset" ][ "y" ].get_or( 0 ) );

			newItem.leftOffset = glm::vec2( item_attrib[ "left_offset" ][ "x" ].get_or( 0 ),
											item_attrib[ "left_offset" ][ "y" ].get_or( 0 ) );
			newItem.duration = item_attrib[ "duration" ].get_or( 3000 );
			newItem.bAnimation = item_attrib[ "animation" ].get_or( false );
			newItem.bVertical = item_attrib[ "vertical" ].get_or( false );

			// Place the item in the map
			m_mapProjectileAttribs.emplace( name, newItem );
		}
		i++;
	}
}

void ProjectileEmitterSystem::SubscribeBtnToEvents( EventManager& eventManager )
{
	eventManager.SubscribeToEvent<GamePadButtonPressedEvent>( this, &ProjectileEmitterSystem::OnBtnPressed );
}
void ProjectileEmitterSystem::OnKeyPressed( KeyPressedEvent& event )
{
	if ( !m_Game.PlayerHold() )
	{
		auto& keyboard = m_InputManager.GetKeyboard();
		auto& gamepad = m_InputManager.GetGamepad();
		auto& player = m_Game.GetPlayer();

		if ( keyboard.IsKeyJustPressed( KEY_SPACE ) || gamepad.IsButtonJustPressed( GP_BTN_X ) )
		{
			ItemUsed();
			m_InputManager.SetAttack( true );
			player->SetAttacking( true );
		}
		else if ( ( keyboard.IsKeyJustPressed( KEY_RSHIFT ) || gamepad.IsButtonJustPressed( GP_BTN_A ) ) &&
				  m_SwordTimer.GetTicks() == 0 )
		{
			SwordUsed();
			m_InputManager.SetAttack( true );
			player->SetAttacking( true );
		}
	}
}

void ProjectileEmitterSystem::OnBtnPressed( GamePadButtonPressedEvent& event )
{
	if ( !m_Game.PlayerHold() )
	{
		auto& player = m_Game.GetPlayer();
		if ( event.button == m_InputManager.GetBtnCode( InputManager::Action::USE_ITEM ) )
		{
			ItemUsed();
			player->SetAttacking( true );
		}
		else if ( event.button == m_InputManager.GetBtnCode( InputManager::Action::ATTACK ) &&
				  m_SwordTimer.GetTicks() == 0 )
		{
			SwordUsed();
			player->SetAttacking( true );
		}
	}
}

void ProjectileEmitterSystem::Update()
{
	for ( auto& entity : GetSystemEntities() )
	{
		// This is the wait time for the player sword
		if ( m_SwordTimer.GetTicks() > 250 )
			m_SwordTimer.Stop();

		// Check the enemy projectile
		if ( entity.BelongsToGroup( "enemies" ) )
		{
			const auto& ai = entity.GetComponent<AIComponent>();
			// If the entity is just a regular enmemy, use this update
			if ( ai.enemyType != EnemyType::NO_TYPE )
				EnemyProjectileUpdate( entity );
			// If the entity is Boss, use this update
			if ( ai.bossType != EnemyBossType::NOT_A_BOSS )
				BossProjectileUpdate( entity );
		}
	}
}

void ProjectileEmitterSystem::EnemyProjectileUpdate( Entity& entity )
{
	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	auto& projectileTransform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	const auto& ai = entity.GetComponent<AIComponent>();

	if ( projectileEmitter.bShotTriggered && !projectileEmitter.bShotFired )
	{
		Entity enemyProjectile = m_Registry.CreateEntity();
		enemyProjectile.Group( "projectile" );
		// Variables that can change based on the type of enemy
		int offsetX = 0;
		int offsetY = 0;
		int srcRectX = 0;
		int srcRectY = 0;
		int scale = 0;

		switch ( ai.enemyType )
		{
		case EnemyType::OCTOROK:
			srcRectX = 48;
			srcRectY = 0;
			scale = 2;
			break;

		case EnemyType::MOBLIN:
			// Set the srcRect for the projectile sprite
			srcRectY = 4 * 16;
			scale = 4;
			switch ( rigid.dir )
			{
			case RigidBodyDir::UP: srcRectX = 0 * 16; break;
			case RigidBodyDir::RIGHT: srcRectX = 1 * 16; break;
			case RigidBodyDir::DOWN: srcRectX = 2 * 16; break;
			case RigidBodyDir::LEFT: srcRectX = 3 * 16; break;
			}

			break;

		case EnemyType::ZORA:
			// TODO: Create a sprite sheet of enemy projectiles
			// enemyProjectile.AddComponent<AnimationComponent>(4, 20, false);
			break;
		default: break;
		}

		// Set the velocity and direction of the projectile based on when it
		// was triggered
		switch ( rigid.dir )
		{
		case RigidBodyDir::UP:
			projectileEmitter.projectileVelocity = glm::vec2( 0, -400 );
			offsetX = 20;
			offsetY = 0;
			break;
		case RigidBodyDir::RIGHT:
			projectileEmitter.projectileVelocity = glm::vec2( 400, 0 );
			offsetX = 0;
			offsetY = 20;
			break;
		case RigidBodyDir::DOWN:
			projectileEmitter.projectileVelocity = glm::vec2( 0, 400 );
			offsetX = 20;
			offsetY = 0;
			break;
		case RigidBodyDir::LEFT:
			projectileEmitter.projectileVelocity = glm::vec2( -400, 0 );
			offsetX = 0;
			offsetY = 20;
			break;
		}

		enemyProjectile.AddComponent<TransformComponent>(
			glm::vec2( projectileTransform.position.x + offsetX, projectileTransform.position.y + offsetY ),
			glm::vec2( scale, scale ),
			0.0 );

		enemyProjectile.AddComponent<RigidBodyComponent>( projectileEmitter.projectileVelocity, rigid.dir );
		enemyProjectile.AddComponent<BoxColliderComponent>( 16, 16, glm::vec2( 0, 0 ) );

		projectileEmitter.bIsFriendly = false;
		enemyProjectile.AddComponent<SpriteComponent>(
			SpriteComponent{ .assetID = "items",
							 .width = 16,
							 .height = 16,
							 .layer = 2,
							 .bIsFixed = false,
							 .srcRect = SDL_Rect{ srcRectX, srcRectY, 16, 16 } } );

		enemyProjectile.AddComponent<ProjectileComponent>(
			ProjectileComponent{ .isFriendly = projectileEmitter.bIsFriendly,
								 .hitPercentDamage = projectileEmitter.hitPercentDamage,
								 .duration = projectileEmitter.projectileDuration,
								 .dir = static_cast<ProjectileDir>( rigid.dir ) } );

		enemyProjectile.AddComponent<GameComponent>();

		projectileEmitter.bShotFired = true;
	}
}

void ProjectileEmitterSystem::BossProjectileUpdate( Entity& entity )
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	const auto& playerPos = playerEnt.GetComponent<TransformComponent>();

	// Boss Variables
	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	auto& projectileTransform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	const auto& ai = entity.GetComponent<AIComponent>();

	if ( ai.bossType == EnemyBossType::AQUAMENTUS )
	{
		if ( projectileEmitter.bShotTriggered && !projectileEmitter.bShotFired )
		{
			Entity enemyProjectile = m_Registry.CreateEntity();
			Entity enemyProjectile2 = m_Registry.CreateEntity();
			Entity enemyProjectile3 = m_Registry.CreateEntity();

			// Have the fireball change direction based on the player position
			glm::vec2 direction = glm::normalize( playerPos.position - projectileTransform.position );

			enemyProjectile.Group( "projectile" );
			enemyProjectile.AddComponent<TransformComponent>(
				glm::vec2( projectileTransform.position.x, projectileTransform.position.y ), glm::vec2( 4, 4 ), 0.0 );

			enemyProjectile.AddComponent<RigidBodyComponent>( glm::vec2( direction.x * 300, direction.y * 300 ) );
			enemyProjectile.AddComponent<BoxColliderComponent>( 16, 16, glm::vec2( 0, 0 ) );

			projectileEmitter.bIsFriendly = false;
			enemyProjectile.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "fire_ball",
																			.width = 16,
																			.height = 16,
																			.layer = 2,
																			.bIsFixed = false,
																			.srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

			enemyProjectile.AddComponent<AnimationComponent>( AnimationComponent{
				.numFrames = 4, .frameSpeedRate = 20, .vertical = false, .isLooped = true, .frameOffset = 0 } );

			enemyProjectile.AddComponent<ProjectileComponent>( ProjectileComponent{
				.isFriendly = projectileEmitter.bIsFriendly,
				.hitPercentDamage = projectileEmitter.hitPercentDamage,
				.duration = projectileEmitter.projectileDuration //,
				//.dir = static_cast<ProjectileDir>(rigid.dir)
			} );

			enemyProjectile.AddComponent<GameComponent>();

			enemyProjectile2.Group( "projectile" );
			enemyProjectile2.AddComponent<TransformComponent>(
				glm::vec2( projectileTransform.position.x, projectileTransform.position.y ), glm::vec2( 4, 4 ), 0.0 );

			enemyProjectile2.AddComponent<RigidBodyComponent>( glm::vec2( direction.x * 300, direction.y * 400 ) );
			enemyProjectile2.AddComponent<BoxColliderComponent>( 16, 16, glm::vec2( 0, 0 ) );

			projectileEmitter.bIsFriendly = false;
			enemyProjectile2.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "fire_ball",
																			 .width = 16,
																			 .height = 16,
																			 .layer = 2,
																			 .bIsFixed = false,
																			 .srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

			enemyProjectile2.AddComponent<AnimationComponent>( AnimationComponent{
				.numFrames = 4, .frameSpeedRate = 20, .vertical = false, .isLooped = true, .frameOffset = 0 } );

			enemyProjectile2.AddComponent<ProjectileComponent>(
				ProjectileComponent{ .isFriendly = projectileEmitter.bIsFriendly,
									 .hitPercentDamage = projectileEmitter.hitPercentDamage,
									 .duration = projectileEmitter.projectileDuration } );

			enemyProjectile2.AddComponent<GameComponent>();

			enemyProjectile3.Group( "projectile" );
			enemyProjectile3.AddComponent<TransformComponent>(
				glm::vec2( projectileTransform.position.x, projectileTransform.position.y ), glm::vec2( 4, 4 ), 0.0 );

			enemyProjectile3.AddComponent<RigidBodyComponent>( glm::vec2( direction.x * 300, direction.y * -300 ) );
			enemyProjectile3.AddComponent<BoxColliderComponent>( 16, 16, glm::vec2( 0, 0 ) );

			projectileEmitter.bIsFriendly = false;
			enemyProjectile3.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "fire_ball",
																			 .width = 16,
																			 .height = 16,
																			 .layer = 2,
																			 .bIsFixed = false,
																			 .srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

			enemyProjectile3.AddComponent<AnimationComponent>( AnimationComponent{
				.numFrames = 4, .frameSpeedRate = 20, .vertical = false, .isLooped = true, .frameOffset = 0 } );

			enemyProjectile3.AddComponent<ProjectileComponent>( ProjectileComponent{
				.isFriendly = projectileEmitter.bIsFriendly,
				.hitPercentDamage = projectileEmitter.hitPercentDamage,
				.duration = projectileEmitter.projectileDuration,
			} );

			enemyProjectile3.AddComponent<GameComponent>();

			projectileEmitter.bShotFired = true;
		}
	}
}

std::string ProjectileEmitterSystem::ProjectileAttrib::ToString()
{
	std::stringstream ss;
	ss << "Group: " << group << "\n"
	   << "Sprite Name: " << sprite_name << "\n"
	   << "Sprite width: " << width << "\n"
	   << "Sprite height: " << height << "\n"
	   << "Num Frames: " << numFrames << "\n"
	   << "Src Rect: [X: " << srcRectX << ", Y: " << srcRectY << "]"
	   << "\n"
	   << "Scale: [X: " << scale.x << ", Y: " << scale.y << "]"
	   << "\n"
	   << "Tran Offset Up: [X: " << transOffsetUp.x << ", Y: " << transOffsetUp.y << "]"
	   << "\n"
	   << "Tran Offset Down: [X: " << transOffsetDown.x << ", Y: " << transOffsetDown.y << "]"
	   << "\n"
	   << "Tran Offset Left: [X: " << transOffsetLeft.x << ", Y: " << transOffsetLeft.y << "]"
	   << "\n"
	   << "Tran Offset Right: [X: " << transOffsetRight.x << ", Y: " << transOffsetRight.y << "]"
	   << "\n"
	   << "Box Size Up: [X: " << boxSizeUp.x << ", Y: " << boxSizeUp.y << "]"
	   << "\n"
	   << "Box Size Down: [X: " << boxSizeDown.x << ", Y: " << boxSizeDown.y << "]"
	   << "\n"
	   << "Box Size Right: [X: " << boxSizeRight.x << ", Y: " << boxSizeRight.y << "]"
	   << "\n"
	   << "Box Size Left: [X: " << boxSizeLeft.x << ", Y: " << boxSizeLeft.y << "]"
	   << "\n"
	   << "Box offset Up: [X: " << upOffset.x << ", Y: " << upOffset.y << "]"
	   << "\n"
	   << "Box offset Down: [X: " << downOffset.x << ", Y: " << downOffset.y << "]"
	   << "\n"
	   << "Box offset Right: [X: " << rightOffset.x << ", Y: " << rightOffset.y << "]"
	   << "\n"
	   << "Box offset Left: [X: " << leftOffset.x << ", Y: " << leftOffset.y << "]"
	   << "\n"
	   << "Duration: " << duration << "\n"
	   << "Animation: " << std::boolalpha << bAnimation << "\n"
	   << "Vertical: " << std::boolalpha << bVertical << "\n";

	return ss.str();
}
