#include "MovementSystem.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/EnemyComponent.h"
#include "../../Components/PlayerComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/ProjectileComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../Game/Game.h"
#include "../../Game/Player.h"


void MovementSystem::UpdateBoomerang(const double& deltaTime)
{
	// If there is no boomerang, nothing to update!
	if (!Registry::Instance().DoesTagExist("boomerang"))
		return;

	const auto& player = game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	const auto& playerPos = playerEnt.GetComponent<TransformComponent>();
	const auto& playerSprite = playerEnt.GetComponent<SpriteComponent>();

	// Boomerang variables
	auto boomerang = Registry::Instance().GetEntityByTag("boomerang");
	auto& rigidbody = boomerang.GetComponent<RigidBodyComponent>();
	auto& proj = boomerang.GetComponent<ProjectileComponent>();
	auto& transform = boomerang.GetComponent<TransformComponent>();

	// Have the boomerang change direction based on the player position
	glm::vec2 direction = glm::normalize(
		((playerPos.position + glm::vec2(playerSprite.width / 2, playerSprite.height / 2)) - transform.position));
	bool boomerangReturned = false;

	if (proj.boomTimer.GetTicks() > 300) // TODO: Change the time based on type of boomerang-->wood/magic
	{
		rigidbody.velocity.x = direction.x * 500; 
		rigidbody.velocity.y = direction.y * 500; 
		boomerangReturned = true;
	}

	if (!transform.collision)
	{
		transform.position.x += rigidbody.velocity.x * deltaTime;
		transform.position.y += rigidbody.velocity.y * deltaTime;
	}
	else
	{
		boomerangReturned = true;
	}
		
	if (boomerangReturned)
	{
		bool setKill = false;
		transform.position.x += direction.x * deltaTime;
		transform.position.y += direction.y * deltaTime;

		// If the boomerang position is within any of these given parameters --> Kill it
		if (transform.position.x <= playerPos.position.x + (playerSprite.width / 2) && transform.position.y <= playerPos.position.y + (playerSprite.height / 2) &&
			transform.position.x >= playerPos.position.x && transform.position.y >= playerPos.position.y)
			setKill = true;

		if (setKill)
			boomerang.Kill();		
	}
}

void MovementSystem::UpdateEnemies(const double& deltaTime)
{
	const auto& enemies = Registry::Instance().GetEntitiesByGroup("enemies");

	for (const auto& enemy : enemies)
	{
		auto& transform = enemy.GetComponent<TransformComponent>();
		auto& enemyComponent = enemy.GetComponent<EnemyComponent>();
		auto& ai = enemy.GetComponent<AIComponent>();
		const auto& collider = enemy.GetComponent<BoxColliderComponent>();

		// Calculate the min max positions -- TODO: MOVE THIS TO ENEMY COMPONENT
		if (!enemyComponent.distanceCalculated)
		{
			const int entityXPanel = transform.position.x / 1024;
			const int entityYPanel = transform.position.y / 672;

			enemyComponent.minDistance.x = entityXPanel * 1024;
			enemyComponent.minDistance.y = entityYPanel * 672;

			enemyComponent.maxDistance.x = enemyComponent.minDistance.x + 1024;
			enemyComponent.maxDistance.y = enemyComponent.minDistance.y + 672;

			enemyComponent.distanceCalculated = true;
			ai.SetEmemyPos(glm::vec2{ entityXPanel, entityYPanel });
		}

		if (ai.GetEnemyPos() != game.GetPlayer()->GetPlayerPos() || ai.GetEnemyType() == AIComponent::EnemyType::BLADE_TRAP)
			continue;
		
		auto& rigidBody = enemy.GetComponent<RigidBodyComponent>();
		auto& sprite = enemy.GetComponent<SpriteComponent>();
		auto& collision = enemy.GetComponent<BoxColliderComponent>();
		auto& health = enemy.GetComponent<HealthComponent>();

		if (!transform.collision && !health.isHurt && health.healthPercentage > 0)
		{
			transform.position.x += rigidBody.velocity.x * deltaTime;
			transform.position.y += rigidBody.velocity.y * deltaTime;

			if (transform.position.x <= enemyComponent.minDistance.x)
			{
				transform.position.x = enemyComponent.minDistance.x;
				rigidBody.velocity.x *= -1;
			}

			if (transform.position.x >= enemyComponent.maxDistance.x)
			{
				transform.position.x = enemyComponent.maxDistance.x - (collider.width * transform.scale.x);
				rigidBody.velocity.x *= -1;
			}

			if (transform.position.y <= enemyComponent.minDistance.y)
			{
				transform.position.y = enemyComponent.minDistance.y;
				rigidBody.velocity.y *= -1;
			}

			if (transform.position.y >= enemyComponent.maxDistance.y)
			{
				transform.position.y = enemyComponent.maxDistance.y - (collider.height * transform.scale.y);
				rigidBody.velocity.y *= -1;
			}
		}
		
		SetDirection(rigidBody);

		if (rigidBody.dir == RigidBodyComponent::Dir::DOWN)
			sprite.srcRect.x = sprite.width * 0 + sprite.offset.x;
		else if (rigidBody.dir == RigidBodyComponent::Dir::LEFT)
			sprite.srcRect.x = sprite.width * 1 + sprite.offset.x;
		else if (rigidBody.dir == RigidBodyComponent::Dir::UP)
			sprite.srcRect.x = sprite.width * 2 + sprite.offset.x;
		else if (rigidBody.dir == RigidBodyComponent::Dir::RIGHT)
			sprite.srcRect.x = sprite.width * 3 + sprite.offset.x;

	}
}

