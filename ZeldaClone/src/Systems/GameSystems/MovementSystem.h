#pragma once
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../Game/Game.h"
#include "../../ECS/ECS.h"
#include "KeyboardControlSystem.h"

const int MAX_WORLD_WIDTH = 16;
const int MAX_WORLD_HEIGHT = 8;

class MovementSystem : public System
{
public:
	MovementSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<RigidBodyComponent>();
		RequiredComponent<BoxColliderComponent>();
	}

	void Update(const double& deltaTime)
	{
		// Update entity position based on its velocity
		for (auto entity : GetSystemEntities())
		{			
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();

			if (!transform.collision)
			{
				transform.position.x += rigidBody.velocity.x * deltaTime;
				transform.position.y += rigidBody.velocity.y * deltaTime;
			}

			// Update the player position for the location map in the HUD!
			if (entity.HasTag("player"))
			{
				for (int i = 0; i < MAX_WORLD_WIDTH; i++)
				{
					for (int j = 0; j < MAX_WORLD_HEIGHT; j++)
					{
						if (transform.position.x >= (1024 * i) && transform.position.x <= 1024 + (1024 * i))
						{
							if (transform.position.y >= (672 * j) && transform.position.y <= 672 + (672 * j))
							{
								Game::Instance()->GetPlayerPos().x = i;
								Game::Instance()->GetPlayerPos().y = j;
							}
						}
					}
				}
			}

			if (entity.BelongsToGroup("enemies"))
			{
				if (rigidBody.velocity.y > 0)
				{
					sprite.srcRect.x = sprite.width * 0 + sprite.offset.x;
				}
				if (rigidBody.velocity.y < 0)
				{
					sprite.srcRect.x = sprite.width * 2 + sprite.offset.x;
				}
				if (rigidBody.velocity.x > 0)
				{
					sprite.srcRect.x = sprite.width * 3 + sprite.offset.x;
				}
				if (rigidBody.velocity.x < 0)
				{
					sprite.srcRect.x = sprite.width * 1 + sprite.offset.x;
				}
			}
		}
	}

	void SubscribeToEvents(const std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& colEvent)
	{
		Entity a = colEvent.a;
		Entity b = colEvent.b;

		if (a.BelongsToGroup("colliders") && (b.HasTag("player") || b.HasTag("the_shield") || b.HasTag("the_sword"))) OnPlayerHitsObstacle(a, b);
		if (b.BelongsToGroup("colliders") && (a.HasTag("player") || a.HasTag("the_shield") || a.HasTag("the_sword"))) OnPlayerHitsObstacle(b, a);
		if (b.BelongsToGroup("colliders") && a.BelongsToGroup("enemies")) OnEnemyHitsObstacle(a, b);
		//if (a.BelongsToGroup("enemies") && b.BelongsToGroup("colliders")) OnEnemyHitsObstacle(a, b);

	}

	void OnEnemyHitsObstacle(Entity enemy, Entity obstacle)
	{
		auto& enemy1Rigidbody = enemy.GetComponent<RigidBodyComponent>();
		auto& transform = enemy.GetComponent<TransformComponent>();

		if (obstacle.BelongsToGroup("colliders") && enemy.BelongsToGroup("enemies"))
		{
			if (enemy1Rigidbody.velocity.y > 0)
			{
				transform.position.y -= 5;
				enemy1Rigidbody.velocity.x = enemy1Rigidbody.velocity.y;
				enemy1Rigidbody.velocity.y = 0;
			}
			else if (enemy1Rigidbody.velocity.y < 0)
			{
				transform.position.y += 5;
				enemy1Rigidbody.velocity.x = enemy1Rigidbody.velocity.y;
				enemy1Rigidbody.velocity.y = 0;
			}
			else if (enemy1Rigidbody.velocity.x > 0)
			{
				transform.position.x -= 5;
				enemy1Rigidbody.velocity.y = -enemy1Rigidbody.velocity.x;
				enemy1Rigidbody.velocity.x = 0;
			}
			else if (enemy1Rigidbody.velocity.x < 0)
			{
				transform.position.x += 5;
				enemy1Rigidbody.velocity.y = enemy1Rigidbody.velocity.x;
				enemy1Rigidbody.velocity.x = 0;
			}
		}
	}

	void OnPlayerHitsObstacle( Entity obstacle, Entity player)
	{
		if (player.HasComponent<TransformComponent>() && player.HasComponent<BoxColliderComponent>())
		{
			if (obstacle.BelongsToGroup("colliders") && (player.HasTag("player") || player.HasTag("the_shield") || player.HasTag("the_sword")))
			{
				auto& playerTransform = player.GetComponent<TransformComponent>();
				auto& playerCollider = player.GetComponent<BoxColliderComponent>();
				auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();

				auto& obstacleTransform = obstacle.GetComponent<TransformComponent>();
				auto& obstacleCollider = obstacle.GetComponent<BoxColliderComponent>();
				//auto& obstacleRigidbody = obstacle.GetComponent<RigidBodyComponent>();

				if (KeyboardControlSystem::dir == UP)
				{
					playerTransform.collision = true;
					playerRigidbody.velocity.y = 0;
					playerTransform.position.y =  (obstacleTransform.position.y - playerCollider.offset.y + obstacleCollider.offset.y)  + (obstacleCollider.height * obstacleTransform.scale.y);
					playerTransform.position.x = playerTransform.position.x;
				}

				if (KeyboardControlSystem::dir == DOWN)
				{
					playerTransform.collision = true;
					playerRigidbody.velocity.y = 0;
					playerTransform.position.y = (obstacleTransform.position.y - playerCollider.offset.y + obstacleCollider.offset.y) - (playerCollider.height * playerTransform.scale.y);
					playerTransform.position.x = playerTransform.position.x;
				}

				if (KeyboardControlSystem::dir == LEFT)
				{
					playerTransform.collision = true;
					playerRigidbody.velocity.x = 0;
					playerTransform.position.x = (obstacleTransform.position.x - playerCollider.offset.x + obstacleCollider.offset.x) + (obstacleCollider.width * obstacleTransform.scale.x);
					playerTransform.position.y = playerTransform.position.y;
				}

				if (KeyboardControlSystem::dir == RIGHT)
				{
					playerTransform.collision = true;
					playerRigidbody.velocity = glm::vec2(0,0);
					playerTransform.position.x = (obstacleTransform.position.x - playerCollider.offset.x + obstacleCollider.offset.x) - (playerCollider.width * playerTransform.scale.x);
					playerTransform.position.y = playerTransform.position.y;
				}
			}
		}
	}
};