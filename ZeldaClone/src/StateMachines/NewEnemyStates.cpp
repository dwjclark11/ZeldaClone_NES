#include "NewEnemyStates.h"
#include "UpdateEnemies.h"
#include "../ECS/ECS.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/AIComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ItemComponent.h"
#include "../Components/RupeeGameComponent.h"
#include "../Components/GameComponent.h"

#include "../Game/Game.h"
#include "../Game/Player.h"

// IdleState
void EnemyIdleState::OnEnter(Entity& entity)
{
	//auto& ai = entity.GetComponent<AIComponent>();

	//if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
	//{
	//	auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	//	if (projEmitter.shotTriggered)
	//		projEmitter.timer.Start();
	//}
}

void EnemyIdleState::OnExit(Entity& entity)
{

}

void EnemyIdleState::Update(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();

	if (ai.IsABoss())
		return;

	switch (ai.GetEnemyType())
	{
	case AIComponent::EnemyType::OCTOROK:
	case AIComponent::EnemyType::MOBLIN:
		IdleUpdateOctoMoblin(entity);
		break;
	case AIComponent::EnemyType::DARKNUT:
		break;
	case AIComponent::EnemyType::LEEVER:
		IdleUpdateLever(entity);
		break;
	case AIComponent::EnemyType::TEKTITE:
		break;
	case AIComponent::EnemyType::PEAHAT:
		break;
	case AIComponent::EnemyType::ARMOS:
		break;
	case AIComponent::EnemyType::KEESE:
		break;
	case AIComponent::EnemyType::BLADE_TRAP:
		IdleUpdateBladeTrap(entity);
		return;
	case AIComponent::EnemyType::GEL:
		break;
	case AIComponent::EnemyType::STALFOS:
		break;
	case AIComponent::EnemyType::GORIYA:
		break;
	case AIComponent::EnemyType::NO_TYPE:
		__debugbreak();
		return;
	}
}

// AttackState
void EnemyAttackState::OnEnter(Entity& entity)
{
	//Logger::Err("entering Attack State");
}

void EnemyAttackState::Update(Entity& entity)
{
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();

}

void EnemyAttackState::OnExit(Entity& entity)
{
	//Logger::Err("Exiting Attack State");
}

// PatrolState
PatrolState::PatrolState()
{
	
	
}

void PatrolState::OnEnter(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();
	ai.aiTimer.Start();


	if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
	{
		auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		if (!projEmitter.shotTriggered)
		{
			projEmitter.timer.Start();
		}
	}
}
void PatrolState::OnExit(Entity& entity)
{
	//if (!entity.HasComponent<ProjectileEmitterComponent>())
	//	return;

	//auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	//projEmitter.timer.Stop();
}

void PatrolState::Update(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();

	if (ai.IsABoss())
		return;

	switch (ai.GetEnemyType())
	{
	case AIComponent::EnemyType::OCTOROK:
	case AIComponent::EnemyType::MOBLIN:
		PatrolUpdateOctoMoblin(entity);
		break;
	case AIComponent::EnemyType::DARKNUT:
		break;
	case AIComponent::EnemyType::LEEVER:
		PatrolUpdateLever(entity);
		break;
	case AIComponent::EnemyType::TEKTITE:
		break;
	case AIComponent::EnemyType::PEAHAT:
		break;
	case AIComponent::EnemyType::ARMOS:
		break;
	case AIComponent::EnemyType::KEESE:
		break;
	case AIComponent::EnemyType::BLADE_TRAP:
		PatrolUpdateBladeTrap(entity);
		return;
	case AIComponent::EnemyType::GEL:
		break;
	case AIComponent::EnemyType::STALFOS:
		break;
	case AIComponent::EnemyType::GORIYA:
		break;
	case AIComponent::EnemyType::NO_TYPE:
		__debugbreak();
		return;
	}
}