void MovementSystem::UpdatePlayer(const double& deltaTime)
{
	const auto& player = game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& player_transform = playerEnt.GetComponent<TransformComponent>();
	auto& player_rigidBody = playerEnt.GetComponent<RigidBodyComponent>();
	auto& player_sprite = playerEnt.GetComponent<SpriteComponent>();
	auto& player_collision = playerEnt.GetComponent<BoxColliderComponent>();
	
	const int playerXPanel = player_transform.position.x / 1024;
	const int playerYPanel = player_transform.position.y / 672;

	player->SetPlayerPos(glm::vec2(playerXPanel, playerYPanel));

	if (!player_transform.collision)
	{
		player_transform.position.x += player_rigidBody.velocity.x * deltaTime;
		player_transform.position.y += player_rigidBody.velocity.y * deltaTime;
	}

	// Stop the Sword and shield from moving ahead while transitioning from different screens
	auto sword = player->GetSword();
	auto& swordTransform = sword.GetComponent<TransformComponent>();
	auto shield = player->GetShield();
	auto& shieldTransform = shield.GetComponent<TransformComponent>();

	if (player_rigidBody.velocity.x == 0 && player_rigidBody.velocity.y == 0)
	{
		swordTransform.position = player_transform.position;
		shieldTransform.position = player_transform.position;
	}

	SetDirection(player_rigidBody);
}

void MovementSystem::UpdateProjectiles(const double& deltaTime)
{
	if (!Registry::Instance().DoesGroupExist("projectile"))
		return;

	const auto& projectiles = Registry::Instance().GetEntitiesByGroup("projectile");

	for (const auto& projectile : projectiles)
	{
		auto& transform = projectile.GetComponent<TransformComponent>();
		auto& rigidBody = projectile.GetComponent<RigidBodyComponent>();
		auto& sprite = projectile.GetComponent<SpriteComponent>();
		auto& collision = projectile.GetComponent<BoxColliderComponent>();

		if (!transform.collision)
		{
			transform.position.x += rigidBody.velocity.x * deltaTime;
			transform.position.y += rigidBody.velocity.y * deltaTime;
		}

		SetDirection(rigidBody);
	}
}

void MovementSystem::SetDirection(RigidBodyComponent& rigidBody)
{
	if (rigidBody.velocity.y > 0)
	{
		rigidBody.dir = RigidBodyComponent::Dir::DOWN;
	}
	else if (rigidBody.velocity.y < 0)
	{
		rigidBody.dir = RigidBodyComponent::Dir::UP;
	}
	else if (rigidBody.velocity.x > 0)
	{
		rigidBody.dir = RigidBodyComponent::Dir::RIGHT;
	}
	else if (rigidBody.velocity.x < 0)
	{
		rigidBody.dir = RigidBodyComponent::Dir::LEFT;
	}
}

MovementSystem::MovementSystem()
	: game(Game::Instance())
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<RigidBodyComponent>();
	RequiredComponent<BoxColliderComponent>();
}

void MovementSystem::Update(const double& deltaTime)
{
	UpdateBoomerang(deltaTime);
	UpdatePlayer(deltaTime);
	UpdateEnemies(deltaTime);
	UpdateProjectiles(deltaTime);
}

void MovementSystem::SubscribeToEvents(EventManager& eventManager)
{
	eventManager.SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
}

