#include "Systems/GameSystems/DamageSystem.h"
#include "Components/HealthComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/TransformComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/AIComponent.h"
#include "Components/SpriteComponent.h"
#include "Events/EventManager.h"
#include "Events/CollisionEvent.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Systems/SoundFXSystem.h"
#include "Utilities/GameData.h"

DamageSystem::DamageSystem()
	: m_Game( Game::Instance() )
{
	RequiredComponent<BoxColliderComponent>();
	RequiredComponent<HealthComponent>();
}

void DamageSystem::SubscribeToEvents( EventManager& eventManager )
{
	eventManager.SubscribeToEvent<CollisionEvent>( this, &DamageSystem::OnCollision );
}

void DamageSystem::OnCollision( CollisionEvent& event )
{
	Entity& a = event.a;
	Entity& b = event.b;

	// Enemy projectile hits the player
	if ( a.HasComponent<ProjectileComponent>() && b.HasComponent<PlayerComponent>() )
	{
		OnPlayerHitsProjectile( a, b );
		return;
	}
	else if ( b.HasComponent<ProjectileComponent>() && a.HasComponent<PlayerComponent>() )
	{
		OnPlayerHitsProjectile( b, a );
		return;
	}

	// Player/Enemy run into each other
	if ( a.HasComponent<EnemyComponent>() && b.HasTag( "player" ) )
	{
		OnEnemyHitsPlayer( a, b );
		return;
	}

	else if ( b.HasComponent<EnemyComponent>() && a.HasTag( "player" ) )
	{
		OnEnemyHitsPlayer( b, a );
		return;
	}

	bool bAttacking{ m_Game.GetPlayer()->IsAttacking() };
	// Player Sword Hits Enemy
	if ( b.HasTag( "the_sword" ) && a.HasComponent<EnemyComponent>() )
	{
		if ( bAttacking )
			OnPlayerSwordHitsEnemy( b, a );

		return;
	}
	else if ( a.HasTag( "the_sword" ) && b.HasComponent<EnemyComponent>() )
	{
		if ( bAttacking )
			OnPlayerSwordHitsEnemy( a, b );

		return;
	}

	// Player Sword beam or arrow hits enemy
	if ( a.HasComponent<EnemyComponent>() && ( b.HasComponent<ProjectileComponent>() && !b.HasTag( "boomerang" ) ) )
	{
		OnEnemyHitsPlayerProjectile( a, b );
		return;
	}
	else if ( b.HasComponent<EnemyComponent>() &&
			  ( a.HasComponent<ProjectileComponent>() && !a.HasTag( "boomerang" ) ) )
	{
		OnEnemyHitsPlayerProjectile( b, a );
		return;
	}

	if ( a.HasComponent<EnemyComponent>() && b.HasTag( "boomerang" ) )
	{
		OnEnemyHitsBoomerang( a, b );
		return;
	}
	else if ( b.HasComponent<EnemyComponent>() && a.HasTag( "boomerang" ) )
	{
		OnEnemyHitsBoomerang( b, a );
		return;
	}
}

void DamageSystem::OnPlayerHitsProjectile( Entity projectile, Entity player )
{
	// Should I put a check for this?
	auto& playerHealth = player.GetComponent<HealthComponent>();
	auto& project = projectile.GetComponent<ProjectileComponent>();
	if ( project.isFriendly || playerHealth.bIsHurt )
		return;

	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& playerRigid = player.GetComponent<RigidBodyComponent>();
	auto& projectileRigid = projectile.GetComponent<RigidBodyComponent>();

	bool is_shield = player.HasTag( "the_shield" );

	if ( !is_shield )
	{
		playerHealth.healthPercentage -=
			project.hitPercentDamage; // This will need to change based on the enemy attack power!

		if ( !playerTransform.bCollision )
		{
			switch ( projectileRigid.dir )
			{
			case RigidBodyDir::UP: playerRigid.velocity.y -= 64; break;
			case RigidBodyDir::DOWN: playerRigid.velocity.y += 64; break;
			case RigidBodyDir::LEFT: playerRigid.velocity.x -= 64; break;
			case RigidBodyDir::RIGHT: playerRigid.velocity.y += 64; break;
			case RigidBodyDir::NO_DIR: playerRigid.velocity.y -= 64; break;
			}
		}

		m_Game.GetSoundPlayer().PlaySoundFX( "link_hurt", 0, SoundChannel::ANY );
		playerHealth.bIsHurt = true;
		// Remove the projectile
		projectile.Kill();
	}
	else
	{
		// Have the projectile bounce off the shield and not hurt the player
		m_Game.GetSoundPlayer().PlaySoundFX( "shield", 0, SoundChannel::ANY );

		if ( projectileRigid.velocity.y < 0 )
		{
			projectileRigid.velocity.y = 100;
			projectileRigid.velocity.x = ( rand() % 2 - 1 ) * 50;
		}
		else if ( projectileRigid.velocity.y > 0 )
		{
			projectileRigid.velocity.y = -100;
			projectileRigid.velocity.x = ( rand() % 2 - 1 ) * 50;
		}
		else if ( projectileRigid.velocity.x > 0 )
		{
			projectileRigid.velocity.x = -100;
			projectileRigid.velocity.y = ( rand() % 2 - 1 ) * 50;
		}
		else if ( projectileRigid.velocity.x < 0 )
		{
			projectileRigid.velocity.x = 100;
			projectileRigid.velocity.y = ( rand() % 2 - 1 ) * 50;
		}

		// Start the projectile death
		// project.startTime = project.duration;
		projectile.Kill();
	}
}

