#include "StateMachines/UpdateEnemies.h"
#include "StateMachines/NewEnemyStates.h"
#include "ECS/ECS.h"
#include "Logger/Logger.h"
#include "Components/AIComponent.h"
#include "Components/HealthComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ProjectileEmitterComponent.h"

#include "Game/Game.h"
#include "Game/Player.h"

#include <glm/glm.hpp>

void IdleUpdateOctoMoblin( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();
	auto& enemyHealth = entity.GetComponent<HealthComponent>();

	if ( enemyHealth.bIsHurt )
	{
		ai.esm->AddState( std::make_unique<HurtState>() );
		ai.esm->ChangeState( entity );
	}
	else if ( ai.bStunned )
	{
		ai.esm->AddState( std::make_unique<EnemyStunnedState>() );
		ai.esm->ChangeState( entity );
	}

	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();

	auto p_RNG = std::make_unique<RNG>();
	int num = static_cast<int>( std::round( p_RNG->Float() * 100 ) ) % 4 + 1;

	auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();

	if ( projEmitter.timer.GetTicks() > 2500 )
	{
		projEmitter.bShotTriggered = false;
		projEmitter.bShotFired = false;
		projEmitter.timer.Stop();
	}

	if ( !projEmitter.bShotTriggered )
	{
		if ( num == 1 )
			rigid.velocity = glm::vec2( 0, 150 );
		else if ( num == 2 )
			rigid.velocity = glm::vec2( 0, -150 );
		else if ( num == 3 )
			rigid.velocity = glm::vec2( 150, 0 );
		else if ( num == 1 )
			rigid.velocity = glm::vec2( -150, 0 );
	}

	if ( rigid.velocity != glm::vec2( 0, 0 ) )
	{
		ai.esm->AddState( std::make_unique<PatrolState>() );
		ai.esm->ChangeState( entity );
	}
}

void IdleUpdateBladeTrap( Entity& entity )
{
	// Get the player attributes
	const auto& player = Game::Instance().GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	const auto& player_transform = playerEnt.GetComponent<TransformComponent>();
	const auto& player_collider = playerEnt.GetComponent<BoxColliderComponent>();

	// Get the entity attributes
	auto& ai = entity.GetComponent<AIComponent>();
	const auto& enemy = entity.GetComponent<EnemyComponent>();
	const auto& enemy_transform = entity.GetComponent<TransformComponent>();
	const auto& enemy_collider = entity.GetComponent<BoxColliderComponent>();

	bool attack = false;

	switch ( enemy.moveDir )
	{
	case MoveDir::UP:
		if ( ( player_transform.position.x + player_collider.offset.x +
			   ( player_collider.width * player_transform.scale.x ) ) >= enemy_transform.position.x &&
			 ( player_transform.position.x + player_collider.offset.x ) <=
				 ( enemy_transform.position.x + ( enemy_collider.width * enemy_transform.scale.x ) ) &&
			 player_transform.position.y + player_collider.offset.y <= enemy_transform.position.y &&
			 player_transform.position.y + ( player_collider.height * player_transform.scale.y ) >=
				 enemy_transform.position.y - enemy.maxMoveDistance )
		{
			attack = true;
		}
		break;
	case MoveDir::RIGHT:
		if ( ( player_transform.position.x + player_collider.offset.x ) >=
				 ( enemy_transform.position.x + ( enemy_collider.width * enemy_transform.scale.x ) ) &&
			 ( player_transform.position.x + player_collider.offset.x ) <=
				 ( enemy_transform.position.x + ( enemy_collider.width * enemy_transform.scale.x ) +
				   enemy.maxMoveDistance ) &&
			 player_transform.position.y + ( player_collider.height * player_transform.scale.y ) +
					 player_collider.offset.y >=
				 enemy_transform.position.y &&
			 player_transform.position.y + player_collider.offset.y <=
				 enemy_transform.position.y + ( enemy_collider.height * enemy_transform.scale.y ) )
		{
			attack = true;
		}
		break;
	case MoveDir::DOWN:
		if ( ( player_transform.position.x + player_collider.offset.x +
			   ( player_collider.width * player_transform.scale.x ) ) >= enemy_transform.position.x &&
			 ( player_transform.position.x + player_collider.offset.x ) <=
				 ( enemy_transform.position.x + ( enemy_collider.width * enemy_transform.scale.x ) ) &&
			 player_transform.position.y >= enemy_transform.position.y &&
			 player_transform.position.y <= enemy_transform.position.y +
												( enemy_collider.height * enemy_transform.scale.y ) +
												enemy.maxMoveDistance )
		{
			attack = true;
		}
		break;
	case MoveDir::LEFT:
		if ( ( player_transform.position.x + player_collider.offset.x ) <= ( enemy_transform.position.x ) &&
			 ( player_transform.position.x + player_collider.offset.x +
			   ( player_collider.width * player_transform.scale.x ) ) >=
				 ( enemy_transform.position.x - enemy.maxMoveDistance ) &&
			 player_transform.position.y + ( player_collider.height * player_transform.scale.y ) +
					 player_collider.offset.y >=
				 enemy_transform.position.y &&
			 player_transform.position.y + player_collider.offset.y <=
				 enemy_transform.position.y + ( enemy_collider.height * enemy_transform.scale.y ) )
		{
			attack = true;
		}
		break;
	case MoveDir::NO_MOVE: return;
	}

	if ( attack )
	{
		ai.esm->AddState( std::make_unique<PatrolState>() );
		ai.esm->ChangeState( entity );
	}
}

