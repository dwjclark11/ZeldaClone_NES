#include "BossStates.h"
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

#include "../Systems/SoundFXSystem.h"
#include "../Game/Game.h"


void BossIdleState::OnEnter(Entity& entity)
{
	
}

void BossIdleState::OnExit(Entity& entity) 
{
	
}

void BossIdleState::Update(Entity& entity) 
{
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& esm = ai.GetEnemyStateMachine();

	if (ai.GetBossType() == AIComponent::EnemyBossType::AQUAMENTUS)
	{
		auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();

		if (!projEmitter.shotTriggered)
		{
			if (rigid.velocity == glm::vec2(0))
			{
				rigid.velocity = glm::vec2(-50, 0);
				rigid.left = true;
			}

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
			esm.AddState(std::make_unique<BossPatrolState>());
			esm.ChangeState(entity);
		}
	}
	
}

void BossAttackState::OnEnter(class Entity& entity)
{
	
}

void BossAttackState::OnExit (class Entity& entity)
{
	
}

void BossAttackState::Update (class Entity& entity)
{
	
}


void BossPatrolState::OnEnter(class Entity& entity)
{
	walkTimer.Start();
}

void BossPatrolState::OnExit (class Entity& entity)
{
	walkTimer.Stop();
}

void BossPatrolState::Update (class Entity& entity)
{
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();

	if (walkTimer.GetTicks() > 2000)
	{
		if (rigid.down)
		{
			rigid.down = false;
			rigid.right = false;
			rigid.up = false;
			rigid.left = true;
		}
		else if (rigid.left)
		{
			rigid.down = false;
			rigid.right = false;
			rigid.up = true;
			rigid.left = false;

		}
		else if (rigid.right)
		{
			rigid.down = true;
			rigid.right = false;
			rigid.up = false;
			rigid.left = false;

		}
		else if (rigid.up)
		{
			rigid.down = false;
			rigid.right = true;
			rigid.up = false;
			rigid.left = false;
		}

		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance().GetAssetManager(), "boss_scream_1", 0, -1);

		ai.GetEnemyStateMachine().AddState(std::make_unique<BossIdleState>());
		ai.GetEnemyStateMachine().ChangeState(entity);
	}

}

void BossHurtState::OnEnter(class Entity& entity)
{
	
} 

void BossHurtState::OnExit (class Entity& entity) 
{
	
}

void BossHurtState::Update(class Entity& entity)  
{
	
}

void BossDeathState::OnEnter(class Entity& entity)
{
	
}

void BossDeathState::OnExit	(class Entity& entity)
{
	
}

void BossDeathState::Update (class Entity& entity)
{
	
}

void BossStunnedState::OnEnter(class Entity& entity)
{
	
}

void BossStunnedState::OnExit(class Entity& entity) 
{
	
}

void BossStunnedState::Update(class Entity& entity) 
{
	
}
