#include "Systems/GameSystems/MovementSystem.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/TransformComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/HealthComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/AIComponent.h"
#include "Systems/SoundFXSystem.h"
#include "Events/EventManager.h"
#include "Events/CollisionEvent.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/Utility.h"

constexpr int MAX_WORLD_WIDTH = 16;
constexpr int MAX_WORLD_HEIGHT = 8;

void MovementSystem::UpdateBoomerang( const double& deltaTime )
{
	// If there is no boomerang, nothing to update!
	if ( !Registry::Instance().DoesTagExist( "boomerang" ) )
		return;

	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	const auto& playerPos = playerEnt.GetComponent<TransformComponent>();
	const auto& playerSprite = playerEnt.GetComponent<SpriteComponent>();

	// Boomerang variables
	auto boomerang = Registry::Instance().GetEntityByTag( "boomerang" );
	auto& rigidbody = boomerang.GetComponent<RigidBodyComponent>();
	auto& proj = boomerang.GetComponent<ProjectileComponent>();
	auto& transform = boomerang.GetComponent<TransformComponent>();

	// Have the boomerang change direction based on the player position
	glm::vec2 direction = glm::normalize( (
		( playerPos.position + glm::vec2( playerSprite.width / 2, playerSprite.height / 2 ) ) - transform.position ) );
	bool boomerangReturned = false;

	if ( proj.boomTimer.GetTicks() > 300 ) // TODO: Change the time based on type of boomerang-->wood/magic
	{
		rigidbody.velocity.x = direction.x * 500;
		rigidbody.velocity.y = direction.y * 500;
		boomerangReturned = true;
	}

	if ( !transform.bCollision )
	{
		transform.position.x += rigidbody.velocity.x * deltaTime;
		transform.position.y += rigidbody.velocity.y * deltaTime;
	}
	else
	{
		boomerangReturned = true;
	}

	if ( boomerangReturned )
	{
		bool setKill = false;
		transform.position.x += direction.x * deltaTime;
		transform.position.y += direction.y * deltaTime;

		// If the boomerang position is within any of these given parameters --> Kill it
		if ( transform.position.x <= playerPos.position.x + ( playerSprite.width / 2.f ) &&
			 transform.position.y <= playerPos.position.y + ( playerSprite.height / 2.f ) &&
			 transform.position.x >= playerPos.position.x && transform.position.y >= playerPos.position.y )
			setKill = true;

		if ( setKill )
			boomerang.Kill();
	}
}

void MovementSystem::UpdateEnemies( const double& deltaTime )
{
	const auto& enemies = Registry::Instance().GetEntitiesByGroup( "enemies" );

	for ( const auto& enemy : enemies )
	{
		auto& transform = enemy.GetComponent<TransformComponent>();
		auto& enemyComponent = enemy.GetComponent<EnemyComponent>();
		auto& ai = enemy.GetComponent<AIComponent>();
		const auto& collider = enemy.GetComponent<BoxColliderComponent>();

		// Calculate the min max positions -- TODO: MOVE THIS TO ENEMY COMPONENT
		if ( !enemyComponent.distanceCalculated )
		{
			const int entityXPanel = transform.position.x / PANEL_WIDTH;
			const int entityYPanel = transform.position.y / PANEL_HEIGHT;

			enemyComponent.minDistance.x = entityXPanel * PANEL_WIDTH;
			enemyComponent.minDistance.y = entityYPanel * PANEL_HEIGHT;

			enemyComponent.maxDistance.x = enemyComponent.minDistance.x + PANEL_WIDTH;
			enemyComponent.maxDistance.y = enemyComponent.minDistance.y + PANEL_HEIGHT;

			enemyComponent.distanceCalculated = true;
			ai.enemyPos = glm::vec2{ entityXPanel, entityYPanel };
		}

		if ( ai.enemyPos != m_Game.GetPlayer()->GetPlayerPos() || ai.enemyType == EnemyType::BLADE_TRAP )
			continue;

		auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();
		auto& sprite = enemy.GetComponent<SpriteComponent>();
		auto& bCollision = enemy.GetComponent<BoxColliderComponent>();
		auto& health = enemy.GetComponent<HealthComponent>();

		if ( !transform.bCollision && !health.bIsHurt && health.healthPercentage > 0 )
		{
			transform.position.x += rigidBody.velocity.x * deltaTime;
			transform.position.y += rigidBody.velocity.y * deltaTime;

			if ( transform.position.x <= enemyComponent.minDistance.x )
			{
				transform.position.x = enemyComponent.minDistance.x;
				rigidBody.velocity.x *= -1;
			}

			if ( transform.position.x >= enemyComponent.maxDistance.x )
			{
				transform.position.x = enemyComponent.maxDistance.x - ( collider.width * transform.scale.x );
				rigidBody.velocity.x *= -1;
			}

			if ( transform.position.y <= enemyComponent.minDistance.y )
			{
				transform.position.y = enemyComponent.minDistance.y;
				rigidBody.velocity.y *= -1;
			}

			if ( transform.position.y >= enemyComponent.maxDistance.y )
			{
				transform.position.y = enemyComponent.maxDistance.y - ( collider.height * transform.scale.y );
				rigidBody.velocity.y *= -1;
			}
		}

		SetDirection( rigidBody );

		switch ( rigidBody.dir )
		{
		case RigidBodyDir::UP: sprite.srcRect.x = sprite.width * 2 + sprite.offset.x; break;
		case RigidBodyDir::RIGHT: sprite.srcRect.x = sprite.width * 3 + sprite.offset.x; break;
		case RigidBodyDir::DOWN: sprite.srcRect.x = sprite.width * 0 + sprite.offset.x; break;
		case RigidBodyDir::LEFT: sprite.srcRect.x = sprite.width * 1 + sprite.offset.x; break;
		}
	}
}

