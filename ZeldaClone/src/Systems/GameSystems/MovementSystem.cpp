#include "MovementSystem.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "KeyboardControlSystem.h"
#include "../../Game/Game.h"

MovementSystem::MovementSystem()
	: game(Game::Instance())
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<RigidBodyComponent>();
	RequiredComponent<BoxColliderComponent>();
}

void MovementSystem::Update(const double& deltaTime)
{
	// Update entity position based on its velocity
	for (const auto& entity : GetSystemEntities())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& collision = entity.GetComponent<BoxColliderComponent>();

		if (!transform.collision)
		{
			transform.position.x += rigidBody.velocity.x * deltaTime;
			transform.position.y += rigidBody.velocity.y * deltaTime;
		}
		
		//auto& playerPos = game.GetPlayerPos();

		// Update the player position for the location map in the HUD!
		if (entity.HasTag("player"))
		{
			for (int i = 0; i < MAX_WORLD_WIDTH; i++)
			{
				for (int j = 0; j < MAX_WORLD_HEIGHT; j++)
				{
					if (transform.position.x  >= (1024 * i) && transform.position.x + collision.offset.x <= 1024 + (1024 * i))
					{
						if (transform.position.y + collision.height - collision.offset.y >= (672 * j) && transform.position.y + collision.offset.y <= 672 + (672 * j))
						{
							/*playerPos.x = i;
							playerPos.y = j;
							*/game.SetPlayerPos(glm::vec2(i, j));
						}
					}
				}
			}

			// Stop the Sword and shield from moving ahead while transitioning from different screens
			auto sword = Registry::Instance().GetEntityByTag("the_sword");
			auto& swordTransform = sword.GetComponent<TransformComponent>();
			auto shield = Registry::Instance().GetEntityByTag("the_shield");
			auto& shieldTransform = shield.GetComponent<TransformComponent>();

			if (rigidBody.velocity.x == 0 && rigidBody.velocity.y == 0)
			{
				swordTransform.position = transform.position;
				shieldTransform.position = transform.position;
			}

			if (rigidBody.velocity.y > 0)
			{
				// Set the direction
				rigidBody.down = true;
				rigidBody.up = false;
				rigidBody.left = false;
				rigidBody.right = false;
			}
			if (rigidBody.velocity.y < 0)
			{
				// Set the direction
				rigidBody.down = false;
				rigidBody.up = true;
				rigidBody.left = false;
				rigidBody.right = false;
			}
			if (rigidBody.velocity.x > 0)
			{
				// Set the direction
				rigidBody.down = false;
				rigidBody.up = false;
				rigidBody.left = false;
				rigidBody.right = true;
			}
			if (rigidBody.velocity.x < 0)
			{
				// Set the direction
				rigidBody.down = false;
				rigidBody.up = false;
				rigidBody.left = true;
				rigidBody.right = false;
			}
		}

		if (entity.BelongsToGroup("enemies"))
		{
			auto& ai = entity.GetComponent<AIComponent>();

			if (rigidBody.velocity.y > 0)
			{
				if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
					sprite.srcRect.x = sprite.width * 0 + sprite.offset.x;

				// Set the direction
				rigidBody.down = true;
				rigidBody.up = false;
				rigidBody.left = false;
				rigidBody.right = false;
				//Logger::Log("Down");
			}
			if (rigidBody.velocity.y < 0)
			{
				if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
					sprite.srcRect.x = sprite.width * 2 + sprite.offset.x;

				// Set the direction
				rigidBody.down = false;
				rigidBody.up = true;
				rigidBody.left = false;
				rigidBody.right = false;
				//Logger::Log("Up");
			}
			if (rigidBody.velocity.x > 0)
			{
				if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
					sprite.srcRect.x = sprite.width * 3 + sprite.offset.x;

				// Set the direction
				rigidBody.down = false;
				rigidBody.up = false;
				rigidBody.left = false;
				rigidBody.right = true;
				//Logger::Log("Right");
			}
			if (rigidBody.velocity.x < 0)
			{
				if (ai.GetEnemyType() != AIComponent::EnemyType::LEEVER)
					sprite.srcRect.x = sprite.width * 1 + sprite.offset.x;

				// Set the direction
				rigidBody.down = false;
				rigidBody.up = false;
				rigidBody.left = true;
				rigidBody.right = false;
				//Logger::Log("Left");
			}

			auto& enemyPos = ai.GetEnemyPos();

			for (int i = 0; i < MAX_WORLD_WIDTH; i++)
			{
				for (int j = 0; j < MAX_WORLD_HEIGHT; j++)
				{
					if (transform.position.x >= (1024 * i) && transform.position.x <= 1024 + (1024 * i))
					{
						if (transform.position.y >= (672 * j) && transform.position.y <= 672 + (672 * j))
						{
							enemyPos.x = i;
							enemyPos.y = j;
						}
					}
				}
			}
		}
	}
}

