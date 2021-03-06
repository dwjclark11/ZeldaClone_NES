#include "TriggerSystem.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SceneChangeComponent.h"
#include "../../Components/SecretComponent.h"
#include "../../Systems/GameSystems/RenderTileSystem.h"
#include "../../Systems/GameSystems/KeyboardControlSystem.h"
#include "../../Systems/RenderTextSystem.h"
#include "../../Events/CollisionEvent.h"
#include "../../Events/TriggerEvent.h"
#include "../../Events/EventManager.h"
#include "../../Game/Game.h"

#include "../../Utilities/Timer.h"
#include "../SoundFXSystem.h"
#include "../../States/GameState.h"


bool TriggerSystem::CheckInventory(ItemComponent::SpecialItemType& item)
{
	// Check to see the item is in the inventory
	switch (item)
	{
	case ItemComponent::SpecialItemType::WOOD_SWORD:
		if (game.GetGameItems().woodSword)
			return true;
		break;
	case ItemComponent::SpecialItemType::STEEL_SWORD:
		if (game.GetGameItems().steelSword)
			return true;
		break;
	case ItemComponent::SpecialItemType::MAGIC_SWORD:
		if (game.GetGameItems().magicSword)
			return true;
		break;
	case ItemComponent::SpecialItemType::RED_CANDLE:
		if (game.GetGameItems().candle)
			return true;
		break;
	case ItemComponent::SpecialItemType::WOOD_BOW:
		if (game.GetGameItems().bow)
			return true;
		break;
	default:
		return false;
		break;
	}

	// If the item is not in the inventory return false
	return false;
}

void TriggerSystem::SetInventory(ItemComponent::SpecialItemType& item)
{
	auto player = Registry::Instance().GetEntityByTag("player");

	auto& playerHealth = player.GetComponent<HealthComponent>();

	switch (item)
	{
	case ItemComponent::SpecialItemType::NOT_SPECIAL:
		break;

	case ItemComponent::SpecialItemType::WOOD_SWORD:
		game.GetGameItems().woodSword = true;
		break;

	case ItemComponent::SpecialItemType::STEEL_SWORD:
		game.GetGameItems().steelSword = true;
		break;

	case ItemComponent::SpecialItemType::MAGIC_SWORD:
		game.GetGameItems().magicSword = true;
		break;

	case ItemComponent::SpecialItemType::FULL_HEART:
	{
		playerHealth.addHeart = true;
		break;
	}
	case ItemComponent::SpecialItemType::RAFT:
		game.GetGameItems().raft = true;
		break;
	case ItemComponent::SpecialItemType::POWER_BRACLET:
		game.GetGameItems().powerBraclet = true;
		break;
	case ItemComponent::SpecialItemType::RED_CANDLE:
		game.GetGameItems().candle = true;
		break;
	case ItemComponent::SpecialItemType::WOOD_BOOMERANG:
		game.GetGameItems().woodBoomerang = true;
		break;
	case ItemComponent::SpecialItemType::MAGIC_BOOMERANG:
		game.GetGameItems().magicBoomerang = true;
		break;
	case ItemComponent::SpecialItemType::LADDER:
		game.GetGameItems().ladder = true;
		break;
	case ItemComponent::SpecialItemType::WOOD_BOW:
		game.GetGameItems().bow = true;
		break;
	default:
		break;
	}
}

