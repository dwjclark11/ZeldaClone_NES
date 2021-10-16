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

	glm::vec2 playerPosition;
	bool boomerangReturned;
	bool playerSet;
	GamePadSystem controller;

	ProjectileEmitterSystem()
	{
		RequiredComponent<ProjectileEmitterComponent>();
		RequiredComponent<TransformComponent>();

		playerPosition = glm::vec2(0);
		boomerangReturned = false;
		playerSet = false;
		magicBeam = false;
		fullLife = false;
		swordTimer.Stop();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitterSystem::OnKeyPressed);
		//eventManager->SubscribeToEvent<GamePadButtonPressedEvent>(this, &ProjectileEmitterSystem::OnButtonPressed);
	}

	void UseItem(ItemAttrib& attrib)
	{
		for (auto entity : GetSystemEntities())
		{
			if (entity.HasTag("player"))
			{
				// The playerSet is setting the position of the player for the boomerang to come back to in the update function
				if (!playerSet && attrib.group == "boomerang")
				{
					playerPosition = entity.GetComponent<TransformComponent>().position;
					playerSet = true;
				}

				auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
				const auto& transform = entity.GetComponent<TransformComponent>();
				const auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

				// If the entity has a sprite --> Position the projectile in the center
				glm::vec2 projectilePosition = transform.position;

				auto sprite = entity.GetComponent<SpriteComponent>();
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
				Entity newItem = entity.registry->CreateEntity();
				newItem.Group(attrib.group);
				newItem.AddComponent<TransformComponent>(projectilePosition, glm::vec2(attrib.scale.x, attrib.scale.y), 0.0);

				if (attrib.group != "bomber")
				{
					// Set the projectile velocity based on the direction 
					projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
					projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;
					newItem.AddComponent<RigidBodyComponent>(projectileVelocity);
					newItem.AddComponent<BoxColliderComponent>((int)BoxSize.x / transform.scale.x, (int)BoxSize.y / transform.scale.y, glm::vec2(BoxOffset.x, BoxOffset.y));
				}else Logger::Log("Bomber");
				// Does the projectile have an animation component?
				if (attrib.animation)
				{
					// TODO: Change the frame speed from hard coded to individual --> attrib.frame_speed
					newItem.AddComponent<AnimationComponent>(attrib.numFrames, 20, attrib.vertical, true);
				}
				projectileEmitter.isFriendly = true;
				newItem.AddComponent<SpriteComponent>(attrib.sprite_name, attrib.width, attrib.height, 2, false, attrib.srcRectX, attrib.srcRectY);
				newItem.AddComponent<ProjectileEmitterComponent>(projectileEmitter.projectileVelocity, 0,attrib.duration, projectileEmitter.hitPercentDamage, projectileEmitter.isFriendly);
				newItem.AddComponent<ProjectileComponent>(true, 10, attrib.duration);
				newItem.AddComponent<GameComponent>();
			}
		}
	}
	void UseSword()
	{
		for (auto entity : GetSystemEntities())
		{
			
			// Change the sprite srcRect based on the direction facing
			if (entity.HasTag("player"))
			{
				auto entity = Registry::Instance()->GetEntityByTag("player");
				const auto health = entity.GetComponent<HealthComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();
				auto& box = entity.GetComponent<BoxColliderComponent>();

				if (KeyboardControlSystem::dir == UP)
				{
					sprite.srcRect.y = sprite.height * 5;
					box.offset.y = 60;
				}

				if (KeyboardControlSystem::dir == RIGHT)
				{
					sprite.srcRect.y = sprite.height * 5;
					box.offset.x = 30;
				}

				if (KeyboardControlSystem::dir == DOWN)
				{
					sprite.srcRect.y = sprite.height * 5;
					box.offset.y = 30;
				}

				if (KeyboardControlSystem::dir == LEFT)
				{
					sprite.srcRect.y = sprite.height * 5;
					box.offset.x = 60;
				}

				// Check to see if health is full for sword beam
				if (health.healthPercentage == (health.maxHearts * 2)) fullLife = true;
				else fullLife = false;
			}
			// Set the size and Position of the Sword BoxCollider based on direction facing
			if (entity.HasTag("the_sword"))
			{
				auto& box = entity.GetComponent<BoxColliderComponent>();

				if (KeyboardControlSystem::dir == UP)
				{
					box.height = 40;
					box.width = 10;
					box.offset.x = 60;
					box.offset.y = 12;
				}
				if (KeyboardControlSystem::dir == RIGHT)
				{
					box.height = 10;
					box.width = 40;
					box.offset.x = 78;
					box.offset.y = 58;
				}
				if (KeyboardControlSystem::dir == DOWN)
				{
					box.height = 40;
					box.width = 10;
					box.offset.x = 64;
					box.offset.y = 74;
				}
				if (KeyboardControlSystem::dir == LEFT)
				{
					box.height = 10;
					box.width = 40;
					box.offset.x = 12;
					box.offset.y = 56;
				}
			}

			if (entity.HasTag("the_shield"))
			{
				auto& box = entity.GetComponent<BoxColliderComponent>();

				if (KeyboardControlSystem::dir == UP)
				{
					box.offset.y = 60;
				}
				if (KeyboardControlSystem::dir == RIGHT)
				{
					box.offset.x = 70;
				}
				if (KeyboardControlSystem::dir == DOWN)
				{
					box.offset.y = 60;
				}
				if (KeyboardControlSystem::dir == LEFT)
				{
					box.offset.x = 64;
				}
			}
		}
	}
	
	void UseMagicWand()
	{
		for (auto entity : GetSystemEntities())
		{
			// Change the sprite srcRect position based on direction we are facing
			if (entity.HasTag("player"))
			{
				const auto health = entity.GetComponent<HealthComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();

				if (KeyboardControlSystem::dir == UP)
				{
					sprite.srcRect.y = sprite.height * 3;
				}

				if (KeyboardControlSystem::dir == RIGHT)
				{
					sprite.srcRect.y = sprite.height * 3;
				}

				if (KeyboardControlSystem::dir == DOWN)
				{
					sprite.srcRect.y = sprite.height * 3;
				}

				if (KeyboardControlSystem::dir == LEFT)
				{
					sprite.srcRect.y = sprite.height * 3;
				}
			}
		}
	}
	
	void OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.symbol == SDLK_SPACE && !KeyboardControlSystem::keyDown)
		{
			if (ItemSelectKeyboardControlSystem::itemType == WOOD_BOW || ItemSelectKeyboardControlSystem::itemType == MAGIC_BOW)
			{
				ItemAttrib bow("projectile", "items", 16, 16, 0, 64, 4, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
					glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 3000, false, false);
				
				UseItem(bow);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "boomerang_arrow", 0, 1);
				KeyboardControlSystem::keyDown = true;
			}
			else if (ItemSelectKeyboardControlSystem::itemType == BOMB)
			{
				ItemAttrib bomb("bomber", "items", 16, 16, 64, 112, 1, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
					glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 3000, false, false);
				
				UseItem(bomb);
				GameState::totalBombs--;
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "bomb_drop", 0, 1);
				KeyboardControlSystem::keyDown = true;
			}
			else if (ItemSelectKeyboardControlSystem::itemType == CANDLE)
			{
				ItemAttrib candle("projectile", "items", 16, 16, 0, 96, 1, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0),
					glm::vec2(16, 16), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 500, false, false);
				
				UseItem(candle);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "candle", 0, 1);
				KeyboardControlSystem::keyDown = true;
			}
			else if (ItemSelectKeyboardControlSystem::itemType == BOOMERANG)
			{
				if (!boomerangReturned && !playerSet)
				{
					ItemAttrib boomerang("boomerang", "items", 16, 16, 0, 112, 4, glm::vec2(4, 4), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(-40, -40),
						glm::vec2(8, 8), glm::vec2(8, 8), glm::vec2(8, 8), glm::vec2(8, 8), glm::vec2(10, 10), glm::vec2(10, 10), glm::vec2(20, 20), glm::vec2(18, 18), 3000, true, false);
					
					UseItem(boomerang);
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "boomerang_arrow", 0, 1);
					KeyboardControlSystem::keyDown = true;
				}
			}
			else if (ItemSelectKeyboardControlSystem::itemType == MAGIC_ROD)
			{
				ItemAttrib beam("projectile", "items", 16, 16, 64, 96, 4, glm::vec2(4, 4), glm::vec2(0, 60), glm::vec2(30, 60), glm::vec2(30, 60), glm::vec2(30, 60),
					glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), 2000, false, false);
				
				UseMagicWand();
				UseItem(beam);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "magic_rod", 0, 1);
				KeyboardControlSystem::keyDown = true;
			}
		}
		else if (event.symbol == SDLK_RSHIFT && !KeyboardControlSystem::keyDown && swordTimer.GetTicks() == 0)
		{
			UseSword();
			// If the player life is full, allow sword beam projectile
			if (fullLife)
			{
				// Create Sword beam projectile
				ItemAttrib beam("projectile", "Sword", 16, 16, 0, 0, 4, glm::vec2(4, 4), glm::vec2(35, 45), glm::vec2(40, 25), glm::vec2(15, 30), glm::vec2(65, 30),
					glm::vec2(10, 40), glm::vec2(10, 40), glm::vec2(40, 10), glm::vec2(40, 10), glm::vec2(25, 0), glm::vec2(25, 20), glm::vec2(15, 30), glm::vec2(10, 30), 1000, true, true);

				UseItem(beam);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "sword_shoot", 0, 1);
			}
			else
			{
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "sword_slash", 0, 1);
			}
			
			swordTimer.Start();
			KeyboardControlSystem::keyDown = true;
		}
	}
	
	void UpdateGamePad()
	{
		if (GamePadSystem::xPressed && !GamePadSystem::buttonDown)
		{
			if (ItemSelectKeyboardControlSystem::itemType == WOOD_BOW || ItemSelectKeyboardControlSystem::itemType == MAGIC_BOW)
			{
				/*ItemAttrib bow("projectile", "items", 16, 16, 0, 64, 4, 4, 4, 64, 64, 0, 0, 0, 0, 0);
				UseItem(bow);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "boomerang_arrow", 0, 1);
				GamePadSystem::buttonDown = true;*/
			}
			else if (ItemSelectKeyboardControlSystem::itemType == CANDLE)
			{
				/*ItemAttrib bomb("bomber", "items", 16, 16, 64, 112, 1, 4, 4, 0, 0, 0, 0, 0, 0, 0);
				UseItem(bomb);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "bomb_drop", 0, 1);
				GamePadSystem::buttonDown = true;*/
			}
			else if (ItemSelectKeyboardControlSystem::itemType == BOOMERANG)
			{
				/*ItemAttrib candle("projectile", "items", 16, 16, 0, 96, 1, 4, 4, 0, 0, 500, 14, 14, 5, 5);
				UseItem(candle);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "candle", 0, 1);
				GamePadSystem::buttonDown = true;*/
			}
			else if (ItemSelectKeyboardControlSystem::itemType == MAGIC_ROD)
			{
				/*ItemAttrib beam("projectile", "items", 16, 16, 64, 96, 4, 4, 4, 0, 0, 2000, 16, 16, 0, 0);
				UseMagicWand();
				UseItem(beam);
				Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "magic_rod", 0, 1);
				GamePadSystem::buttonDown = true;*/
			} 
		}
	}
	
	//void UpdateBoomerang()
	//{
	//	auto entity = Registry::Instance()->GetEntitiesByGroup("boomerang");
	//	
	//	// Make the boomerang come back to the player and destroy it when it returns
	//	
	///*	if (entity.BelongsToGroup("boomerang"))
	//	{*/
	//		auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
	//		//Logger::Log("Boomer Pos.y: " + std::to_string(entity.GetComponent<TransformComponent>().position.y) + "PlayerPos.y: " + std::to_string(playerPosition.y));
	//		if (KeyboardControlSystem::dir == UP)
	//		{
	//			if (entity.GetComponent<TransformComponent>().position.y < (playerPosition.y - 200) && !boomerangReturned)
	//			{
	//				rigidbody.velocity *= -1;
	//				boomerangReturned = true;
	//			}
	//			else if (entity.GetComponent<TransformComponent>().position.y >= (playerPosition.y - 64) && boomerangReturned)
	//			{
	//				entity.Kill();
	//				boomerangReturned = false;
	//				playerSet = false;
	//			}
	//		}
	//		else if (KeyboardControlSystem::dir == RIGHT)
	//		{
	//			if (entity.GetComponent<TransformComponent>().position.x > (playerPosition.x + 200) && !boomerangReturned)
	//			{
	//				rigidbody.velocity *= -1;
	//				boomerangReturned = true;
	//			}
	//			else if (entity.GetComponent<TransformComponent>().position.x <= (playerPosition.x + 64) && boomerangReturned)
	//			{
	//				entity.Kill();
	//				boomerangReturned = false;
	//				playerSet = false;
	//			}
	//		}
	//		else if (KeyboardControlSystem::dir == DOWN)
	//		{
	//			if (entity.GetComponent<TransformComponent>().position.y > (playerPosition.y + 200) && !boomerangReturned)
	//			{
	//				rigidbody.velocity *= -1;
	//				boomerangReturned = true;
	//			}
	//			else if (entity.GetComponent<TransformComponent>().position.y <= (playerPosition.y + 64) && boomerangReturned)
	//			{
	//				entity.Kill();
	//				boomerangReturned = false;
	//				playerSet = false;
	//			}
	//		}
	//		else if (KeyboardControlSystem::dir == LEFT)
	//		{
	//			if (entity.GetComponent<TransformComponent>().position.x < (playerPosition.x - 200) && !boomerangReturned)
	//			{
	//				rigidbody.velocity *= -1;
	//				boomerangReturned = true;
	//			}
	//			else if (entity.GetComponent<TransformComponent>().position.x >= (playerPosition.x - 64) && boomerangReturned)
	//			{
	//				entity.Kill();
	//				boomerangReturned = false;
	//				playerSet = false;
	//			}
	//		}
	//	//}
	//}
	
	void Update(Registry* registry)
	{
		//UpdateBoomerang();
		for (auto entity : GetSystemEntities())
		{
			// Make the boomerang come back to the player and destroy it when it returns
			if (entity.BelongsToGroup("boomerang"))
			{
				auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
				//Logger::Log("Boomer Pos.y: " + std::to_string(entity.GetComponent<TransformComponent>().position.y) + "PlayerPos.y: " + std::to_string(playerPosition.y));
				if (KeyboardControlSystem::dir == UP)
				{
					if (entity.GetComponent<TransformComponent>().position.y < (playerPosition.y - 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.y >= (playerPosition.y - 64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
				else if (KeyboardControlSystem::dir == RIGHT)
				{
					if (entity.GetComponent<TransformComponent>().position.x > (playerPosition.x + 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.x <= (playerPosition.x + 64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
				else if (KeyboardControlSystem::dir == DOWN)
				{
					if (entity.GetComponent<TransformComponent>().position.y > (playerPosition.y + 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.y <= (playerPosition.y + 64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
				else if (KeyboardControlSystem::dir == LEFT)
				{
					if (entity.GetComponent<TransformComponent>().position.x < (playerPosition.x - 200) && !boomerangReturned)
					{
						rigidbody.velocity *= -1;
						boomerangReturned = true;
					}
					else if (entity.GetComponent<TransformComponent>().position.x >= (playerPosition.x - 64) && boomerangReturned)
					{
						entity.Kill();
						boomerangReturned = false;
						playerSet = false;
					}
				}
			}
			// This is the wait time for the player sword 
			if (swordTimer.GetTicks() > 250) swordTimer.Stop();

			// Check the enemy projectile 
			if (entity.BelongsToGroup("enemies"))
			{
				auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
				auto& projectileTransform = entity.GetComponent<TransformComponent>();
				auto& rigid = entity.GetComponent<RigidBodyComponent>();
				int offsetX = 0;
				int offsetY = 0;
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

				if (projectileEmitter.shotTriggered && !projectileEmitter.shotFired)
				{
					// Create new projectile entity and add it to the world
					Entity enemyProjectile = entity.registry->CreateEntity();
					enemyProjectile.Group("projectile");
					enemyProjectile.AddComponent<TransformComponent>(glm::vec2(projectileTransform.position.x + offsetX, projectileTransform.position.y + offsetY), glm::vec2(2, 2), 0.0);

					enemyProjectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
					enemyProjectile.AddComponent<BoxColliderComponent>(16, 16, glm::vec2(0, 0));

					/* 				// Does the projectile have an animation component?
									if (attrib.animation)
									{
										// TODO: Change the frame speed from hard coded to individual --> attrib.frame_speed
										newItem.AddComponent<AnimationComponent>(attrib.numFrames, 20, attrib.vertical, true);
									} */
					projectileEmitter.isFriendly = false;
					enemyProjectile.AddComponent<SpriteComponent>("items", 16, 16, 2, false, 48, 0);
					enemyProjectile.AddComponent<ProjectileEmitterComponent>(projectileEmitter.projectileVelocity, 0, projectileEmitter.projectileDuration, projectileEmitter.hitPercentDamage, projectileEmitter.isFriendly);
					enemyProjectile.AddComponent<ProjectileComponent>(false, 10, projectileEmitter.projectileDuration);
					enemyProjectile.AddComponent<GameComponent>();

					projectileEmitter.shotFired = true;
				}	
			}
		}
	}
};