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
private:
	class Game& game;
public:
	
	DamageSystem()
		: game(*Game::Instance())
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
		if (a.BelongsToGroup("projectile") && (b.HasTag("player") || b.HasTag("the_shield")))
			OnPlayerHitsProjectile(a, b);
		if (b.BelongsToGroup("projectile") && (a.HasTag("player") || a.HasTag("the_shield")))
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

		if (a.BelongsToGroup("enemies") && b.BelongsToGroup("boomerang"))
			OnEnemyHitsBoomerang(a, b);
		if (b.BelongsToGroup("enemies") &&  a.BelongsToGroup("boomerang"))
			OnEnemyHitsBoomerang(b, a);
	}

	void OnPlayerHitsProjectile(Entity projectile, Entity player)
	{
		// Should I put a check for this?
		auto& playerHealth = player.GetComponent<HealthComponent>();
		
		if (player.HasComponent<TransformComponent>() && player.HasComponent<BoxColliderComponent>() && !playerHealth.isHurt)
		{
			auto& playerTransform = player.GetComponent<TransformComponent>();
			auto& projectileRigid = projectile.GetComponent<RigidBodyComponent>();
			auto& project = projectile.GetComponent<ProjectileComponent>();

			// If the projectile hits the player, inflict damage
			if (projectile.BelongsToGroup("projectile") && player.HasTag("player") && !project.isFriendly)
			{
				playerHealth.healthPercentage -= project.hitPercentDamage; // This will need to change based on the enemy attack power!
				playerTransform.position.x += 5; 
				playerTransform.position.y += 5; 
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "link_hurt", 0, 2);
				playerHealth.isHurt = true;
				
				// Remove the projectile
				projectile.Kill();
			}
			// Have the projectile bounce off the shield and not hurt the player
			else if (projectile.BelongsToGroup("projectile") && player.HasTag("the_shield") && !project.isFriendly)
			{
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "shield", 0, 6);
				
				if (projectileRigid.velocity.y < 0)
				{
					projectileRigid.velocity.y = 100;
					projectileRigid.velocity.x = (rand() % 2 - 1) * 50;
				}
				else if (projectileRigid.velocity.y > 0)
				{
					projectileRigid.velocity.y = -100;
					projectileRigid.velocity.x = (rand() % 2 - 1) * 50;
				}
				else if (projectileRigid.velocity.x > 0)
				{
					projectileRigid.velocity.x = -100;
					projectileRigid.velocity.y = (rand() % 2 - 1) * 50;
				}
				else if (projectileRigid.velocity.x < 0)
				{
					projectileRigid.velocity.x = 100;
					projectileRigid.velocity.y = (rand() % 2 - 1) * 50;
				}

				// Start the projectile death
				project.startTime = project.duration;
			}
		}
	}

	void OnPlayerSwordHitsEnemy(Entity sword, Entity enemy)
	{
		if (sword.HasTag("the_sword")  && enemy.BelongsToGroup("enemies") && KeyboardControlSystem::keyDown)
		{		
			auto& enemyTransform	= enemy.GetComponent<TransformComponent>();
			auto& enemyRigidbody	= enemy.GetComponent<RigidBodyComponent>();
			auto& enemyHealth		= enemy.GetComponent<HealthComponent>();
			auto& enemyAI			= enemy.GetComponent<AIComponent>();
			if (enemyAI.GetEnemyType() != AIComponent::EnemyType::LEEVER)
			{
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
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_hit", 0, 2);

					if (enemyHealth.healthPercentage <= 0)
					{
						// Play the enemy die sound and kill the entity
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_die", 0, 2);
					}
					enemyHealth.isHurt = true;
				}
			}
			else
			{
				// We cannot attack the leever while it is transitioning from above/under ground
				if (enemyAI.leeverTimer.GetTicks() > 3100 && enemyAI.leeverTimer.GetTicks() < 10000)
				{
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
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_hit", 0, 2);

						if (enemyHealth.healthPercentage <= 0)
						{
							// Play the enemy die sound and kill the entity
							game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_die", 0, 2);
						}
						enemyHealth.isHurt = true;
					}
				}
			}
		}
	}
	
	void OnEnemyHitsPlayerProjectile(Entity enemy, Entity projectile)
	{
		if (enemy.BelongsToGroup("enemies") && (projectile.BelongsToGroup("projectile") || projectile.BelongsToGroup("beam")))
		{
			auto& health = enemy.GetComponent<HealthComponent>();
			auto& enemyTransform = enemy.GetComponent<TransformComponent>();
			const auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();
			auto& proj = projectile.GetComponent<ProjectileComponent>();
			auto& enemyAI = enemy.GetComponent<AIComponent>();

			if (proj.isFriendly && !health.isHurt)
			{
				if (enemyAI.GetEnemyType() != AIComponent::EnemyType::LEEVER)
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
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_die", 0, 2);
					}
					else
					{
						// Play the enemy hit sound FX
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_hit", 0, 2);
					}

					health.isHurt = true;
					// Kill the projectile --> start the projectile death
					proj.startTime = proj.duration - 500;
				}
				else
				{
					// We cannot attack the leever while it is transitioning from above/under ground
					if (enemyAI.leeverTimer.GetTicks() > 3100 && enemyAI.leeverTimer.GetTicks() < 10000)
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
							game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_die", 0, 2);
						}
						else
						{
							// Play the enemy hit sound FX
							game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_hit", 0, 2);
						}

						health.isHurt = true;
						// Kill the projectile --> start the projectile death
						proj.startTime = proj.duration - 500;
					}
				}
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

			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "link_hurt", 0, 2);
			playerHealth.isHurt = true;
		}
	}

	void OnEnemyHitsBoomerang(Entity enemy, Entity boomerang)
	{
		auto& enemyRigid = enemy.GetComponent<RigidBodyComponent>();
		auto& enemyAI = enemy.GetComponent<AIComponent>();

		if (boomerang.BelongsToGroup("boomerang"))
		{
			enemyRigid.velocity = glm::vec2(0);

			if (!enemyAI.GetStunned())
			{
				enemyAI.SetStunned(true);
				// Play the enemy hit sound FX
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "enemy_hit", 0, 2);
			}
		}
	}
};