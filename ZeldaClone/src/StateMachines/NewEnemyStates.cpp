#include "StateMachines/NewEnemyStates.h"
#include "StateMachines/UpdateEnemies.h"
#include "ECS/ECS.h"

#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/AIComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ItemComponent.h"
#include "Components/RupeeGameComponent.h"
#include "Components/GameComponent.h"

#include "Game/Game.h"
#include "Game/Player.h"

// IdleState
void EnemyIdleState::OnEnter( Entity& entity )
{
	// auto& ai = entity.GetComponent<AIComponent>();

	// if (ai.enemyType() != EnemyType::LEEVER)
	//{
	//	auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	//	if (projEmitter.shotTriggered)
	//		projEmitter.timer.Start();
	// }
}

void EnemyIdleState::OnExit( Entity& entity )
{
}

void EnemyIdleState::Update( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();

	if ( ai.bIsBoss )
		return;

	switch ( ai.enemyType )
	{
	case EnemyType::OCTOROK:
	case EnemyType::MOBLIN: IdleUpdateOctoMoblin( entity ); break;
	case EnemyType::DARKNUT: break;
	case EnemyType::LEEVER: IdleUpdateLever( entity ); break;
	case EnemyType::TEKTITE: break;
	case EnemyType::PEAHAT: break;
	case EnemyType::ARMOS: break;
	case EnemyType::KEESE: break;
	case EnemyType::BLADE_TRAP: IdleUpdateBladeTrap( entity ); return;
	case EnemyType::GEL: break;
	case EnemyType::STALFOS: break;
	case EnemyType::GORIYA: break;
	case EnemyType::NO_TYPE: return;
	}
}

// AttackState
void EnemyAttackState::OnEnter( Entity& entity )
{
	// Logger::Err("entering Attack State");
}

void EnemyAttackState::Update( Entity& entity )
{
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
}

void EnemyAttackState::OnExit( Entity& entity )
{
	// Logger::Err("Exiting Attack State");
}

// PatrolState
PatrolState::PatrolState()
{
}

void PatrolState::OnEnter( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();
	ai.aiTimer.Start();

	if ( ai.enemyType != EnemyType::LEEVER )
	{
		auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		if ( !projEmitter.bShotTriggered )
		{
			projEmitter.timer.Start();
		}
	}
}
void PatrolState::OnExit( Entity& entity )
{
	// if (!entity.HasComponent<ProjectileEmitterComponent>())
	//	return;

	// auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	// projEmitter.timer.Stop();
}

void PatrolState::Update( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();

	if ( ai.bIsBoss )
		return;

	switch ( ai.enemyType )
	{
	case EnemyType::OCTOROK:
	case EnemyType::MOBLIN: PatrolUpdateOctoMoblin( entity ); break;
	case EnemyType::DARKNUT: break;
	case EnemyType::LEEVER: PatrolUpdateLever( entity ); break;
	case EnemyType::TEKTITE: break;
	case EnemyType::PEAHAT: break;
	case EnemyType::ARMOS: break;
	case EnemyType::KEESE: break;
	case EnemyType::BLADE_TRAP: PatrolUpdateBladeTrap( entity ); return;
	case EnemyType::GEL: break;
	case EnemyType::STALFOS: break;
	case EnemyType::GORIYA: break;
	case EnemyType::NO_TYPE: return;
	}
}

// HurtState
void HurtState::OnEnter( Entity& entity )
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();

	// Start hurt invinciblity timer
	enemyHealth.hurtTimer.Start();

	rigid.velocity = glm::vec2( 0 );
	animation.numFrames = 3;
	animation.frameOffset = 32;
	animation.frameSpeedRate = 20;
}
void HurtState::Update( Entity& entity )
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	if ( !ai.esm )
		return;

	if ( enemyHealth.hurtTimer.GetTicks() > 1000 && enemyHealth.healthPercentage > 0 )
	{
		enemyHealth.bIsHurt = false;
		enemyHealth.hurtTimer.Stop();

		if ( !ai.stunTimer.isStarted() )
		{
			ai.esm->AddState( std::make_unique<EnemyIdleState>() );
			ai.esm->ChangeState( entity );
		}
		else
		{
			ai.esm->AddState( std::make_unique<EnemyStunnedState>() );
			ai.esm->ChangeState( entity );
		}
	}
	// If the enemy's heath is <= 0, call death state!
	else if ( enemyHealth.healthPercentage <= 0 )
	{
		// Call the EnemyDeathState
		ai.esm->AddState( std::make_unique<EnemyDeathState>() );
		ai.esm->ChangeState( entity );
	}
}