void MovementSystem::UpdatePlayer( const double& deltaTime )
{
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& player_transform = playerEnt.GetComponent<TransformComponent>();
	auto& player_rigidBody = playerEnt.GetComponent<RigidBodyComponent>();
	auto& player_sprite = playerEnt.GetComponent<SpriteComponent>();
	auto& player_bCollision = playerEnt.GetComponent<BoxColliderComponent>();

	const int playerXPanel = player_transform.position.x / PANEL_WIDTH;
	const int playerYPanel = player_transform.position.y / PANEL_HEIGHT;

	player->SetPlayerPos( glm::vec2( playerXPanel, playerYPanel ) );

	if ( !player_transform.bCollision )
	{
		player_transform.position.x += player_rigidBody.velocity.x * deltaTime;
		player_transform.position.y += player_rigidBody.velocity.y * deltaTime;
	}

	// Stop the Sword and shield from moving ahead while transitioning from different screens
	auto sword = player->GetSword();
	auto& swordTransform = sword.GetComponent<TransformComponent>();
	auto shield = player->GetShield();
	auto& shieldTransform = shield.GetComponent<TransformComponent>();

	if ( player_rigidBody.velocity.x == 0 && player_rigidBody.velocity.y == 0 )
	{
		swordTransform.position = player_transform.position;
		shieldTransform.position = player_transform.position;
	}

	SetDirection( player_rigidBody );
}

void MovementSystem::UpdateProjectiles( const double& deltaTime )
{
	if ( !Registry::Instance().DoesGroupExist( "projectile" ) )
		return;

	const auto& projectiles = Registry::Instance().GetEntitiesByGroup( "projectile" );

	for ( const auto& projectile : projectiles )
	{
		auto& transform = projectile.GetComponent<TransformComponent>();
		auto& rigidBody = projectile.GetComponent<RigidBodyComponent>();
		auto& sprite = projectile.GetComponent<SpriteComponent>();
		auto& bCollision = projectile.GetComponent<BoxColliderComponent>();

		if ( !transform.bCollision )
		{
			transform.position.x += rigidBody.velocity.x * deltaTime;
			transform.position.y += rigidBody.velocity.y * deltaTime;
		}

		SetDirection( rigidBody );
	}
}

void MovementSystem::SetDirection( RigidBodyComponent& rigidBody )
{
	if ( rigidBody.velocity.y > 0 )
	{
		rigidBody.dir = RigidBodyDir::DOWN;
	}
	else if ( rigidBody.velocity.y < 0 )
	{
		rigidBody.dir = RigidBodyDir::UP;
	}
	else if ( rigidBody.velocity.x > 0 )
	{
		rigidBody.dir = RigidBodyDir::RIGHT;
	}
	else if ( rigidBody.velocity.x < 0 )
	{
		rigidBody.dir = RigidBodyDir::LEFT;
	}
}

MovementSystem::MovementSystem()
	: m_Game( Game::Instance() )
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<RigidBodyComponent>();
	RequiredComponent<BoxColliderComponent>();
}

void MovementSystem::Update( const double& deltaTime )
{
	UpdateBoomerang( deltaTime );
	UpdatePlayer( deltaTime );
	UpdateEnemies( deltaTime );
	UpdateProjectiles( deltaTime );
}