void MovementSystem::OnCollision(CollisionEvent& colEvent)
{
	Entity a = colEvent.a;
	Entity b = colEvent.b;

	if ((a.HasComponent<ColliderComponent>() || a.BelongsToGroup("tiles")) && (b.HasComponent<PlayerComponent>()))
	{
		OnPlayerHitsObstacle(a, b);
		return;
	}
	else if ((b.HasComponent<ColliderComponent>() || b.BelongsToGroup("tiles")) && (a.HasComponent<PlayerComponent>()))
	{
		OnPlayerHitsObstacle(b, a);
		return;
	}
		

	if ((b.HasComponent<ColliderComponent>() || b.BelongsToGroup("tiles")) && a.HasComponent<EnemyComponent>())
	{
		OnEnemyHitsObstacle(a, b);
		return;
	}
	else if (b.HasComponent<EnemyComponent>() && (a.HasComponent<ColliderComponent>() || a.BelongsToGroup("tiles"))) 
	{
		OnEnemyHitsObstacle(b, a);
		return;
	}

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
	auto& enemy_transform = enemy.GetComponent<TransformComponent>();
	auto& enemy_collider = enemy.GetComponent<BoxColliderComponent>();

	auto& obstacle_transform = obstacle.GetComponent<TransformComponent>();
	auto& obstacle_collider = obstacle.GetComponent<BoxColliderComponent>();

	if (enemy1Rigidbody.dir == RigidBodyComponent::Dir::DOWN)
	{
		enemy_transform.collision = true;
		enemy1Rigidbody.velocity.y = 0;
		enemy_transform.position.y = (obstacle_transform.position.y + obstacle_collider.offset.y) -
			(enemy_collider.height * enemy_transform.scale.y) - enemy_collider.offset.y;
		enemy1Rigidbody.velocity.x = -150;
	}
	else if (enemy1Rigidbody.dir == RigidBodyComponent::Dir::UP)
	{
		enemy_transform.collision = true;
		enemy1Rigidbody.velocity.y = 0;
		enemy_transform.position.y = (obstacle_transform.position.y + obstacle_collider.offset.y) +
			(obstacle_collider.height * obstacle_transform.scale.y) - enemy_collider.offset.y;

		enemy1Rigidbody.velocity.x = 150;
	}
	else if (enemy1Rigidbody.dir == RigidBodyComponent::Dir::RIGHT)
	{
		enemy_transform.collision = true;
		enemy1Rigidbody.velocity.x = 0;
		enemy_transform.position.x = (obstacle_transform.position.x + obstacle_collider.offset.x) -
			(obstacle_collider.width * obstacle_transform.scale.x) - enemy_collider.offset.x;
		
		enemy1Rigidbody.velocity.y = 150;
	}
	else if (enemy1Rigidbody.dir == RigidBodyComponent::Dir::LEFT)
	{
		enemy_transform.collision = true;
		enemy1Rigidbody.velocity.x = 0;
		enemy_transform.position.x = (obstacle_transform.position.x + obstacle_collider.offset.x) +
			(enemy_collider.width * enemy_transform.scale.x) - enemy_collider.offset.x;
		
		enemy1Rigidbody.velocity.y = -150;
	}
}

void MovementSystem::OnPlayerHitsObstacle(Entity obstacle, Entity player)
{
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& playerCollider = player.GetComponent<BoxColliderComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();

	auto& obstacleTransform = obstacle.GetComponent<TransformComponent>();
	auto& obstacleCollider = obstacle.GetComponent<BoxColliderComponent>();

	switch (playerRigidbody.dir)
	{
	case RigidBodyComponent::Dir::UP:
		playerTransform.collision = true;
		playerRigidbody.velocity.y = 0;
		playerTransform.position.y = (obstacleTransform.position.y - playerCollider.offset.y + obstacleCollider.offset.y) + (obstacleCollider.height * obstacleTransform.scale.y);
		playerTransform.position.x = playerTransform.position.x;
		break;
	case RigidBodyComponent::Dir::RIGHT:
		playerTransform.collision = true;
		playerRigidbody.velocity.x = 0;
		playerTransform.position.x = (obstacleTransform.position.x - playerCollider.offset.x + obstacleCollider.offset.x) - (playerCollider.width * playerTransform.scale.x);
		playerTransform.position.y = playerTransform.position.y;
		break;
	case RigidBodyComponent::Dir::DOWN:
		playerTransform.collision = true;
		playerRigidbody.velocity.y = 0;
		playerTransform.position.y = (obstacleTransform.position.y - playerCollider.offset.y + obstacleCollider.offset.y) - (playerCollider.height * playerTransform.scale.y);
		playerTransform.position.x = playerTransform.position.x;
		break;
	case RigidBodyComponent::Dir::LEFT:
		playerTransform.collision = true;
		playerRigidbody.velocity.x = 0;
		playerTransform.position.x = (obstacleTransform.position.x - playerCollider.offset.x + obstacleCollider.offset.x) + (obstacleCollider.width * obstacleTransform.scale.x);
		playerTransform.position.y = playerTransform.position.y;
		break;
	}
}