void HurtState::OnExit( Entity& entity )
{
	auto& animation = entity.GetComponent<AnimationComponent>();
	animation.numFrames = 2;
	animation.frameOffset = 0;
	animation.frameSpeedRate = 10;
}

// Death State
void EnemyDeathState::OnEnter( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();

	sprite.assetID = "enemy_death";
	sprite.height = 16;
	sprite.width = 16;
	sprite.srcRect.x = 0;
	sprite.srcRect.y = 0;

	ai.deathTimer.Start();

	animation.frameOffset = 0;
	animation.numFrames = 8;
	animation.currentFrame = 0;
	animation.frameSpeedRate = 15;
	animation.isLooped = false;
	animation.vertical = false;
	animation.startTime = static_cast<int>( SDL_GetTicks() );

	srand( SDL_GetTicks() );

	int chance = rand();
	if ( chance % 2 == 0 )
	{
		ItemDrop( entity );
	}
}
void EnemyDeathState::Update( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();

	if ( ai.deathTimer.GetTicks() > 500 )
	{
		entity.Kill();
	}
}

void EnemyDeathState::OnExit( Entity& entity )
{
	// Animation does not have to return to regular because the entity is destroyed!
}

void EnemyStunnedState::OnEnter( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();
	ai.stunTimer.Start();
}

void EnemyStunnedState::OnExit( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();
	ai.bStunned = false;
}

void EnemyStunnedState::Update( Entity& entity )
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	if ( !ai.esm )
		return;

	if ( ai.stunTimer.GetTicks() > 3000 )
	{
		ai.stunTimer.Stop();
		ai.esm->AddState( std::make_unique<EnemyIdleState>() );
		ai.esm->ChangeState( entity );
	}

	if ( enemyHealth.bIsHurt )
	{
		ai.esm->AddState( std::make_unique<HurtState>() );
		ai.esm->ChangeState( entity );
	}
}

void EnemyDeathState::ItemDrop( Entity& enemy )
{
	const auto& enemyTransform = enemy.GetComponent<TransformComponent>();
	auto& pos = enemyTransform.position;
	int chance = rand();

	if ( chance % 7 != 0 && chance % 5 != 0 && chance % 3 != 0 && chance % 2 != 0 )
		return;

	Entity item = Registry::Instance().CreateEntity();
	item.Group( "items" );
	item.AddComponent<TransformComponent>( pos, glm::vec2( 4, 4 ) );
	item.AddComponent<BoxColliderComponent>( 16, 16 );
	item.AddComponent<GameComponent>();

	if ( chance % 7 == 0 )
	{
		item.AddComponent<ItemComponent>( ItemCollectType::BOMBS );
		item.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "items",
															 .width = 16,
															 .height = 16,
															 .layer = 1,
															 .bIsFixed = false,
															 .srcRect = SDL_Rect{ 64, 112, 16, 16 } } );
	}

	else if ( chance % 5 == 0 )
	{
		item.AddComponent<ItemComponent>( ItemCollectType::BLUE_RUPEE );
		item.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hearts",
															 .width = 16,
															 .height = 16,
															 .layer = 1,
															 .bIsFixed = false,
															 .srcRect = SDL_Rect{ 48, 0, 16, 16 } } );

		item.AddComponent<AnimationComponent>( AnimationComponent{
			.numFrames = 2, .frameSpeedRate = 10, .vertical = false, .isLooped = true, .frameOffset = 48 } );
	}
	else if ( chance % 3 == 0 )
	{
		item.AddComponent<ItemComponent>( ItemCollectType::HEARTS );
		item.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hearts",
															 .width = 16,
															 .height = 16,
															 .layer = 1,
															 .bIsFixed = false,
															 .srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

		item.AddComponent<AnimationComponent>( AnimationComponent{
			.numFrames = 2, .frameSpeedRate = 10, .vertical = false, .isLooped = true, .frameOffset = 0 } );
	}
	else if ( chance % 2 == 0 )
	{
		item.AddComponent<ItemComponent>( ItemCollectType::YELLOW_RUPEE );

		item.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hearts",
															 .width = 16,
															 .height = 16,
															 .layer = 1,
															 .bIsFixed = false,
															 .srcRect = SDL_Rect{ 48, 0, 16, 16 } } );

		item.AddComponent<AnimationComponent>( AnimationComponent{
			.numFrames = 2, .frameSpeedRate = 10, .vertical = false, .isLooped = true, .frameOffset = 48 } );
	}
}