void MovementSystem::SubscribeToEvents(const std::unique_ptr<EventManager>& eventManager)
{
	eventManager->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
}

void MovementSystem::OnCollision(CollisionEvent& colEvent)
{
	Entity a = colEvent.a;
	Entity b = colEvent.b;

	if ((a.BelongsToGroup("colliders") || a.BelongsToGroup("tiles")) && (b.HasTag("player"))) 
		OnPlayerHitsObstacle(a, b);
	if ((b.BelongsToGroup("colliders") || b.BelongsToGroup("tiles")) && (a.HasTag("player"))) 
		OnPlayerHitsObstacle(b, a);

	if ((b.BelongsToGroup("colliders") || b.BelongsToGroup("tiles")) && a.BelongsToGroup("enemies")) OnEnemyHitsObstacle(a, b);
	if (b.BelongsToGroup("enemies") && (a.BelongsToGroup("colliders") || a.BelongsToGroup("tiles"))) OnEnemyHitsObstacle(b, a);

	if (a.HasComponent<TriggerBoxComponent>() && (b.HasTag("player")))
	{
		if (a.GetComponent<TriggerBoxComponent>().collider)
			OnPlayerHitsObstacle(a, b);
	}

	if (b.HasComponent<TriggerBoxComponent>() && (a.HasTag("player")))
	{
		if (b.GetComponent<TriggerBoxComponent>().collider)
			OnPlayerHitsObstacle(b, a);
	}
}

void MovementSystem::OnEnemyHitsObstacle(Entity enemy, Entity obstacle)
{
	auto& enemy1Rigidbody = enemy.GetComponent<RigidBodyComponent>();
	auto& transform = enemy.GetComponent<TransformComponent>();
	if ((obstacle.BelongsToGroup("colliders") || obstacle.BelongsToGroup("tiles")) && enemy.BelongsToGroup("enemies"))
	{
		if (enemy1Rigidbody.velocity.y > 0)
		{
			transform.position.y -= 10;
			enemy1Rigidbody.velocity.x = enemy1Rigidbody.velocity.y;
			enemy1Rigidbody.velocity.y = 0;
		}
		else if (enemy1Rigidbody.velocity.y < 0)
		{
			transform.position.y += 10;
			enemy1Rigidbody.velocity.x = enemy1Rigidbody.velocity.y;
			enemy1Rigidbody.velocity.y = 0;
		}
		else if (enemy1Rigidbody.velocity.x > 0)
		{
			transform.position.x -= 10;
			enemy1Rigidbody.velocity.y = -enemy1Rigidbody.velocity.x;
			enemy1Rigidbody.velocity.x = 0;
		}
		else if (enemy1Rigidbody.velocity.x < 0)
		{
			transform.position.x += 10;
			enemy1Rigidbody.velocity.y = enemy1Rigidbody.velocity.x;
			enemy1Rigidbody.velocity.x = 0;
		}
	}
}

void MovementSystem::OnPlayerHitsObstacle(Entity obstacle, Entity player)
{
	if (player.HasComponent<TransformComponent>() && player.HasComponent<BoxColliderComponent>())
	{
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();

		auto& obstacleTransform = obstacle.GetComponent<TransformComponent>();
		auto& obstacleCollider = obstacle.GetComponent<BoxColliderComponent>();

		if (KeyboardControlSystem::dir == UP)
		{
			playerTransform.collision = true;
			playerRigidbody.velocity.y = 0;
			playerTransform.position.y = (obstacleTransform.position.y - playerCollider.offset.y + obstacleCollider.offset.y) + (obstacleCollider.height * obstacleTransform.scale.y);
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
			playerRigidbody.velocity.x = 0;
			playerTransform.position.x = (obstacleTransform.position.x - playerCollider.offset.x + obstacleCollider.offset.x) - (playerCollider.width * playerTransform.scale.x);
			playerTransform.position.y = playerTransform.position.y;
		}
	}
}
