#include "EnemyStates.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/AIComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"

// IdleState
void EnemyIdleState::OnEnter(EnemyStateMachine* pOwner, Entity& entity)
{
	auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	if (projEmitter.shotTriggered)
		projEmitter.timer.Start();
}
void EnemyIdleState::OnExit(EnemyStateMachine* pOwner, Entity& entity)
{
	//Logger::Log("Leaving Idle State");
}
void EnemyIdleState::Execute(EnemyStateMachine* pOwner, Entity& entity)
{
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
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
		{
			rigid.velocity = glm::vec2(0, 50);
			//Logger::Log("DOWN");
		}
			
		if (rigid.up)
		{
			//Logger::Log("UP");
			rigid.velocity = glm::vec2(0, -50);
		}
			
		if (rigid.right)
		{
			//Logger::Log("RIGHT");
			rigid.velocity = glm::vec2(50, 0);
		}
			
		if (rigid.left)
		{
			//Logger::Log("LEFT");
			rigid.velocity = glm::vec2(-50, 0);
		}
			
	}
	if (rigid.velocity != glm::vec2(0, 0))
	{
		//Logger::Err("Enemy is in Patrol State");
		pOwner->ChangeState(pOwner->patrolState, entity);
	}
}

// AttackState
void EnemyAttackState::OnEnter(EnemyStateMachine* pOwner, Entity& entity)
{
	//Logger::Err("entering Attack State");
}

void EnemyAttackState::Execute(EnemyStateMachine* pOwner, Entity& entity)
{
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& projEmitter = entity.GetComponent<ProjectileEmitterComponent>();
	
}

void EnemyAttackState::OnExit(EnemyStateMachine* pOwner, Entity& entity)
{

}

// PatrolState
void PatrolState::OnEnter(EnemyStateMachine* pOwner, Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();

	ai.aiTimer.Start();
}
void PatrolState::OnExit(EnemyStateMachine* pOwner, Entity& entity) 
{
	
}

void PatrolState::Execute(EnemyStateMachine* pOwner, Entity& entity)
{
	auto player = Registry::Instance()->GetEntityByTag("player");
	auto& transform = entity.GetComponent<TransformComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	
	if (entity.HasComponent<ProjectileEmitterComponent>())
	{
		auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		
		if (!projectileEmitter.shootDown && rigid.down && playerTransform.position.x < transform.position.x + 32 && playerTransform.position.x > transform.position.x - 32
			&& playerTransform.position.y < transform.position.y + 256 && playerTransform.position.y > transform.position.y + 64)
		{
			//Logger::Err("Shoot Player Down");
			rigid = glm::vec2(0,0);
			projectileEmitter.shootDown = true;
			projectileEmitter.shotTriggered = true;
			
		}
		else if (!projectileEmitter.shootUp && rigid.up && playerTransform.position.x < transform.position.x + 32 && playerTransform.position.x > transform.position.x - 32
			&& playerTransform.position.y < transform.position.y - 64 && playerTransform.position.y > transform.position.y  - 256)
		{
			//Logger::Err("Shoot Player Up!");
			
			rigid = glm::vec2(0,0);
			projectileEmitter.shootUp = true;
			projectileEmitter.shotTriggered = true;
		}
		else if (!projectileEmitter.shootRight && rigid.right && playerTransform.position.x < transform.position.x + 256 && playerTransform.position.x > transform.position.x + 64
			&& playerTransform.position.y < transform.position.y + 32 && playerTransform.position.y > transform.position.y  -32)
		{
			//Logger::Err("Shoot Player Right!");
			rigid = glm::vec2(0,0);
			projectileEmitter.shootRight = true;
			projectileEmitter.shotTriggered = true;
		}
		else if (!projectileEmitter.shootLeft && rigid.left && playerTransform.position.x > transform.position.x - 256 && playerTransform.position.x < transform.position.x - 64
			&& playerTransform.position.y < transform.position.y + 32 && playerTransform.position.y > transform.position.y - 32)
		{
			//Logger::Err("Shoot Player Left!");
			rigid = glm::vec2(0,0);
			projectileEmitter.shootLeft = true;
			projectileEmitter.shotTriggered = true;
		}
		
		if (projectileEmitter.shootDown || projectileEmitter.shootUp || projectileEmitter.shootLeft || projectileEmitter.shootRight)
		{
			//Logger::Err("Enemy has fired a projectile now is idle");
			pOwner->ChangeState(pOwner->idleState, entity);
		}
		
		
		if (ai.aiTimer.GetTicks() > 2000)
		{
			srand(time(NULL));
			int num = rand() % 2 + 1;
			int sign = 1;

			// Set the direction of the enemy randomly
			if (num > 1)
				sign = 1;
			else
				sign = -1;
			
			if (rigid.down)
				rigid.velocity = glm::vec2(sign*50, 0);
			else if (rigid.up)
				rigid.velocity = glm::vec2(sign*50, 0);
			else if (rigid.left)
				rigid.velocity = glm::vec2(0, sign*50);
			else if (rigid.right)
				rigid.velocity = glm::vec2(0, sign*50);

			ai.aiTimer.Stop();
		}

		if (!ai.aiTimer.isStarted())
		{
			ai.aiTimer.Start();
		}
		
		if (enemyHealth.isHurt)
			pOwner->ChangeState(pOwner->hurtState, entity);
		else if (rigid.velocity == glm::vec2(0))
			pOwner->ChangeState(pOwner->idleState, entity);
	}
}


// HurtState
void HurtState::OnEnter(EnemyStateMachine* pOwner, Entity& entity)
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();

	// Start hurt invinciblity timer
	enemyHealth.hurtTimer.Start();
	
	rigid.velocity = glm::vec2(0);
	animation.numFrames = 3;
	animation.frameOffset = 32;
}
void HurtState::Execute(EnemyStateMachine* pOwner, Entity& entity)
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& ai = entity.GetComponent<AIComponent>();


	if (enemyHealth.hurtTimer.GetTicks() > 1000 && enemyHealth.healthPercentage > 0)
	{
		enemyHealth.isHurt = false;
		enemyHealth.hurtTimer.Stop();
		pOwner->ChangeState(pOwner->idleState, entity);
	}
	else if (enemyHealth.healthPercentage <= 0)
	{
		ai.GarbageCollect();
		entity.Kill();
	}
}

void HurtState::OnExit(EnemyStateMachine* pOwner, Entity& entity)
{
	auto& animation = entity.GetComponent<AnimationComponent>();
	animation.numFrames = 2;
	animation.frameOffset = 0;
	//Logger::Err("Enemy Leaving HURT state");
}