void TriggerSystem::StopPlayerMovement(Entity& player, Entity& trigger)
{
	if (player.HasComponent<TransformComponent>() && player.HasComponent<BoxColliderComponent>())
	{
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();

		auto& obstacleTransform = trigger.GetComponent<TransformComponent>();
		auto& obstacleCollider = trigger.GetComponent<BoxColliderComponent>();

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

void TriggerSystem::SecretTrigger(Entity& trigger, bool startup)
{
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& secretCollider = trigger.GetComponent<BoxColliderComponent>();
	auto& secretTrigger = trigger.GetComponent<TriggerBoxComponent>();
	auto& secretTransform = trigger.GetComponent<TransformComponent>();

	if (!secret.found && !startup)
	{
		TriggerBoxComponent::TriggerType trigType = loader.ConvertStringToTriggerType(secret.newTrigger);
		
		auto secretArea = Registry::Instance().CreateEntity();
		secretArea.Group("trigger");
		secretArea.AddComponent<BoxColliderComponent>(secretCollider.width, secretCollider.height, secretCollider.offset);
	
		secretArea.AddComponent<TriggerBoxComponent>(trigType,
			secretTrigger.transportOffset, secretTrigger.cameraOffset);
		
		if (trigger.HasComponent<SceneChangeComponent>())
		{
			const auto& scene = trigger.GetComponent<SceneChangeComponent>();
			secretArea.AddComponent<SceneChangeComponent>(
				scene.levelMusic, scene.assetFile, scene.enemyFile,
				scene.colliderFile, scene.tileMapName, scene.tileImageName,
				scene.mapImageName, scene.entityFileName, scene.triggerFile,
				scene.imageWidth, scene.imageHeight);
		}


		secretArea.AddComponent<TransformComponent>(secretTransform.position, secretTransform.scale, secretTransform.rotation);
		secretArea.AddComponent<SpriteComponent>(secret.newSpriteAssetID, secret.spriteWidth, secret.spriteHeight, 1, false, secret.spriteSrcX, secret.spriteSrcY);
		secretArea.AddComponent<GameComponent>();

		secret.found = true;
		game.SetSecretFound(secret.locationID, true);
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "secret", 0, -1);

	
	}
	else if (secret.found && startup)
	{
		TriggerBoxComponent::TriggerType trigType = loader.ConvertStringToTriggerType(secret.newTrigger);

		auto secretArea = Registry::Instance().CreateEntity();
		secretArea.Group("trigger");
		secretArea.AddComponent<BoxColliderComponent>(secretCollider.width, secretCollider.height, secretCollider.offset);
		secretArea.AddComponent<TriggerBoxComponent>(trigType,
			secretTrigger.transportOffset, secretTrigger.cameraOffset);


		if (trigger.HasComponent<SceneChangeComponent>())
		{
			const auto& scene = trigger.GetComponent<SceneChangeComponent>();
			secretArea.AddComponent<SceneChangeComponent>(
				scene.levelMusic, scene.assetFile, scene.enemyFile,
				scene.colliderFile, scene.tileMapName, scene.tileImageName,
				scene.mapImageName, scene.entityFileName, scene.triggerFile,
				scene.imageWidth, scene.imageHeight);
		}

		secretArea.AddComponent<TransformComponent>(secretTransform.position, secretTransform.scale, secretTransform.rotation);
		secretArea.AddComponent<SpriteComponent>(secret.newSpriteAssetID, secret.spriteWidth, secret.spriteHeight, 1, false, secret.spriteSrcX, secret.spriteSrcY);
		secretArea.AddComponent<GameComponent>();
	}
	trigger.Kill();
}

TriggerSystem::TriggerSystem()
	: game(Game::Instance())
	, angle(0.f)
	, heartOffset(0.f)
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<BoxColliderComponent>();
	RequiredComponent<TriggerBoxComponent>();
}


void TriggerSystem::SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
{
	eventManager->SubscribeToEvent<CollisionEvent>(this, &TriggerSystem::OnTrigger);
}