void DamageSystem::OnPlayerSwordHitsEnemy( Entity sword, Entity enemy )
{
	if ( !enemy.HasComponent<HealthComponent>() )
		return;

	auto& enemyHealth = enemy.GetComponent<HealthComponent>();

	if ( /*!KeyboardControlSystem::keyDown || */ enemyHealth.bIsHurt )
		return;

	auto& enemyTransform = enemy.GetComponent<TransformComponent>();
	auto& enemyRigidbody = enemy.GetComponent<RigidBodyComponent>();
	const auto& player = m_Game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	const auto& player_rb = playerEnt.GetComponent<RigidBodyComponent>();

	auto& enemyAI = enemy.GetComponent<AIComponent>();
	if ( enemyAI.enemyType != EnemyType::LEEVER )
	{
		// Subtract 1 HP from the enemy
		enemyHealth.healthPercentage -= player->GetAttackValue();

		if ( !enemyTransform.bCollision )
		{
			// Move the enemy slightly away in the opposite direction of movement
			switch ( player_rb.dir )
			{
			case RigidBodyDir::UP:
				enemyRigidbody.velocity.y -= 64;
				enemyRigidbody.dir = RigidBodyDir::UP;
				break;
			case RigidBodyDir::RIGHT:
				enemyTransform.position.x += 64;
				enemyRigidbody.dir = RigidBodyDir::RIGHT;
				break;
			case RigidBodyDir::DOWN:
				enemyRigidbody.velocity.y += 64;
				enemyRigidbody.dir = RigidBodyDir::DOWN;
				break;
			case RigidBodyDir::LEFT:
				enemyRigidbody.velocity.x -= 64;
				enemyRigidbody.dir = RigidBodyDir::LEFT;
				break;
			}
		}
		// Play the enemy hit sound FX
		m_Game.GetSoundPlayer().PlaySoundFX( "enemy_hit", 0, SoundChannel::ANY );

		if ( enemyHealth.healthPercentage <= 0 )
		{
			// Play the enemy die sound and kill the entity
			m_Game.GetSoundPlayer().PlaySoundFX( "enemy_die", 0, SoundChannel::ANY );
		}
		enemyHealth.bIsHurt = true;
	}
	else
	{
		// We cannot attack the leever while it is transitioning from above/under ground
		if ( enemyAI.leeverTimer.GetTicks() > 3100 && enemyAI.leeverTimer.GetTicks() < 10000 )
		{
			// Subtract 1 HP from the enemy
			enemyHealth.healthPercentage -= player->GetAttackValue();

			// Move the enemy slightly away in the opposite direction of movement
			if ( enemyRigidbody.velocity.x > 0 )
				enemyTransform.position.x -= 5;
			if ( enemyRigidbody.velocity.x < 0 )
				enemyTransform.position.x += 5;
			if ( enemyRigidbody.velocity.y > 0 )
				enemyTransform.position.y -= 5;
			if ( enemyRigidbody.velocity.y < 0 )
				enemyTransform.position.y += 5;

			// Play the enemy hit sound FX
			m_Game.GetSoundPlayer().PlaySoundFX( "enemy_hit", 0, SoundChannel::ANY );

			if ( enemyHealth.healthPercentage <= 0 )
			{
				// Play the enemy die sound and kill the entity
				m_Game.GetSoundPlayer().PlaySoundFX( "enemy_die", 0, SoundChannel::ANY );
			}
			enemyHealth.bIsHurt = true;
		}
	}
}

