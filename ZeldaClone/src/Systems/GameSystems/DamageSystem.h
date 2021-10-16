#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/ProjectileComponent.h"
#include "../../Components/ProjectileEmitterComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../Game/Game.h"
#include "../SoundFXSystem.h"

class DamageSystem : public System
{
private: 
	bool dead;

public:
	
	DamageSystem()
	{
		dead = false;
		RequiredComponent<BoxColliderComponent>();
		RequiredComponent<HealthComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& event)
	{
		Entity a = event.a;
		Entity b = event.b;

		if (a.BelongsToGroup("projectile") && b.HasTag("player")) OnPlayerHitsProjectile(a, b);
		if (b.BelongsToGroup("projectile") && a.HasTag("player")) OnPlayerHitsProjectile(b, a);
		if (a.BelongsToGroup("enemies") && b.HasTag("player")) OnEnemyHitsPlayer(a, b);
		if (b.BelongsToGroup("enemies") && a.HasTag("player")) OnEnemyHitsPlayer(b, a);


		if (b.BelongsToGroup("projectile") && a.BelongsToGroup("enemies")) OnPlayerProjectileHitsEnemy(b, a);
		if (b.BelongsToGroup("enemies") && a.BelongsToGroup("colliders")) OnEnemyHitsObstacle(b, a);
		if (a.BelongsToGroup("enemies") && (b.BelongsToGroup("projectile") || b.HasTag("the_sword"))) OnEnemyHitsPlayerProjectile(a, b);
		if (b.BelongsToGroup("enemies") && (a.BelongsToGroup("projectile") || a.HasTag("the_sword"))) OnEnemyHitsPlayerProjectile(b, a);
	}

	void OnPlayerHitsProjectile(Entity projectile, Entity player)
	{
		// Should I put a check for this?
		auto& playerHealth = player.GetComponent<HealthComponent>();
		
		if (player.HasComponent<TransformComponent>() && player.HasComponent<BoxColliderComponent>() && !playerHealth.isHurt)
		{
			auto& playerTransform = player.GetComponent<TransformComponent>();
			auto& playerCollider = player.GetComponent<BoxColliderComponent>();
			auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
			

			auto& obstacleTransform = projectile.GetComponent<TransformComponent>();
			auto& obstacleCollider = projectile.GetComponent<BoxColliderComponent>();
			const auto& project = projectile.GetComponent<ProjectileEmitterComponent>();

			if (projectile.BelongsToGroup("projectile") && player.HasTag("player") && !project.isFriendly)
			{
				playerHealth.healthPercentage -= 1;
				playerTransform.position.x += 5; 
				playerTransform.position.y += 5; 
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "link_hurt", 0, 2);
				playerHealth.isHurt = true;
				
				// Remove the projectile
				projectile.Kill();
			}
		}
	}

	void OnPlayerProjectileHitsEnemy(Entity projectile, Entity enemy)
	{
		if ((projectile.HasTag("the_sword") || projectile.BelongsToGroup("projectile")) && enemy.BelongsToGroup("enemies"))
		{
			auto& enemyTransform = enemy.GetComponent<TransformComponent>();
			auto& enemyCollider = enemy.GetComponent<BoxColliderComponent>();
			auto& enemyRigidbody = enemy.GetComponent<RigidBodyComponent>();
			auto& enemyHealth = enemy.GetComponent<HealthComponent>();
			const auto& proj = projectile.GetComponent<ProjectileEmitterComponent>();

			if (proj.isFriendly && !enemyHealth.isHurt)
			{
				enemyHealth.healthPercentage -= 1;
				projectile.Kill();
				// Move the enemy slightly away in the opposite direction of movement
				if (enemyRigidbody.velocity.x > 0) enemyTransform.position.x -= 5;
				if (enemyRigidbody.velocity.x < 0) enemyTransform.position.x += 5;
				if (enemyRigidbody.velocity.y > 0) enemyTransform.position.y -= 5;
				if (enemyRigidbody.velocity.y < 0) enemyTransform.position.y += 5;

				// Play the enemy hit sound FX
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "enemy_hit", 0, 2);

				if (enemyHealth.healthPercentage <= 0)
				{
					// Play the enemy die sound and kill the entity
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "enemy_die", 0, 2);
				}

				enemyHealth.isHurt = true;
			}
		}
	}

	void OnEnemyHitsEnemy(Entity enemy1, Entity enemy2)
	{
		// Simple AI to have the enemy change direction if it collides with another enemy
		if (enemy1.BelongsToGroup("enemies") && enemy2.BelongsToGroup("enemies"))
		{
			auto& enemy1Rigidbody = enemy1.GetComponent<RigidBodyComponent>();
			auto& enemy2Rigidbody = enemy2.GetComponent<RigidBodyComponent>();

			if ((enemy1Rigidbody.velocity.x > 0 || enemy1Rigidbody.velocity.x < 0) && enemy1Rigidbody.velocity.y == 0)
				enemy1Rigidbody.velocity.x *= -1;
			if ((enemy1Rigidbody.velocity.y > 0 || enemy1Rigidbody.velocity.y < 0) && enemy1Rigidbody.velocity.x == 0)
				enemy2Rigidbody.velocity.y *= -1;
		}
	}

	void OnEnemyHitsObstacle(Entity enemy, Entity obstacle)
	{

	}
	
	void OnEnemyHitsPlayerProjectile(Entity enemy, Entity projectile)
	{
		if (enemy.BelongsToGroup("enemies") && (projectile.HasTag("the_sword") || projectile.BelongsToGroup("projectile")))
		{
			auto& health = enemy.GetComponent<HealthComponent>();
			auto& enemyTransform = enemy.GetComponent<TransformComponent>();
			auto& animation = enemy.GetComponent<AnimationComponent>();
			const auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();
			auto& proj = projectile.GetComponent<ProjectileEmitterComponent>();
			
			if (proj.isFriendly && !health.isHurt)
			{
				health.healthPercentage -= 1;
				
				// Move the enemy slightly away in the opposite direction of movement
				if (rigidBody.velocity.x > 0) enemyTransform.position.x -= 5;
				if (rigidBody.velocity.x < 0) enemyTransform.position.x += 5;
				if (rigidBody.velocity.y > 0) enemyTransform.position.y -= 5;
				if (rigidBody.velocity.y < 0) enemyTransform.position.y += 5;
				
				if (health.healthPercentage <= 0)
				{
					// Play the enemy die sound and kill the entity
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "enemy_die", 0, 2);
				}
				else
				{
					// Play the enemy hit sound FX
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "enemy_hit", 0, 2);
				}

				health.isHurt = true;
			}
		}
	}

	void OnEnemyHitsPlayer(Entity enemy, Entity player)
	{
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		const auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
		auto& playerHealth = player.GetComponent<HealthComponent>();

		const auto& obstacleTransform = enemy.GetComponent<TransformComponent>();
		const auto& obstacleCollider = enemy.GetComponent<BoxColliderComponent>();

		if (enemy.BelongsToGroup("enemies") && player.HasTag("player") && !playerHealth.isHurt)
		{
			playerHealth.healthPercentage -= 1;
			playerTransform.position.x += 5;
			playerTransform.position.y += 5;
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "link_hurt", 0, 2);
			playerHealth.isHurt = true;
		}
	}

	void Update();
};