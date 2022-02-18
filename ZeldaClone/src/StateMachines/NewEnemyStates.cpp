#include "NewEnemyStates.h"
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


// IdleState
void EnemyIdleState::OnEnter(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();

	if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
	{
		auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		if (projEmitter.shotTriggered)
			projEmitter.timer.Start();
	}
	else
	{
		ai.leeverTimer.Start();
	}

}
void EnemyIdleState::OnExit(Entity& entity)
{
	//Logger::Log("Leaving Idle State");
}

void EnemyIdleState::Update(Entity& entity)
{
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& esm = ai.GetEnemyStateMachine();

	if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
	{
		auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();

		if (projEmitter.timer.GetTicks() > 1000)
		{
			projEmitter.shotTriggered = false;
			projEmitter.shotFired = false;
			projEmitter.timer.Stop();
		}

		if (!projEmitter.shotTriggered)
		{
			if (rigid.down)
				rigid.velocity = glm::vec2(0, 50);

			if (rigid.up)
				rigid.velocity = glm::vec2(0, -50);

			if (rigid.right)
				rigid.velocity = glm::vec2(50, 0);

			if (rigid.left)
				rigid.velocity = glm::vec2(-50, 0);
		}
		if (rigid.velocity != glm::vec2(0, 0))
		{
			esm.AddState(std::make_unique<PatrolState>());
			esm.ChangeState(entity);
		}
	}
	else
	{
		rigid.velocity = glm::vec2(0);

		if (ai.leeverTimer.GetTicks() > 1500 && ai.leeverTimer.GetTicks() < 1600)
		{
			sprite.srcRect.y = 16;
		}

		if (ai.leeverTimer.GetTicks() > 2000 && ai.leeverTimer.GetTicks() < 2100)
		{
			sprite.srcRect.y = 32;
		}

		if (ai.leeverTimer.GetTicks() > 2500 && ai.leeverTimer.GetTicks() < 2600)
		{
			sprite.srcRect.y = 48;
		}
		if (ai.leeverTimer.GetTicks() > 3000 && ai.leeverTimer.GetTicks() < 3100)
		{
			sprite.srcRect.y = 64;
			animation.numFrames = 2;
			animation.frameSpeedRate = 12;
			animation.frameOffset = 64;
			rigid.velocity = glm::vec2(50, 0);
			esm.AddState(std::make_unique<PatrolState>());
			esm.ChangeState(entity);
		}
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
void PatrolState::OnEnter(Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();
	ai.aiTimer.Start();
}
void PatrolState::OnExit(Entity& entity)
{

}

void PatrolState::Update(Entity& entity)
{
	auto player = Registry::Instance().GetEntityByTag("player");
	auto& playerTransform = player.GetComponent<TransformComponent>();

	auto& transform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& esm = ai.GetEnemyStateMachine();

	int sign = 1;

	if (entity.HasComponent<ProjectileEmitterComponent>())
	{
		auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();

		if (!projectileEmitter.shootDown && rigid.down && playerTransform.position.x < transform.position.x + 32
			&& playerTransform.position.x > transform.position.x - 32
			&& playerTransform.position.y < transform.position.y + 256
			&& playerTransform.position.y > transform.position.y + 64)
		{
			rigid = glm::vec2(0, 0);
			projectileEmitter.shootDown = true;
			projectileEmitter.shotTriggered = true;

		}
		else if (!projectileEmitter.shootUp && rigid.up && playerTransform.position.x < transform.position.x + 32
			&& playerTransform.position.x > transform.position.x - 32
			&& playerTransform.position.y < transform.position.y - 64
			&& playerTransform.position.y > transform.position.y - 256)
		{
			rigid = glm::vec2(0, 0);
			projectileEmitter.shootUp = true;
			projectileEmitter.shotTriggered = true;
		}
		else if (!projectileEmitter.shootRight && rigid.right && playerTransform.position.x < transform.position.x + 256
			&& playerTransform.position.x > transform.position.x + 64
			&& playerTransform.position.y < transform.position.y + 32
			&& playerTransform.position.y > transform.position.y - 32)
		{
			rigid = glm::vec2(0, 0);
			projectileEmitter.shootRight = true;
			projectileEmitter.shotTriggered = true;
		}
		else if (!projectileEmitter.shootLeft && rigid.left && playerTransform.position.x > transform.position.x - 256
			&& playerTransform.position.x < transform.position.x - 64
			&& playerTransform.position.y < transform.position.y + 32
			&& playerTransform.position.y > transform.position.y - 32)
		{
			rigid = glm::vec2(0, 0);
			projectileEmitter.shootLeft = true;
			projectileEmitter.shotTriggered = true;
		}

		if (projectileEmitter.shootDown || projectileEmitter.shootUp || projectileEmitter.shootLeft || projectileEmitter.shootRight)
		{
			esm.AddState(std::make_unique<EnemyIdleState>());
			esm.ChangeState(entity);
		}
	}

	if (ai.aiTimer.GetTicks() > 2000)
	{
		srand(time(NULL));
		int num = rand() % 2 + 1;


		// Set the direction of the enemy randomly
		if (num > 1)
			sign = 1;
		else
			sign = -1;

		if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
		{
			if (rigid.down)
				rigid.velocity = glm::vec2(sign * 50, 0);
			else if (rigid.up)
				rigid.velocity = glm::vec2(sign * 50, 0);
			else if (rigid.left)
				rigid.velocity = glm::vec2(0, sign * 50);
			else if (rigid.right)
				rigid.velocity = glm::vec2(0, sign * 50);
		}
		else
		{
			if (ai.leeverTimer.GetTicks() > 3000 && ai.leeverTimer.GetTicks() < 10000)
			{
				if (rigid.down)
					rigid.velocity = glm::vec2(sign * 50, 0);
				else if (rigid.up)
					rigid.velocity = glm::vec2(sign * 50, 0);
				else if (rigid.left)
					rigid.velocity = glm::vec2(0, sign * 50);
				else if (rigid.right)
					rigid.velocity = glm::vec2(0, sign * 50);
			}
		}

		ai.aiTimer.Stop();
	}

	if (!ai.aiTimer.isStarted())
	{
		ai.aiTimer.Start();
	}

	if (enemyHealth.isHurt)
	{
		esm.AddState(std::make_unique<HurtState>());
		esm.ChangeState(entity);
	}
	else if (rigid.velocity == glm::vec2(0) && ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
	{
		esm.AddState(std::make_unique<EnemyIdleState>());
		esm.ChangeState(entity);
	}
	
	if (ai.GetStunned())
	{
		esm.AddState(std::make_unique<EnemyStunnedState>());
		esm.ChangeState(entity);
	}
		
	if (ai.GetEnemyType() == AIComponent::EnemyType::LEEVER)
	{
		if (ai.leeverTimer.GetTicks() > 10000 && ai.leeverTimer.GetTicks() < 10100)
		{
			rigid.velocity = glm::vec2(0);
			sprite.srcRect.y = 96;
		}
		if (ai.leeverTimer.GetTicks() > 10500 && ai.leeverTimer.GetTicks() < 10600)
			sprite.srcRect.y = 112;

		if (ai.leeverTimer.GetTicks() > 11000 && ai.leeverTimer.GetTicks() < 11100)
			sprite.srcRect.y = 128;

		if (ai.leeverTimer.GetTicks() > 11500 && ai.leeverTimer.GetTicks() < 11600)
		{
			sprite.srcRect.y = 0;

			animation.numFrames = 1;
			animation.frameSpeedRate = 1;
			animation.frameOffset = 0;

			ai.leeverTimer.Stop();

			esm.AddState(std::make_unique<EnemyIdleState>());
			esm.ChangeState(entity);
		}
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
	Timer time;
	time.Start();
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

	if (ai.deathTimer.GetTicks() > 500)
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
	
	if (chance % 7 == 0)
	{
		Entity item = Registry::Instance().CreateEntity();
		item.Group("items");
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::BOMBS);
		item.AddComponent<SpriteComponent>("items", 16, 16, 1, false, 64, 112);
		item.AddComponent<TransformComponent>(pos, glm::vec2(4, 4));
		item.AddComponent<BoxColliderComponent>(16, 16);
		item.AddComponent<GameComponent>();
	}
	else if (chance % 5 == 0)
	{
		Entity item = Registry::Instance().CreateEntity();
		item.Group("items");
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::BLUE_RUPEE);
		item.AddComponent<SpriteComponent>("hearts", 16, 16, 1, false, 48, 0);
		item.AddComponent<TransformComponent>(pos, glm::vec2(4, 4));
		item.AddComponent<AnimationComponent>(2, 10, false, true, 48);
		//item.AddComponent<RupeeTypeComponent>(RupeeType::BLUE);
		item.AddComponent<BoxColliderComponent>(16, 16);
		item.AddComponent<GameComponent>();
	}
	else if (chance % 3 == 0)
	{
		Entity item = Registry::Instance().CreateEntity();
		item.Group("items");
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::HEARTS);
		item.AddComponent<SpriteComponent>("hearts", 16, 16, 1, false, 0, 0);
		item.AddComponent<TransformComponent>(pos, glm::vec2(4, 4));
		item.AddComponent<AnimationComponent>(2, 10, false, true, 0);
		item.AddComponent<BoxColliderComponent>(16, 16);
		item.AddComponent<GameComponent>();
	}
	else if (chance % 2 == 0)
	{
		Entity item = Registry::Instance().CreateEntity();
		item.Group("items");
		item.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::YELLOW_RUPEE);
		item.AddComponent<SpriteComponent>("hearts", 16, 16, 1, false, 48, 0);
		item.AddComponent<TransformComponent>(pos, glm::vec2(4, 4));
		item.AddComponent<AnimationComponent>(2, 10, false, true, 48);
		//item.AddComponent<RupeeTypeComponent>(RupeeType::YELLOW);
		item.AddComponent<BoxColliderComponent>(16, 16);
		item.AddComponent<GameComponent>();
	}
}