void DamageSystem::OnEnemyHitsPlayerProjectile( Entity enemy, Entity projectile )
{
	if ( !enemy.HasComponent<HealthComponent>() )
		return;

	auto& health = enemy.GetComponent<HealthComponent>();
	auto& proj = projectile.GetComponent<ProjectileComponent>();

	if ( !proj.isFriendly || health.bIsHurt )
		return;

	const auto& proj_rb = projectile.GetComponent<RigidBodyComponent>();
	auto& enemyTransform = enemy.GetComponent<TransformComponent>();
	auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();
	auto& enemyAI = enemy.GetComponent<AIComponent>();

	if ( enemyAI.enemyType != EnemyType::LEEVER )
	{
		health.healthPercentage -= 1;
		if ( !enemyTransform.bCollision )
		{
			switch ( proj_rb.dir )
			{
			case RigidBodyDir::UP:
				enemyTransform.position.y -= 64;
				rigidBody.dir = RigidBodyDir::UP;
				break;
			case RigidBodyDir::RIGHT:
				enemyTransform.position.x += 64;
				rigidBody.dir = RigidBodyDir::RIGHT;
				break;
			case RigidBodyDir::DOWN:
				enemyTransform.position.y += 64;
				rigidBody.dir = RigidBodyDir::DOWN;
				break;
			case RigidBodyDir::LEFT:
				enemyTransform.position.x -= 64;
				rigidBody.dir = RigidBodyDir::LEFT;
				break;
			}
		}

		if ( health.healthPercentage <= 0 )
		{
			// Play the enemy die sound and kill the entity
			m_Game.GetSoundPlayer().PlaySoundFX( "enemy_die", 0, SoundChannel::ANY );
		}
		else
		{
			// Play the enemy hit sound FX
			m_Game.GetSoundPlayer().PlaySoundFX( "enemy_hit", 0, SoundChannel::ANY );
		}

		health.bIsHurt = true;
	}
	else
	{
		// We cannot attack the leever while it is transitioning from above/under ground
		if ( enemyAI.leeverTimer.GetTicks() > 3100 && enemyAI.leeverTimer.GetTicks() < 10000 )
		{
			health.healthPercentage -= 1;

			if ( !enemyTransform.bCollision )
			{
				// Move the enemy slightly away in the opposite direction of movement
				switch ( proj_rb.dir )
				{
				case RigidBodyDir::UP: enemyTransform.position.y -= 64; break;
				case RigidBodyDir::RIGHT: enemyTransform.position.x += 64; break;
				case RigidBodyDir::DOWN: enemyTransform.position.y += 64; break;
				case RigidBodyDir::LEFT: enemyTransform.position.x -= 64; break;
				}
			}

			if ( health.healthPercentage <= 0 )
			{
				// Play the enemy die sound and kill the entity
				m_Game.GetSoundPlayer().PlaySoundFX( "enemy_die", 0, -1 );
			}
			else
			{
				// Play the enemy hit sound FX
				m_Game.GetSoundPlayer().PlaySoundFX( "enemy_hit", 0, -1 );
			}

			health.bIsHurt = true;
		}
	}

	if ( projectile.BelongsToGroup( "beam" ) )
	{
		auto& sprite = projectile.GetComponent<SpriteComponent>();
		auto& animation = projectile.GetComponent<AnimationComponent>();
		auto& rigid = projectile.GetComponent<RigidBodyComponent>();

		rigid.velocity = glm::vec2( 0 );
		sprite.srcRect.y = 128;
		animation.vertical = false;
		animation.numFrames = 4;
		animation.isLooped = false;
		animation.startTime = static_cast<int>( SDL_GetTicks() );
		animation.currentFrame = 0;
	}

	// Kill the projectile --> start the projectile death
	proj.startTime = proj.duration - 500;
}

void DamageSystem::OnEnemyHitsPlayer( Entity enemy, Entity player )
{
	auto& playerHealth = player.GetComponent<HealthComponent>();

	if ( enemy.HasComponent<HealthComponent>() )
	{
		const auto& enemyHealth = enemy.GetComponent<HealthComponent>();

		if ( enemyHealth.healthPercentage <= 0 )
			return;
	}

	if ( playerHealth.bIsHurt )
		return;

	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& rigidBody = player.GetComponent<RigidBodyComponent>();
	const auto& enemy_rb = enemy.GetComponent<RigidBodyComponent>();
	playerHealth.healthPercentage -= 1;

	if ( !playerTransform.bCollision )
	{
		switch ( enemy_rb.dir )
		{
		case RigidBodyDir::UP:
			playerTransform.position.y -= 64;
			rigidBody.dir = RigidBodyDir::UP;
			break;
		case RigidBodyDir::RIGHT:
			playerTransform.position.x += 64;
			rigidBody.dir = RigidBodyDir::RIGHT;
			break;
		case RigidBodyDir::DOWN:
			playerTransform.position.y += 64;
			rigidBody.dir = RigidBodyDir::DOWN;
			break;
		case RigidBodyDir::LEFT:
			playerTransform.position.x -= 64;
			rigidBody.dir = RigidBodyDir::LEFT;
			break;
		}
	}

	m_Game.GetSoundPlayer().PlaySoundFX( "link_hurt", 0, SoundChannel::ANY );
	playerHealth.bIsHurt = true;
}

void DamageSystem::OnEnemyHitsBoomerang( Entity enemy, Entity boomerang )
{
	auto& enemyRigid = enemy.GetComponent<RigidBodyComponent>();
	auto& enemyAI = enemy.GetComponent<AIComponent>();

	enemyRigid.velocity = glm::vec2( 0 );

	if ( !enemyAI.bStunned )
	{
		enemyAI.bStunned = true;
		// Play the enemy hit sound FX
		m_Game.GetSoundPlayer().PlaySoundFX( "enemy_hit", 0, SoundChannel::ANY );
	}
}