void TriggerSystem::OnTrigger(CollisionEvent& event)
{
	Entity& a = event.a;
	Entity& b = event.b;

	// Check to see if the player has activated a trigger
	if (!a.HasComponent<SecretComponent>() && a.HasComponent<TriggerBoxComponent>() && b.HasTag("player"))
	{
		OnEnterTrigger(b, a);
	}

	if (!b.HasComponent<SecretComponent>() && b.HasComponent<TriggerBoxComponent>() && a.HasTag("player"))
	{
		OnEnterTrigger(a, b);
	}

	// Check for secret Component 
	if (b.HasComponent<SecretComponent>())
	{
		auto& trig = b.GetComponent<TriggerBoxComponent>();
		if (trig.triggerType == TriggerBoxComponent::TriggerType::BOMB_SECRET && a.BelongsToGroup("explode"))
			OnEnterTrigger(a, b);
		if (trig.triggerType == TriggerBoxComponent::TriggerType::BURN_BUSHES && a.BelongsToGroup("fire"))
			OnEnterTrigger(a, b);
		if (trig.triggerType == TriggerBoxComponent::TriggerType::PUSH_ROCKS && a.HasTag("player"))
			OnEnterTrigger(a, b);
		if (trig.triggerType == TriggerBoxComponent::TriggerType::RAFT && a.HasTag("player"))
			OnEnterTrigger(a, b);
	}

	if (a.HasComponent<SecretComponent>())
	{
		auto& trig = a.GetComponent<TriggerBoxComponent>();
		if (trig.triggerType == TriggerBoxComponent::TriggerType::BOMB_SECRET && b.BelongsToGroup("explode"))
			OnEnterTrigger(b, a);
		if (trig.triggerType == TriggerBoxComponent::TriggerType::BURN_BUSHES && b.BelongsToGroup("fire"))
			OnEnterTrigger(b, a);
		if (trig.triggerType == TriggerBoxComponent::TriggerType::PUSH_ROCKS && b.HasTag("player"))
			OnEnterTrigger(b, a);
		if (trig.triggerType == TriggerBoxComponent::TriggerType::RAFT && b.HasTag("player"))
			OnEnterTrigger(b, a);
	}
}

