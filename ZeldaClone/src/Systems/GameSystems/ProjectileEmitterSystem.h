#pragma once
#include "../../ECS/ECS.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Events/GamePadButtonPressedEvent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/ProjectileComponent.h"
#include "../../Components/ProjectileEmitterComponent.h"
#include "../../Components/GameComponent.h"
#include "../GameSystems/KeyboardControlSystem.h"
#include "../GameSystems/ProjectileLifeCycleSystem.h"
#include "../PauseSystems/ItemSelectKeyboardSystem.h"
#include "../../States/GameState.h"
#include <SDL.h>
#include "../Systems/SoundFXSystem.h"
#include "GamePadSystem.h"

class ProjectileEmitterSystem : public System
{
private:
	Timer swordTimer;
	class Game& game;

	struct ItemAttrib
	{
		std::string group;
		std::string sprite_name;
		int width;
		int height;
		int srcRectX;
		int srcRectY;
		int numFrames;
		glm::vec2 scale;
		glm::vec2 transOffsetUp;
		glm::vec2 transOffsetDown;
		glm::vec2 transOffsetLeft;
		glm::vec2 transOffsetRight;
		glm::vec2 boxSizeUp;
		glm::vec2 boxSizeDown;
		glm::vec2 boxSizeLeft;
		glm::vec2 boxSizeRight;
		glm::vec2 upOffset;
		glm::vec2 downOffset;
		glm::vec2 leftOffset;
		glm::vec2 rightOffset;
		int duration;
		bool animation;
		bool vertical;

		ItemAttrib(std::string group, std::string sprite_name, int width, int height, int src_rect_x, int src_rect_y, int num_frames, glm::vec2 scale,
			glm::vec2 trans_offset_up, glm::vec2 trans_offset_down, glm::vec2 trans_offset_right, glm::vec2 trans_offset_left, glm::vec2 box_size_up,
			glm::vec2 box_size_down, glm::vec2 box_size_left, glm::vec2 box_size_right, glm::vec2 box_up_offset, glm::vec2 box_down_offset,
			glm::vec2 box_right_offset, glm::vec2 box_left_offset, int duration = 3000, bool animation = false, bool vertical = false)
		{
			this->group = group;
			this->sprite_name = sprite_name;
			this->width = width;
			this->height = height;
			srcRectX = src_rect_x;
			srcRectY = src_rect_y;
			numFrames = num_frames;
			this->scale = scale;
			transOffsetUp = trans_offset_up;
			transOffsetDown = trans_offset_down;
			transOffsetLeft = trans_offset_left;
			transOffsetRight = trans_offset_right;
			boxSizeUp = box_size_up;
			boxSizeDown = box_size_down;
			boxSizeLeft = box_size_left;
			boxSizeRight = box_size_right;
			upOffset = box_up_offset;
			downOffset = box_down_offset;
			leftOffset = box_left_offset;
			rightOffset = box_right_offset;
			this->duration = duration;
			this->animation = animation;
			this->vertical = vertical;
		}
	};

	bool magicBeam;
	bool fullLife;
public:

	bool boomerangReturned;
	GamePadSystem controller;