void IdleUpdateLever( Entity& entity )
{
	auto& ai = entity.GetComponent<AIComponent>();
	auto& enemyHealth = entity.GetComponent<HealthComponent>();

	if ( enemyHealth.bIsHurt )
	{
		ai.esm->AddState( std::make_unique<HurtState>() );
		ai.esm->ChangeState( entity );
	}
	else if ( ai.bStunned )
	{
		ai.esm->AddState( std::make_unique<EnemyStunnedState>() );
		ai.esm->ChangeState( entity );
	}

	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();

	rigid.velocity = glm::vec2( 0 );

	if ( !ai.leeverTimer.isStarted() )
		ai.leeverTimer.Start();

	if ( ai.leeverTimer.GetTicks() > 1500 && ai.leeverTimer.GetTicks() < 1600 )
	{
		sprite.srcRect.y = 16;
	}

	if ( ai.leeverTimer.GetTicks() > 2000 && ai.leeverTimer.GetTicks() < 2100 )
	{
		sprite.srcRect.y = 32;
	}

	if ( ai.leeverTimer.GetTicks() > 2500 && ai.leeverTimer.GetTicks() < 2600 )
	{
		sprite.srcRect.y = 48;
	}
	if ( ai.leeverTimer.GetTicks() > 3000 && ai.leeverTimer.GetTicks() < 3100 )
	{
		sprite.srcRect.y = 64;
		animation.numFrames = 2;
		animation.frameSpeedRate = 12;
		animation.frameOffset = 64;
		rigid.velocity = glm::vec2( 50, 0 );
		ai.esm->AddState( std::make_unique<PatrolState>() );
		ai.esm->ChangeState( entity );
	}
}

void PatrolUpdateOctoMoblin( Entity& entity )
{
	const auto& player = Game::Instance().GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& playerTransform = playerEnt.GetComponent<TransformComponent>();

	auto& transform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();

	if ( enemyHealth.bIsHurt )
	{
		ai.esm->AddState( std::make_unique<HurtState>() );
		ai.esm->ChangeState( entity );
	}
	else if ( rigid.velocity == glm::vec2( 0 ) && ai.enemyType != EnemyType::LEEVER )
	{
		ai.esm->AddState( std::make_unique<EnemyIdleState>() );
		ai.esm->ChangeState( entity );
	}

	if ( ai.bStunned )
	{
		ai.esm->AddState( std::make_unique<EnemyStunnedState>() );
		ai.esm->ChangeState( entity );
	}

	int sign = 1;

	auto p_RNG = std::make_unique<RNG>();
	int num = static_cast<int>( std::round( p_RNG->Float() * 100 ) ) % 4 + 1;

	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();

	if ( projectileEmitter.timer.GetTicks() > ( 1500 * num ) )
	{
		if ( !projectileEmitter.bShotTriggered )
		{
			rigid.velocity = glm::vec2( 0, 0 );
			projectileEmitter.bShotTriggered = true;
		}
		else
		{
			ai.esm->AddState( std::make_unique<EnemyIdleState>() );
			ai.esm->ChangeState( entity );
		}
	}

	if ( ai.aiTimer.GetTicks() > 1000 )
	{
		// Set the direction of the enemy randomly
		sign = num % 2 == 0 ? 1 : -1;

		if ( ai.enemyType != EnemyType::LEEVER )
		{
			if ( num == 1 )
				rigid.velocity = glm::vec2( sign * 150, 0 );
			else if ( num == 2 )
				rigid.velocity = glm::vec2( sign * 150, 0 );
			else if ( num == 3 )
				rigid.velocity = glm::vec2( 0, sign * 150 );
			else if ( num == 4 )
				rigid.velocity = glm::vec2( 0, sign * 150 );
		}

		ai.aiTimer.Stop();
	}

	if ( !ai.aiTimer.isStarted() )
	{
		ai.aiTimer.Start();
	}
}

