#include "BossStates.h"
#include "../ECS/ECS.h"
#include "../Utilities/GameData.h"
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
#include "../Game/Player.h"
#include "../Components/TriggerBoxComponent.h"

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
		
		//if (rigid.velocity == glm::vec2(0))
		//{
		//	rigid.velocity = glm::vec2(-50, 0);
		//	rigid.dir = RigidBodyComponent::Dir::LEFT;
		//}

		switch (rigid.dir)
		{
		case RigidBodyComponent::Dir::UP:
			rigid.velocity = glm::vec2(0, -50);
			break;
		case RigidBodyComponent::Dir::RIGHT:
			rigid.velocity = glm::vec2(50, 0);
			break; 
		case RigidBodyComponent::Dir::DOWN:
			rigid.velocity = glm::vec2(0, 50);
			break;
		case RigidBodyComponent::Dir::LEFT:
			rigid.velocity = glm::vec2(-50, 0);
			break;
		default:
			break;
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
	const auto& player = Game::Instance().GetPlayer();
	bool transition = false;

	const auto& playerEnt = player->GetPlayer();
	auto& playerTransform = playerEnt.GetComponent<TransformComponent>();

	auto& transform = entity.GetComponent<TransformComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& ai = entity.GetComponent<AIComponent>();
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& esm = ai.GetEnemyStateMachine();
	
	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();

	if (walkTimer.GetTicks() > 1000)
	{
		if (rigid.dir == RigidBodyComponent::Dir::LEFT)
		{
			rigid.dir = RigidBodyComponent::Dir::RIGHT;
			transition = true;
		}
		else if (rigid.dir == RigidBodyComponent::Dir::RIGHT)
		{
			rigid.dir = RigidBodyComponent::Dir::LEFT;
			transition = true;
		}
	}
	
	if (playerTransform.position.x > transform.position.x - 400 && playerTransform.position.x < transform.position.x + 400 &&
		playerTransform.position.y > transform.position.y - 400 && playerTransform.position.y < transform.position.y + 400 && !projectileEmitter.timer.isStarted())
	{
		projectileEmitter.shotTriggered = true;
		projectileEmitter.timer.Start();
	}

	// Check to see if the shot timer has started and passed the given ticks
	if (projectileEmitter.timer.GetTicks() > 1000 && projectileEmitter.shotTriggered)
	{
		projectileEmitter.timer.Stop();
		projectileEmitter.shotTriggered = false;
		projectileEmitter.shotFired = false;
	}

	if (transition)
	{
		Game::Instance().GetSoundPlayer().PlaySoundFX("boss_scream_1", 0, SoundChannel::ANY);

		ai.GetEnemyStateMachine().AddState(std::make_unique<BossIdleState>());
		ai.GetEnemyStateMachine().ChangeState(entity);
	}

	if (enemyHealth.isHurt)
	{
		esm.AddState(std::make_unique<BossHurtState>());
		esm.ChangeState(entity);
	}
}

void BossHurtState::OnEnter(class Entity& entity)
{
	auto& enemyHealth = entity.GetComponent<HealthComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();
	auto& rigid = entity.GetComponent<RigidBodyComponent>();
	auto& sprite = entity.GetComponent<SpriteComponent>();
	// Start hurt invinciblity timer
	enemyHealth.hurtTimer.Start();

	rigid.velocity = glm::vec2(0);

	// Set the Sprite Src rect x to the first column for hurt animation!
	sprite.srcRect.x = 0;

	// Adjust the animation to the hurt state parameters
	animation.numFrames = 4;
	animation.frameOffset = 0;
	animation.frameSpeedRate = 20;
	animation.vertical = true;

	Game::Instance().GetSoundPlayer().PlaySoundFX("boss_hit", 0, SoundChannel::ANY);

} 

void BossHurtState::OnExit (class Entity& entity) 
{
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& animation = entity.GetComponent<AnimationComponent>();

	// Reset the animation back to orignal
	animation.vertical = false;
	animation.frameSpeedRate = 10;

	// Reset the Sprite Src Rect to [0,0]
	sprite.srcRect.x = 0;
	sprite.srcRect.y = 0;
}

void BossHurtState::Update(class Entity& entity)  
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
			esm.AddState(std::make_unique<BossIdleState>());
			esm.ChangeState(entity);
		}
		else
		{
			esm.AddState(std::make_unique<BossStunnedState>());
			esm.ChangeState(entity);
		}

	}
	// If the enemy's heath is <= 0, call death state!
	else if (enemyHealth.healthPercentage <= 0)
	{
		// Call the EnemyDeathState
		esm.AddState(std::make_unique<BossDeathState>());
		esm.ChangeState(entity);
	}
}

void BossDeathState::OnEnter(class Entity& entity)
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

	// Drop a new Full Heart // TODO: Add location --> For now just fall where the boss is
	DropHeart(entity);

	// Get the trap door entity
	if (Registry::Instance().DoesTagExist("level1Door"))
	{
		auto trapDoor = Registry::Instance().GetEntityByTag("level1Door");
		// Play door open sound
		Game::Instance().GetSoundPlayer().PlaySoundFX("door_unlock", 0, SoundChannel::ANY);
		trapDoor.Kill();
	}

}

void BossDeathState::OnExit	(class Entity& entity)
{
	
}

void BossDeathState::Update (class Entity& entity)
{
	auto& ai = entity.GetComponent<AIComponent>();

	if (ai.deathTimer.GetTicks() > 500)
	{

		entity.Kill();
		ai.GarbageCollect(); // Delete the enemyStateMachine of this enemy!
	}
}

void BossDeathState::DropHeart(Entity& entity)
{
	const auto& enemyTransform = entity.GetComponent<TransformComponent>();
	auto& pos = enemyTransform.position;

	Entity fullHeart = Registry::Instance().CreateEntity();
	fullHeart.Group("trigger");
	fullHeart.AddComponent<TransformComponent>(pos, glm::vec2(4, 4));
	fullHeart.AddComponent<SpriteComponent>("full_heart", 16, 16, 1, false, 0, 0);
	fullHeart.AddComponent<ItemComponent>(ItemComponent::ItemCollectType::DEFAULT, ItemComponent::SpecialItemType::FULL_HEART);
	fullHeart.AddComponent<BoxColliderComponent>(16, 16);
	fullHeart.AddComponent<TriggerBoxComponent>(TriggerBoxComponent::TriggerType::COLLECT_ITEM);
	fullHeart.AddComponent<GameComponent>();
	Game::Instance().GetSoundPlayer().PlaySoundFX("special_item", 0, SoundChannel::ANY);
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
