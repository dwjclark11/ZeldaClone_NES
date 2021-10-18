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
#include "../GameSystems/KeyboardControlSystem.h"

class DamageSystem : public System
{

public:
	
	DamageSystem()
	{
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

		// Enemy projectile hits the player
		if (a.BelongsToGroup("projectile") && b.HasTag("player")) 
			OnPlayerHitsProjectile(a, b);
		if (b.BelongsToGroup("projectile") && a.HasTag("player")) 
			OnPlayerHitsProjectile(b, a);

		// Player/Enemy run into each other
		if (a.BelongsToGroup("enemies") && b.HasTag("player")) 
			OnEnemyHitsPlayer(a, b);
		if (b.BelongsToGroup("enemies") && a.HasTag("player")) 
			OnEnemyHitsPlayer(b, a);

		// Player Sword Hits Enemy
		if (b.HasTag("the_sword") && a.BelongsToGroup("enemies"))
			OnPlayerSwordHitsEnemy(b, a);
		if (a.HasTag("the_sword") && b.BelongsToGroup("enemies"))
			OnPlayerSwordHitsEnemy(a, b);

		// Player Sword beam or arrow hits enemy
		if (a.BelongsToGroup("enemies") && (b.BelongsToGroup("beam") || b.BelongsToGroup("projectile")))
			OnEnemyHitsPlayerProjectile(a, b);
		if (b.BelongsToGroup("enemies") && (a.BelongsToGroup("beam") || a.BelongsToGroup("projectile"))) 
			OnEnemyHitsPlayerProjectile(b, a);
	}

	void OnPlayerHitsProjectile(Entity projectile, Entity player)
	{
		// Should I put a check for this?
		auto& playerHealth = player.GetComponent<HealthComponent>();
		
		if (player.HasComponent<TransformComponent>() && player.HasComponent<BoxColliderComponent>() && !playerHealth.isHurt)
		{
			auto& playerTransform = player.GetComponent<TransformComponent>();
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

	void OnPlayerSwordHitsEnemy(Entity sword, Entity enemy)
	{
		if (sword.HasTag("the_sword")  && enemy.BelongsToGroup("enemies") && KeyboardControlSystem::keyDown)
		{		
			auto& enemyTransform = enemy.GetComponent<TransformComponent>();
			auto& enemyRigidbody = enemy.GetComponent<RigidBodyComponent>();
			auto& enemyHealth = enemy.GetComponent<HealthComponent>();

			if (!enemyHealth.isHurt)
			{
				// Subtract 1 HP from the enemy
				enemyHealth.healthPercentage -= 1; // * SwordLevel!! TODO!!
				
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

	//void OnEnemyHitsEnemy(Entity enemy1, Entity enemy2)
	//{
	//	// Simple AI to have the enemy change direction if it collides with another enemy
	//	if (enemy1.BelongsToGroup("enemies") && enemy2.BelongsToGroup("enemies"))
	//	{
	//		auto& enemy1Rigidbody = enemy1.GetComponent<RigidBodyComponent>();
	//		auto& enemy2Rigidbody = enemy2.GetComponent<RigidBodyComponent>();

	//		if ((enemy1Rigidbody.velocity.x > 0 || enemy1Rigidbody.velocity.x < 0) && enemy1Rigidbody.velocity.y == 0)
	//			enemy1Rigidbody.velocity.x *= -1;
	//		if ((enemy1Rigidbody.velocity.y > 0 || enemy1Rigidbody.velocity.y < 0) && enemy1Rigidbody.velocity.x == 0)
	//			enemy2Rigidbody.velocity.y *= -1;
	//	}
	//}
	
	void OnEnemyHitsPlayerProjectile(Entity enemy, Entity projectile)
	{
		if (enemy.BelongsToGroup("enemies") && (projectile.BelongsToGroup("projectile") || projectile.BelongsToGroup("beam")))
		{
			auto& health = enemy.GetComponent<HealthComponent>();
			auto& enemyTransform = enemy.GetComponent<TransformComponent>();
			const auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();
			auto& proj = projectile.GetComponent<ProjectileComponent>();
			
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
				// Kill the projectile --> start the projectile death
				proj.startTime = proj.duration - 500;
			}
		}
	}

	void OnEnemyHitsPlayer(Entity enemy, Entity player)
	{
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerHealth = player.GetComponent<HealthComponent>();

		if (enemy.BelongsToGroup("enemies") && player.HasTag("player") && !playerHealth.isHurt)
		{
			playerHealth.healthPercentage -= 1;
			playerTransform.position.x += 5;
			playerTransform.position.y += 5;

			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "link_hurt", 0, 2);
			playerHealth.isHurt = true;
		}
	}
};