	ProjectileEmitterSystem()
		: game(*Game::Instance())
	{
		RequiredComponent<ProjectileEmitterComponent>();
		RequiredComponent<TransformComponent>();

		boomerangReturned = false;
		magicBeam = false;
		fullLife = false;
		swordTimer.Stop();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitterSystem::OnKeyPressed);
	}

	void UseItem(ItemAttrib& attrib)
	{
		auto player = Registry::Instance()->GetEntityByTag("player");

		auto& projectileEmitter = player.GetComponent<ProjectileEmitterComponent>();
		const auto& transform = player.GetComponent<TransformComponent>();
		const auto& rigidbody = player.GetComponent<RigidBodyComponent>();

		// If the entity has a sprite --> Position the projectile in the center
		glm::vec2 projectilePosition = transform.position;

		const auto& sprite = player.GetComponent<SpriteComponent>();
		projectilePosition.x += (transform.scale.x * sprite.width / 2);
		projectilePosition.y += (transform.scale.y * sprite.height / 2);

		glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
		glm::vec2 BoxOffset = glm::vec2(0, 0);
		glm::vec2 BoxSize = glm::vec2(0, 0);

		// If the duration is not put in, default duration 
		if (attrib.duration == 0)
		{
			attrib.duration = projectileEmitter.projectileDuration;
		}

		// Create direction variables the change the velocity direction based on which way we are facing
		int directionX = 0;
		int directionY = 0;

		// Use the fire ahead of the player in the direction the player is facing
		if (KeyboardControlSystem::dir == UP)
		{
			if (attrib.numFrames == 1)
			{
				projectilePosition.x = (transform.position.x + attrib.transOffsetUp.x);
				projectilePosition.y = (transform.position.y + attrib.transOffsetUp.x) - sprite.height * 3;
			}
			else
			{
				projectilePosition.x = (transform.position.x + attrib.transOffsetUp.x);
				projectilePosition.y = (transform.position.y + attrib.transOffsetUp.x) - sprite.height * 3;
				// attrib.srcRectX  += attrib.width * 1;

			}
			BoxSize = attrib.boxSizeUp;
			BoxOffset = attrib.upOffset;
			directionY = -1;
		}
		if (KeyboardControlSystem::dir == RIGHT)
		{
			if (attrib.numFrames == 1)
			{
				projectilePosition.x = ((transform.position.x + attrib.transOffsetRight.x) + sprite.width * 3);
				projectilePosition.y = (transform.position.y + attrib.transOffsetRight.y);
			}
			else
			{
				projectilePosition.x = ((transform.position.x + attrib.transOffsetRight.x) + sprite.width * 3);
				projectilePosition.y = (transform.position.y + attrib.transOffsetRight.y);
				attrib.srcRectX += attrib.width * 1;
			}
			BoxSize = attrib.boxSizeRight;
			BoxOffset = attrib.rightOffset;
			directionX = 1;
		}
		if (KeyboardControlSystem::dir == DOWN)
		{
			if (attrib.numFrames == 1)
			{
				projectilePosition.x = (transform.position.x + attrib.transOffsetDown.x);
				projectilePosition.y = ((transform.position.y + attrib.transOffsetDown.y) + sprite.height * 3);
			}
			else
			{
				projectilePosition.x = (transform.position.x + attrib.transOffsetDown.x);
				projectilePosition.y = ((transform.position.y + attrib.transOffsetDown.y) + sprite.height * 3);
				attrib.srcRectX += attrib.width * 2;
			}
			BoxSize = attrib.boxSizeDown;
			BoxOffset = attrib.downOffset;
			directionY = 1;
		}
		if (KeyboardControlSystem::dir == LEFT)
		{
			if (attrib.numFrames == 1)
			{
				projectilePosition.x = ((transform.position.x + attrib.transOffsetLeft.x) - sprite.width * 3);
				projectilePosition.y = (transform.position.y + attrib.transOffsetLeft.y);
			}
			else
			{
				projectilePosition.x = ((transform.position.x + attrib.transOffsetLeft.x) - sprite.width * 3);
				projectilePosition.y = (transform.position.y + attrib.transOffsetLeft.y);
				attrib.srcRectX += attrib.width * 3;
			}
			BoxSize = attrib.boxSizeLeft;
			BoxOffset = attrib.leftOffset;
			directionX = -1;
		}

		// Create new projectile entity and add it to the world
		Entity newItem = player.registry->CreateEntity();
		newItem.Group(attrib.group);
		newItem.AddComponent<TransformComponent>(projectilePosition, glm::vec2(attrib.scale.x, attrib.scale.y), 0.0);

		if (attrib.group != "bomber")
		{
			// Set the projectile velocity based on the direction 
			projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
			projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
			newItem.AddComponent<RigidBodyComponent>(projectileVelocity);
			newItem.AddComponent<BoxColliderComponent>((int)BoxSize.x / transform.scale.x, (int)BoxSize.y / transform.scale.y, glm::vec2(BoxOffset.x, BoxOffset.y));
		}
		// Does the projectile have an animation component?
		if (attrib.animation)
		{
			// TODO: Change the frame speed from hard coded to individual --> attrib.frame_speed
			newItem.AddComponent<AnimationComponent>(attrib.numFrames, 20, attrib.vertical, true);
		}
		projectileEmitter.isFriendly = true;
		newItem.AddComponent<SpriteComponent>(attrib.sprite_name, attrib.width, attrib.height, 2, false, attrib.srcRectX, attrib.srcRectY);
		newItem.AddComponent<ProjectileEmitterComponent>(projectileEmitter.projectileVelocity, 0, attrib.duration, projectileEmitter.hitPercentDamage, projectileEmitter.isFriendly);
		newItem.AddComponent<ProjectileComponent>(true, 10, attrib.duration);
		newItem.AddComponent<GameComponent>();

		if (attrib.group == "boomerang")
		{
			auto& boomerTimer = newItem.GetComponent<ProjectileComponent>();
			boomerTimer.boomTimer.Start();
		}
	}

	void UseSword()
	{

		auto player = Registry::Instance()->GetEntityByTag("player");
		auto& playerSprite = player.GetComponent<SpriteComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		const auto& playerHealth = player.GetComponent<HealthComponent>();

		auto sword = Registry::Instance()->GetEntityByTag("the_sword");
		auto& swordCollider = sword.GetComponent<BoxColliderComponent>();

		auto shield = Registry::Instance()->GetEntityByTag("the_shield");
		auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();

		if (KeyboardControlSystem::dir == UP)
		{
			playerSprite.srcRect.y = playerSprite.height * 5;
			playerCollider.offset.y = 60;

			swordCollider.height = 40;
			swordCollider.width = 10;
			swordCollider.offset.x = 60;
			swordCollider.offset.y = 12;

			shieldCollider.offset.y = 60;
		}

		if (KeyboardControlSystem::dir == RIGHT)
		{
			playerSprite.srcRect.y = playerSprite.height * 5;
			playerCollider.offset.x = 30;

			swordCollider.height = 10;
			swordCollider.width = 40;
			swordCollider.offset.x = 78;
			swordCollider.offset.y = 58;

			shieldCollider.offset.x = 70;
		}

		if (KeyboardControlSystem::dir == DOWN)
		{
			playerSprite.srcRect.y = playerSprite.height * 5;
			playerCollider.offset.y = 30;

			swordCollider.height = 40;
			swordCollider.width = 10;
			swordCollider.offset.x = 64;
			swordCollider.offset.y = 74;

			shieldCollider.offset.y = 60;
		}

		if (KeyboardControlSystem::dir == LEFT)
		{
			playerSprite.srcRect.y = playerSprite.height * 5;
			playerCollider.offset.x = 60;

			swordCollider.height = 10;
			swordCollider.width = 40;
			swordCollider.offset.x = 12;
			swordCollider.offset.y = 56;

			shieldCollider.offset.x = 64;
		}

		// Check to see if health is full for sword beam
		if (playerHealth.healthPercentage == (playerHealth.maxHearts * 2)) fullLife = true;
		else fullLife = false;
	}

	void UseMagicWand()
	{
		auto player = Registry::Instance()->GetEntityByTag("player");

		const auto& health = player.GetComponent<HealthComponent>();
		auto& sprite = player.GetComponent<SpriteComponent>();
		sprite.srcRect.y = sprite.height * 6;
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{
		if (!game.PlayerHold())
		{
			if (event.symbol == SDLK_SPACE && !KeyboardControlSystem::keyDown)
			{
				if (ItemSelectKeyboardControlSystem::itemType == WOOD_BOW || ItemSelectKeyboardControlSystem::itemType == MAGIC_BOW)
				{
					// Bow uses rupees, must be greater than zero!
					if (GameState::totalRupees > 0)
					{
						ItemAttrib bow("projectile", "items", 16, 16, 0, 64, 4, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
							glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 3000, false, false);

						UseItem(bow);

						// Play the arrow sound!
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "boomerang_arrow", 0, 1);
						// Subtract from the total rupees when using the bow!
						GameState::totalRupees--;
					}
					KeyboardControlSystem::keyDown = true;
				}
				else if (ItemSelectKeyboardControlSystem::itemType == BOMB)
				{
					ItemAttrib bomb("bomber", "items", 16, 16, 64, 112, 1, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
						glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 3000, false, false);
					if (GameState::totalBombs > 0)
					{
						UseItem(bomb);
						GameState::totalBombs--;
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "bomb_drop", 0, 1);
					}
					KeyboardControlSystem::keyDown = true;
				}
				else if (ItemSelectKeyboardControlSystem::itemType == CANDLE)
				{
					ItemAttrib candle("fire", "items", 16, 16, 0, 96, 1, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
						glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 500, false, false);

					UseItem(candle);
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "candle", 0, 1);
					KeyboardControlSystem::keyDown = true;
				}
				else if (ItemSelectKeyboardControlSystem::itemType == BOOMERANG)
				{
					if (!boomerangReturned)
					{
						ItemAttrib boomerang("boomerang", "items", 16, 16, 0, 112, 4, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(-40, -40),
							glm::vec2(8, 8), glm::vec2(8, 8), glm::vec2(8, 8), glm::vec2(8, 8), glm::vec2(10, 10), glm::vec2(10, 10), glm::vec2(20, 20), glm::vec2(18, 18), 3000, true, false);

						UseItem(boomerang);
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "boomerang_arrow", 0, 1);
						KeyboardControlSystem::keyDown = true;
					}
				}
				else if (ItemSelectKeyboardControlSystem::itemType == MAGIC_ROD)
				{
					ItemAttrib beam("projectile", "items", 16, 16, 64, 96, 4, glm::vec2(4, 4), glm::vec2(0, 60), glm::vec2(30, 60), glm::vec2(30, 60), glm::vec2(30, 60),
						glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 2000, false, false);

					UseMagicWand();
					UseItem(beam);
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "magic_rod", 0, 1);
					KeyboardControlSystem::keyDown = true;
				}
			}
			else if (event.symbol == SDLK_RSHIFT && !KeyboardControlSystem::keyDown && swordTimer.GetTicks() == 0)
			{
				// Do not use the sword if we do not have a sword
				if (game.HasSword())
				{
					UseSword();
					// If the player life is full, allow sword beam projectile
					if (fullLife)
					{
						// Create Sword beam projectile
						ItemAttrib beam("beam", "Sword", 32, 32, 0, 0, 4, glm::vec2(4, 4), glm::vec2(0, 45), glm::vec2(0, 25), glm::vec2(15, 20), glm::vec2(65, 20), glm::vec2(10, 40), glm::vec2(10, 40), glm::vec2(40, 10), glm::vec2(40, 10), glm::vec2(56, 30),
							glm::vec2(56, 40), glm::vec2(30, 60), glm::vec2(10, 60), 1000, true, true);
						UseItem(beam);
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "sword_shoot", 0, 1);
					}
					else
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "sword_slash", 0, 1);

					swordTimer.Start();
					KeyboardControlSystem::keyDown = true;
				}
			}
		}
	}

	void Update(Registry* registry)
	{
		for (auto& entity : GetSystemEntities())
		{
			// Make the boomerang come back to the player and destroy it when it returns
			if (entity.BelongsToGroup("boomerang"))
			{
				// Player variables
				auto player = Registry::Instance()->GetEntityByTag("player");
				auto& playerPos = player.GetComponent<TransformComponent>();

				// Boomerang variables
				auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
				auto& proj = entity.GetComponent<ProjectileComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();

				// Have the boomerang change direction based on the player position
				glm::vec2 direction = glm::normalize(playerPos.position - transform.position);

				if (proj.boomTimer.GetTicks() > 300) // TODO: Change the time based on type of boomerang-->wood/magic
				{
					rigidbody.velocity.x = direction.x * 300;
					rigidbody.velocity.y = direction.y * 300;
					boomerangReturned = true;
				}
				if (boomerangReturned)
				{
					bool setKill = false;

					// If the boomerang position is within any of these given parameters --> Kill it
					if (rigidbody.velocity.x > 0 && rigidbody.velocity.y > 0 &&
						transform.position.x >= playerPos.position.x - 32 && transform.position.y >= playerPos.position.y - 32)
						setKill = true;
					else if (rigidbody.velocity.x < 0 && rigidbody.velocity.y > 0 &&
						transform.position.x <= playerPos.position.x + 32 && transform.position.y >= playerPos.position.y - 32)
						setKill = true;
					else if (rigidbody.velocity.x > 0 && rigidbody.velocity.y < 0 &&
						transform.position.x >= playerPos.position.x - 32 && transform.position.y <= playerPos.position.y + 32)
						setKill = true;
					else if (rigidbody.velocity.x < 0 && rigidbody.velocity.y < 0 &&
						transform.position.x <= playerPos.position.x + 32 && transform.position.y <= playerPos.position.y + 32)
						setKill = true;
					else if (rigidbody.velocity.x > 0 && rigidbody.velocity.y == 0 && transform.position.x >= playerPos.position.x - 32)
						setKill = true;
					else if (rigidbody.velocity.x < 0 && rigidbody.velocity.y == 0 && transform.position.x <= playerPos.position.x + 32)
						setKill = true;
					else if (rigidbody.velocity.x == 0 && rigidbody.velocity.y > 0 && transform.position.y >= playerPos.position.y - 32)
						setKill = true;
					else if (rigidbody.velocity.x == 0 && rigidbody.velocity.y < 0 && transform.position.y <= playerPos.position.y + 32)
						setKill = true;

					if (setKill)
					{
						entity.Kill();
						boomerangReturned = false;
					}
				}
			}

			// This is the wait time for the player sword 
			if (swordTimer.GetTicks() > 250)
				swordTimer.Stop();

			// Check the enemy projectile 
			if (entity.BelongsToGroup("enemies"))
			{
				auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
				auto& projectileTransform = entity.GetComponent<TransformComponent>();
				auto& rigid = entity.GetComponent<RigidBodyComponent>();
				const auto& ai = entity.GetComponent<AIComponent>();

				if (projectileEmitter.shotTriggered && !projectileEmitter.shotFired)
				{
					Entity enemyProjectile = entity.registry->CreateEntity();
					enemyProjectile.Group("projectile");
					// Variables that can change based on the type of enemy
					int offsetX = 0;
					int offsetY = 0;
					int srcRectX = 0;
					int srcRectY = 0;
					int scale = 0;

					switch (ai.GetEnemyType())
					{
					case AIComponent::EnemyType::OCTOROK:
						srcRectX = 48;
						srcRectY = 0;
						scale = 2;
						break;

					case AIComponent::EnemyType::MOBLIN:
						// Set the srcRect for the projectile sprite
						srcRectY = 4 * 16;
						scale = 4;
						if (projectileEmitter.shootUp)
							srcRectX = 0 * 16;
						else if (projectileEmitter.shootRight)
							srcRectX = 1 * 16;
						else if (projectileEmitter.shootDown)
							srcRectX = 2 * 16;
						else if (projectileEmitter.shootLeft)
							srcRectX = 3 * 16;
						break;

					case AIComponent::EnemyType::ZORA:
						// TODO: Create a sprite sheet of enemy projectiles
						// enemyProjectile.AddComponent<AnimationComponent>(4, 20, false);
						break;
					default:
						break;
					}

					// Set the velocity and direction of the projectile based on when it 
					// was triggered
					if (projectileEmitter.shootUp)
					{
						projectileEmitter.projectileVelocity = glm::vec2(0, -200);
						projectileEmitter.shootUp = false;
						rigid.up = true;
						offsetX = 20;
						offsetY = 0;

					}
					else if (projectileEmitter.shootRight)
					{
						projectileEmitter.projectileVelocity = glm::vec2(200, 0);
						projectileEmitter.shootRight = false;
						rigid.right = true;
						offsetX = 0;
						offsetY = 20;
					}
					else if (projectileEmitter.shootDown)
					{
						projectileEmitter.projectileVelocity = glm::vec2(0, 200);
						projectileEmitter.shootDown = false;
						rigid.down = true;
						offsetX = 20;
						offsetY = 0;
					}
					else if (projectileEmitter.shootLeft)
					{
						projectileEmitter.projectileVelocity = glm::vec2(-200, 0);
						projectileEmitter.shootLeft = false;
						rigid.left = true;
						offsetX = 0;
						offsetY = 20;
					}

					enemyProjectile.AddComponent<TransformComponent>(glm::vec2(projectileTransform.position.x + offsetX, projectileTransform.position.y + offsetY), glm::vec2(scale, scale), 0.0);

					enemyProjectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
					enemyProjectile.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0, 0));

					projectileEmitter.isFriendly = false;
					enemyProjectile.AddComponent<SpriteComponent>("items", 16, 16, 2, false, srcRectX, srcRectY);
					enemyProjectile.AddComponent<ProjectileComponent>(false, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);
					enemyProjectile.AddComponent<GameComponent>();

					projectileEmitter.shotFired = true;
				}
			}
		}
	}
};