void TriggerSystem::OnEnterTrigger(Entity& player, Entity& trigger)
{
	// Get needed componenets from the triger
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	float x = trig.transportOffset.x;
	float y = trig.transportOffset.y;

	// Get the needed componenets from the player
	auto& playerTransform = player.GetComponent<TransformComponent>();
	Timer loadTimer;
	
	switch (trig.triggerType)
	{
	case TriggerBoxComponent::TriggerType::NO_TRIGGER:
		Logger::Err("TRIGGER_SYSTEM: __LINE: 174 - There is not trigger set for this entity");
		break;

	case TriggerBoxComponent::TriggerType::SCENE_CHANGE: 
	{
		if (game.GetStairsFinished())
		{
			auto _player = Registry::Instance().GetEntityByTag("player");

			auto& rigidbody = _player.GetComponent<RigidBodyComponent>();
			auto& sprite = _player.GetComponent<SpriteComponent>();
			auto& scene = trigger.GetComponent<SceneChangeComponent>();
			// Stop the player movement during the scene transition
			rigidbody.velocity = glm::vec2(0);

			if (trig.collectedTimer.GetTicks() == 0)
			{
				trig.collectedTimer.Start();
			}

			// If the Scene has not faded, start the fade transition
			if (game.GetFadeAlpha() > 0)
			{
				game.StartFadeOut(true);
			}
			else
			{
				// Remove all the prior assets/entities from the current scene
				Registry::Instance().GetSystem<RenderSystem>().OnExit();
				Registry::Instance().GetSystem<RenderTileSystem>().OnExit();
				Registry::Instance().GetSystem<RenderCollisionSystem>().OnExit();
				Registry::Instance().GetSystem<RenderTextSystem>().OnExit();
				
				// Check to see if the trigger has "no_file" assiged if it has a file load the assets for the scene
				if (scene.assetFile != "no_file")
					loader.LoadAssetsFromLuaTable(game.GetLuaState(), scene.assetFile);

				// load the tilemap only if there is an image and a corresponding map
				if (scene.tileMapName != "no_file" && scene.mapImageName != "no_file")
				{
					std::string mapFile = "Assets/Tilemaps/Maps/" + scene.tileMapName + ".map";
					
					loader.LoadTilemap(mapFile, scene.mapImageName);
				}
				
				// If there is only an image name than it is a full map image, not tiles --> Just load the map
				if (scene.tileImageName != "no_file")
				{
					//Logger::Log("Map: Width[" + std::to_string(scene.imageWidth) + "], Height[" + std::to_string(scene.imageHeight) + "]");
					loader.LoadMap(scene.tileImageName, scene.imageWidth, scene.imageHeight);
				}
					
				// Start the new scene's music || stop the music
				if (scene.levelMusic != "stop")
					Registry::Instance().GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), scene.levelMusic, -1);
				else
					Registry::Instance().GetSystem<MusicPlayerSystem>().StopMusic();

				// If there is an entity file, NPCs, Store Items, etc. Load it
				if (scene.entityFileName != "no_file")
					loader.LoadEntitiesFromLuaTable(game.GetLuaState(), scene.entityFileName);

				// Adjust the camera location to the trigger offset
				game.GetCamera().x = trig.cameraOffset.x;
				game.GetCamera().y = trig.cameraOffset.y;

				// Check to see if there is a collider file
				if (scene.colliderFile != "no_file")
					loader.LoadColliders(scene.colliderFile);
				// Check to see if there is a trigger file
				if (scene.triggerFile != "no_file")
				{
					Logger::Log("Trig " + scene.triggerFile);
					loader.LoadTriggers(game.GetLuaState(), scene.triggerFile);
				}

				// Check to see if there is an enemy file
				if (scene.enemyFile != "no_file")
					loader.LoadEnemiesFromLuaTable(game.GetLuaState(), scene.enemyFile);

				// Load the secrets of the scene that have been found
				loader.ReadInSecrets(game.GetLuaState());
				
				// Set player Position
				_player.GetComponent<TransformComponent>().position.x = x;
				_player.GetComponent<TransformComponent>().position.y = y;

				// Finish the fade screen 
				game.StartFadeOut(false);
				game.StartFadeIn(true);
				// Remove the current trigger
				trigger.Kill();
				//Logger::Log("Image: " + tileImageName);
				game.SetStairsFinished(false);
				// Set player sprite back to start
				sprite.srcRect.y = 0;

			}
		}
		else
		{
			game.SetPlayerOnStairs(true);
		}

		break;
	}
	case TriggerBoxComponent::TriggerType::TRANSPORT:
		
		if (game.GetFadeAlpha() > 0)
		{
			game.StartFadeOut(true);
		}
		else
		{
	
			playerTransform.position = trig.transportOffset;
			game.GetCamera().x = trig.cameraOffset.x;
			game.GetCamera().y = trig.cameraOffset.y;
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "stairs", 0, -1);

			// Finish the fade screen 
			game.StartFadeOut(false);
			game.StartFadeIn(true);

		}
		break;
	
	case TriggerBoxComponent::TriggerType::BURN_BUSHES:

		if (trigger.HasComponent<SecretComponent>())
		{
			SecretTrigger(trigger);
			// After the new trigger is created, remove the current trigger!
			trigger.Kill();
		}
		break;

	case TriggerBoxComponent::TriggerType::PUSH_ROCKS:
	{
		auto& playerRigidBody = player.GetComponent<RigidBodyComponent>();
		auto& rockTransform = trigger.GetComponent<TransformComponent>();
		auto& secret = trigger.GetComponent<SecretComponent>();

		if (!trig.active && !secret.found)
		{
			if (playerRigidBody.up)
			{
				trig.active = true;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "secret", 0, -1);
				secret.moveUp = true;
			}
			else if (playerRigidBody.down)
			{
				trig.active = true;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "secret", 0, -1);
				secret.moveDown = true;
			}
		}
	}
		break;

	case TriggerBoxComponent::TriggerType::COLLECT_ITEM:
	{
		if (!trig.active)
		{
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "fanfare", 0, 1);
			game.SetPlayerItem(true);
			trig.active = true;
		}
		break;
	}
	case TriggerBoxComponent::TriggerType::BOMB_SECRET:
		if (trigger.HasComponent<SecretComponent>())
		{
			SecretTrigger(trigger);

			// Check to see if there is a Trap door or something else to remove
			const auto& removeTag = trig.entityRemoveTag;
			if (removeTag != "")
			{
				if (Registry::Instance().DoesTagExist(removeTag))
				{
					auto removedEntity = Registry::Instance().GetEntityByTag(removeTag);

					SecretTrigger(removedEntity);
					// Remove the entity
					removedEntity.Kill();
				}
			}


			// After the new trigger is created, remove the current trigger!
			trigger.Kill();
		}
		break;

	case TriggerBoxComponent::TriggerType::LOCKED_DOOR:
		
		// Unlock the door if keys are available
		if (GameState::totalKeys > 0)
		{
			// Play door open sound
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "door_unlock", 0, 1);
			// Decrement total number of keys
			GameState::totalKeys--;

			// Check to see if there is a sister door to remove
			const auto& removeTag = trig.entityRemoveTag;
			if (removeTag != "")
			{
				auto removedEntity = Registry::Instance().GetEntityByTag(removeTag);
				
				// Remove/Kill the sister door
				removedEntity.Kill();
			}

			// Remove the keyed door
			trigger.Kill();
		}
		
		break;
	case TriggerBoxComponent::TriggerType::SHOP_ITEM:
		// Check to see if the trigger has an Item Components
		if (trigger.HasComponent<ItemComponent>() && GameState::scrollRupees == 0)
		{
			auto& shopItem = trigger.GetComponent<ItemComponent>();
			// Check to see if the player has enough rupees
			if (GameState::totalRupees < shopItem.price)
			{
				// Maybe make a cancel sound?
				Logger::Log("Player Does not Have Enough Rupees!");
				break;
			}

			// Check to see if key item and if it is already in the inventory
			if (shopItem.special != ItemComponent::SpecialItemType::NOT_SPECIAL)
			{
				// Check to see if the item is in the inventory
				if (CheckInventory(shopItem.special))
				{
					Logger::Log("Item is Already in the inventory!");
					break;
				}
			}

			// Test --> Collect the item
			if (!trig.active)
			{
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "fanfare", 0, -1);
				game.SetPlayerItem(true);
				trig.active = true;
				GameState::scrollRupees = shopItem.price;
				GameState::buyItem = true;
			}
		}
		else
		{
			Logger::Log("TRIGGER: __SHOP_ITEM: All Shop Items must have an ItemComponent!");
			break;
		}
		break;
	case TriggerBoxComponent::TriggerType::RAFT:
	{
		auto& trigs = trigger.GetComponent<TriggerBoxComponent>();
		auto& secret = trigger.GetComponent<SecretComponent>();
		const auto& pTran = player.GetComponent<TransformComponent>();

		if (game.GetGameItems().raft)
		{
			if ((secret.moveDown && KeyboardControlSystem::dir == DOWN) || (secret.moveUp && KeyboardControlSystem::dir == UP))
			{
				if (!trigs.active)
				{
					StopPlayerMovement(player, trigger);
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "secret", 0, -1);
					auto raft = Registry::Instance().CreateEntity();
					raft.Tag("raft");
					raft.AddComponent<TransformComponent>(pTran);
					raft.AddComponent<SpriteComponent>("items", 16, 16, 1, false, 0, 16);
					raft.AddComponent<GameComponent>();
					raft.GetComponent<TransformComponent>().position.x += 32;
					raft.GetComponent<TransformComponent>().position.y += 32;
				}
					
				trigs.active = true;
				game.SetOnRaft(true);
			}
		}
		else
		{
			StopPlayerMovement(player, trigger);
		}
		break;
	}
	case TriggerBoxComponent::TriggerType::LADDER:
	{
		auto& trigs = trigger.GetComponent<TriggerBoxComponent>();
		auto& trigTransform = trigger.GetComponent<TransformComponent>();

		if (game.GetGameItems().ladder)
		{
			if (!trigs.active)
			{
				auto xOffset = 64;
				auto yOffset = 64;

				if (KeyboardControlSystem::dir == UP)
				{
					xOffset = 32;
					yOffset = -64;
				}
				else if (KeyboardControlSystem::dir == DOWN)
				{
					xOffset = 32;
					yOffset = 64;
				}
				else if (KeyboardControlSystem::dir == LEFT)
				{
					xOffset = -16;
					yOffset = 32;
				}
				else if (KeyboardControlSystem::dir == RIGHT)
				{
					xOffset = 64;
					yOffset = 32;
				}

				// Create a temporary ladder entity
				auto ladder = Registry::Instance().CreateEntity();
				ladder.Tag("ladder");
				ladder.AddComponent<TransformComponent>(playerTransform);
				ladder.AddComponent<SpriteComponent>("items", 16, 16, 1, false, 112, 48);
				ladder.AddComponent<GameComponent>();
				ladder.GetComponent<TransformComponent>().position.x += xOffset;
				ladder.GetComponent<TransformComponent>().position.y += yOffset;
				trigs.active = true;
			}
		}
		else
		{
			StopPlayerMovement(player, trigger);
		}
		break;
	}
	case TriggerBoxComponent::TriggerType::TRAP_DOOR:
	{
		Logger::Err("Touching a trap door!!");
		break;
	}
	case TriggerBoxComponent::TriggerType::FAIRY_CIRCLE:
	{
		auto& playerHealth = player.GetComponent<HealthComponent>();
		// If the player's health is already at max, no need to start the fairy circle trigger
		if (playerHealth.healthPercentage != playerHealth.maxHearts * 2)
		{
			for (const auto& fairy : Registry::Instance().GetEntitiesByGroup("fairy"))
			{
				// Get the fairy transform. It will be used to check against player position and 
				// to position the circling hearts
				auto& fairyTransform = fairy.GetComponent<TransformComponent>();
				const int checkX = fairyTransform.position.x / 1024;
				const int checkY = fairyTransform.position.y / 672;


				// Check to see if the fairy is in the same panel as the player
				if (checkX != game.GetPlayerPos().x || checkY != game.GetPlayerPos().y)
					continue;

				auto& fairyTrig = trigger.GetComponent<TriggerBoxComponent>();
				// If the trigger is already active, the hearts would have already been created
				if (fairyTrig.active)
					continue;

				// Set the fairy trigger to active
				fairyTrig.active = true;

				// There are Eight Hearts that Circle the fairy
				for (int i = 0; i < 8; i++)
				{
					// Create a new Heart entity to circle the fairy
					auto fairyHeart = Registry::Instance().CreateEntity();;
					fairyHeart.Group("fairy_hearts");
					fairyHeart.AddComponent<TransformComponent>(fairyTransform.position, fairyTransform.scale);
					fairyHeart.AddComponent<SpriteComponent>("hearts", 16, 16, 2, false, 0, 0);
					fairyHeart.AddComponent<GameComponent>();
				}
			}
		}

		break;
	}
	default:
		break;
	}
}