void PatrolUpdateBladeTrap( Entity& entity )
{
	auto& transform = entity.GetComponent<TransformComponent>();
	auto& rigid_body = entity.GetComponent<RigidBodyComponent>();
	auto& enemy = entity.GetComponent<EnemyComponent>();
	auto& delta_time = Game::Instance().GetDeltaTime();
	auto& ai = entity.GetComponent<AIComponent>();

	bool returned{ false };

	switch ( enemy.moveDir )
	{
	case MoveDir::UP:
		if ( !enemy.maxMoveReached )
		{
			if ( transform.position.y > enemy.startPos.y - enemy.maxMoveDistance )
				transform.position.y += rigid_body.velocity.y * delta_time * 3 * -1;
			else
				enemy.maxMoveReached = true;
		}
		else
		{
			if ( transform.position.y < enemy.startPos.y )
				transform.position.y += rigid_body.velocity.y * delta_time;
			else
			{
				enemy.maxMoveReached = false;
				returned = true;
			}
		}
		break;

	case MoveDir::RIGHT:
		if ( !enemy.maxMoveReached )
		{
			if ( transform.position.x < enemy.startPos.x + enemy.maxMoveDistance )
				transform.position.x += rigid_body.velocity.x * delta_time * 3;
			else
				enemy.maxMoveReached = true;
		}
		else
		{
			if ( transform.position.x > enemy.startPos.x )
				transform.position.x += rigid_body.velocity.x * delta_time * -1;
			else
			{
				enemy.maxMoveReached = false;
				returned = true;
			}
		}
		break;

	case MoveDir::DOWN:
		if ( !enemy.maxMoveReached )
		{
			if ( transform.position.y < enemy.startPos.y + enemy.maxMoveDistance )
				transform.position.y += rigid_body.velocity.y * delta_time * 3;
			else
				enemy.maxMoveReached = true;
		}
		else
		{
			if ( transform.position.y > enemy.startPos.y )
				transform.position.y += rigid_body.velocity.y * delta_time * -1;
			else
			{
				enemy.maxMoveReached = false;
				returned = true;
			}
		}
		break;

	case MoveDir::LEFT:
		if ( !enemy.maxMoveReached )
		{
			if ( transform.position.x > enemy.startPos.x - enemy.maxMoveDistance )
				transform.position.x += rigid_body.velocity.x * delta_time * 3 * -1;
			else
				enemy.maxMoveReached = true;
		}
		else
		{
			if ( transform.position.x < enemy.startPos.x )
				transform.position.x += rigid_body.velocity.x * delta_time;
			else
			{
				enemy.maxMoveReached = false;
				returned = true;
			}
		}

		break;

	case MoveDir::NO_MOVE: return;
	}

	if ( returned )
	{
		ai.esm->AddState( std::make_unique<EnemyIdleState>() );
		ai.esm->ChangeState( entity );
	}
}

void PatrolUpdateLever( Entity& entity )
{

	const auto& player = Game::Instance().GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& playerTransform = playerEnt.GetComponent<TransformComponent>();

	auto& transform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();

	if ( enemyHealth.bIsHurt )
	{
		ai.esm->AddState( std::make_unique<HurtState>() );
		ai.esm->ChangeState( entity );
	}
	else if ( rigid.velocity == glm::vec2( 0 ) && ai.enemyType != EnemyType::LEEVER )
	{
		ai.esm->AddState( std::make_unique<EnemyIdleState>() );
		ai.esm->ChangeState( entity );
	}

	if ( ai.bStunned )
	{
		ai.esm->AddState( std::make_unique<EnemyStunnedState>() );
		ai.esm->ChangeState( entity );
	}

	int sign = 1;

	srand( SDL_GetTicks() * sin( 75 ) );
	auto p_RNG = std::make_unique<RNG>();
	int num = static_cast<int>( std::round( p_RNG->Float() * 100 ) ) % 4 + 1;
	if ( ai.leeverTimer.GetTicks() > 3000 && ai.leeverTimer.GetTicks() < 10000 )
	{
		switch ( rigid.dir )
		{
		case RigidBodyDir::UP: rigid.velocity = glm::vec2( sign * 50, 0 ); break;
		case RigidBodyDir::RIGHT: rigid.velocity = glm::vec2( 0, sign * 50 ); break;
		case RigidBodyDir::DOWN: rigid.velocity = glm::vec2( sign * 50, 0 ); break;
		case RigidBodyDir::LEFT: rigid.velocity = glm::vec2( 0, sign * 50 ); break;
		}
	}

	if ( !ai.aiTimer.isStarted() )
	{
		ai.aiTimer.Start();
	}

	if ( ai.enemyType == EnemyType::LEEVER )
	{
		if ( ai.leeverTimer.GetTicks() > 10000 && ai.leeverTimer.GetTicks() < 10100 )
		{
			rigid.velocity = glm::vec2( 0 );
			sprite.srcRect.y = 96;
		}
		if ( ai.leeverTimer.GetTicks() > 10500 && ai.leeverTimer.GetTicks() < 10600 )
			sprite.srcRect.y = 112;

		if ( ai.leeverTimer.GetTicks() > 11000 && ai.leeverTimer.GetTicks() < 11100 )
			sprite.srcRect.y = 128;

		if ( ai.leeverTimer.GetTicks() > 11500 && ai.leeverTimer.GetTicks() < 11600 )
		{
			sprite.srcRect.y = 0;

			animation.numFrames = 1;
			animation.frameSpeedRate = 1;
			animation.frameOffset = 0;

			ai.leeverTimer.Stop();

			ai.esm->AddState( std::make_unique<EnemyIdleState>() );
			ai.esm->ChangeState( entity );
		}
	}
}