// HurtState
void HurtState::OnEnter(Entity& entity)
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();

	// Start hurt invinciblity timer
	enemyHealth.hurtTimer.Start();

	rigid.velocity = glm::vec2(0);
	animation.numFrames = 3;
	animation.frameOffset = 32;
	animation.frameSpeedRate = 20;
}
void HurtState::Update(Entity& entity)
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& esm = ai.GetEnemyStateMachine();

	if (enemyHealth.hurtTimer.GetTicks() > 1000 && enemyHealth.healthPercentage > 0)
	{
		enemyHealth.isHurt = false;
		enemyHealth.hurtTimer.Stop();

		if (!ai.stunTimer.isStarted())
		{
			esm.AddState(std::make_unique<EnemyIdleState>());
			esm.ChangeState(entity);
		}
		else
		{
			esm.AddState(std::make_unique<EnemyStunnedState>());
			esm.ChangeState(entity);
		}
			
	}
	// If the enemy's heath is <= 0, call death state!
	else if (enemyHealth.healthPercentage <= 0)
	{
		// Call the EnemyDeathState
		esm.AddState(std::make_unique<EnemyDeathState>());
		esm.ChangeState(entity);
	}
}

void HurtState::OnExit(Entity& entity)
{
	auto& animation = entity.GetComponent<AnimationComponent>();
	animation.numFrames = 2;
	animation.frameOffset = 0;
	animation.frameSpeedRate = 10;
}

// Death State
void EnemyDeathState::OnEnter(Entity& entity)
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
	animation.frameSpeedRate = 20;
	animation.isLooped = false;
	animation.vertical = false;

	srand(SDL_GetTicks());

	int chance = rand();
	if (chance % 2 == 0)
	{
		ItemDrop(entity);
	}

}
void EnemyDeathState::Update(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();

	if (ai.deathTimer.GetTicks() > 1000)
	{
		entity.Kill();
		ai.GarbageCollect(); // Delete the enemyStateMachine of this enemy!
	}
}

void EnemyDeathState::OnExit(Entity& entity)
{
	// Animation does not have to return to regular because the entity is destroyed!
}


void EnemyStunnedState::OnEnter(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();
	ai.stunTimer.Start();
}

void EnemyStunnedState::OnExit(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();
	ai.SetStunned(false);
}

void EnemyStunnedState::Update(Entity& entity)
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& esm = ai.GetEnemyStateMachine();

	if (ai.stunTimer.GetTicks() > 3000)
	{
		ai.stunTimer.Stop();
		esm.AddState(std::make_unique<EnemyIdleState>());
		esm.ChangeState(entity);
	}

	if (enemyHealth.isHurt)
	{
		esm.AddState(std::make_unique<HurtState>());
		esm.ChangeState(entity);
	}	
}

void EnemyDeathState::ItemDrop(Entity& enemy)
{
	const auto& enemyTransform = enemy.GetComponent<TransformComponent>();
	auto& pos = enemyTransform.position;
	int chance = rand();
	
	if (chance % 7 != 0 && chance % 5 != 0 && chance %3 != 0 && chance % 2 != 0)
		return;

	Entity item = Registry::Instance().CreateEntity();
	item.Group("items");
	item.AddComponent<TransformComponent>(pos, glm::vec2(4, 4));
	item.AddComponent<BoxColliderComponent>(16, 16);
	item.AddComponent<GameComponent>();

	if (chance % 7 == 0)
	{
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::BOMBS);
		item.AddComponent<SpriteComponent>("items", 16, 16, 1, false, 64, 112);

	}
	else if (chance % 5 == 0)
	{
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::BLUE_RUPEE);
		item.AddComponent<SpriteComponent>("hearts", 16, 16, 1, false, 48, 0);
		item.AddComponent<AnimationComponent>(2, 10, false, true, 48);

	}
	else if (chance % 3 == 0)
	{
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::HEARTS);
		item.AddComponent<SpriteComponent>("hearts", 16, 16, 1, false, 0, 0);
		item.AddComponent<AnimationComponent>(2, 10, false, true, 0);
	}
	else if (chance % 2 == 0)
	{
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::YELLOW_RUPEE);
		item.AddComponent<SpriteComponent>("hearts", 16, 16, 1, false, 48, 0);
		item.AddComponent<AnimationComponent>(2, 10, false, true, 48);
	}
}