void TriggerSystem::Update(const float& dt)
{
	for (const auto& entity : GetSystemEntities())
	{
		// Get the player
		auto player = Registry::Instance().GetEntityByTag("player");
		auto& playerTransform = player.GetComponent<TransformComponent>();

		auto& transform = entity.GetComponent<TransformComponent>();
			
		auto sprite = SpriteComponent(); 

		if (entity.HasComponent<SpriteComponent>())
			sprite = entity.GetComponent<SpriteComponent>();

		auto& trig = entity.GetComponent<TriggerBoxComponent>();
				
		if (trig.active && entity.HasComponent<SecretComponent>())
		{
			auto& secret = entity.GetComponent<SecretComponent>();

			// If the secret has already been found, nothing to do
			if (secret.found)
				continue;

			if (trig.triggerType == TriggerBoxComponent::TriggerType::PUSH_ROCKS)
			{
				// Move the Block/Rock Up until it reaches its max position
				if (transform.position.y > secret.startPos.y - sprite.width * Game::gameScale && secret.moveUp)
				{
					transform.position.y--;
				}
				// Move the Block/Rock Down until it reaches its max position
				else if (transform.position.y < secret.startPos.y + sprite.width * Game::gameScale && secret.moveDown)
				{
					transform.position.y++;
				}
				else
				{
					secret.found = true;

					// Check to see if there is a Trap door or something else to remove
					const auto & removeTag = trig.entityRemoveTag;
					if (removeTag != "")
					{
						if (Registry::Instance().DoesTagExist(removeTag))
						{
							auto removedEntity = Registry::Instance().GetEntityByTag(removeTag);

							if (removedEntity.HasComponent<TriggerBoxComponent>())
							{
								const auto& triggerType = removedEntity.GetComponent<TriggerBoxComponent>().triggerType;
								if (triggerType == TriggerBoxComponent::TriggerType::TRAP_DOOR)
								{
									Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance().GetAssetManager(), "door_unlock", 0, -1);
								}
							}
							// Remove the entity
							removedEntity.Kill();
						}
					}
				}
			}
			else if (trig.triggerType == TriggerBoxComponent::TriggerType::RAFT)
			{
				auto raft = Registry::Instance().GetEntityByTag("raft");
				auto& raftTransform = raft.GetComponent<TransformComponent>();

				if (secret.moveUp && KeyboardControlSystem::dir == UP)
				{
					playerTransform.position.y -= 200 * game.GetDeltaTime();
					raftTransform.position.y -= 200 * game.GetDeltaTime();
					if (playerTransform.position.y <= trig.transportOffset.y)
					{
						trig.active = false;
						game.SetOnRaft(false);
						raft.Kill();
					}
				}
				else if (secret.moveDown && KeyboardControlSystem::dir == DOWN)
				{
					playerTransform.position.y += 200 * game.GetDeltaTime();
					raftTransform.position.y += 200 * game.GetDeltaTime();

					if (playerTransform.position.y >= trig.transportOffset.y)
					{
						trig.active = false;
						game.SetOnRaft(false);
						raft.Kill();
					}
				}
			}
			else if (trig.triggerType == TriggerBoxComponent::TriggerType::LADDER)
			{
				if (playerTransform.position.x < transform.position.x - 64 ||
					playerTransform.position.x > transform.position.x + 32)
				{
					auto ladder = Registry::Instance().GetEntityByTag("ladder");
					trig.active = false;
					ladder.Kill();
				}
			}
		}
		else if (trig.active && trig.triggerType == TriggerBoxComponent::TriggerType::FAIRY_CIRCLE)
		{
			auto& playerHealth = player.GetComponent<HealthComponent>();

			// Temp variable
			static int x = 0;

			// Check to see if the player has full health, if so, leave the loop
			if (playerHealth.healthPercentage != playerHealth.maxHearts * 2)
			{
				if (!trig.collectedTimer.isStarted())
					trig.collectedTimer.Start();

				

				// Increment the player health after a certain interval
				if (trig.collectedTimer.GetTicks() > 1000 * x)
				{
					playerHealth.healthPercentage += 1;
					x++;
				}

				// Play the refill sound
				if (Mix_Playing(6) == 0)
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance().GetAssetManager(), "refill_hearts", 0, 6);
			}
			else
			{
				// Reset the trigger
				trig.active = false;
				// Reset the trigger timer
				trig.collectedTimer.Stop();
				// Remove All the created hearts
				for (auto& heart : Registry::Instance().GetEntitiesByGroup("fairy_hearts"))
				{
					heart.Kill();
				}

				x = 0;
				// Leave the loop
				continue;
			}
				
			// Increment the angle for the circle motion calculation
			angle += 0.05f;

			for (const auto& fairy : Registry::Instance().GetEntitiesByGroup("fairy"))
			{
				// Get the fairy transform. It will be used to check against player position and 
				// to position the circling hearts
				auto& fairyTransform = fairy.GetComponent<TransformComponent>();
				const int checkX = fairyTransform.position.x / 1024;
				const int checkY = fairyTransform.position.y / 672;


				// Check to see if the fairy is in the same panel as the player
				if (checkX != game.GetPlayerPos().x || checkY != game.GetPlayerPos().y)
					continue;

				// Loop through the 8 Circling hearts and change their position around the fairy, 
				// The fairy's position is the center of the circle
				for (auto& heart : Registry::Instance().GetEntitiesByGroup("fairy_hearts"))
				{
					auto& heartsTransform = heart.GetComponent<TransformComponent>();

					// Move the heart in a circular motion
					heartsTransform.position = glm::vec2((fairyTransform.position.x) + (cos(angle + heartOffset) * 288), (fairyTransform.position.y) + (sin(angle + heartOffset) * 288));
					
					// This is for offsetting each heart
					heartOffset += 0.75f;

				}
				// Reset the heart offset back to zero so we can restart we we come back into the for loop
				heartOffset = 0.f;
			}
		}
	}
}
