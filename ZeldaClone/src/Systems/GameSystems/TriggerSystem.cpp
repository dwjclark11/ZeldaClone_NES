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
	case ItemComponent::SpecialItemType::ARROWS:
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
	case ItemComponent::SpecialItemType::ARROWS:
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

		// This needs to be moved to the save Screen!!
		//loader.SaveSecrets();
	
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
				//// Remove all the prior assets/entities from the current scene
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

		if (!trig.active)
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

			// After the new trigger is created, remove the current trigger!
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
	default:
		break;
	}
}

void TriggerSystem::Update()
{
	for (const auto& entity : GetSystemEntities())
	{
		if (entity.HasComponent<TriggerBoxComponent>())
		{
			// Get the player
			auto player = Registry::Instance().GetEntityByTag("player");
			auto& playerTransform = player.GetComponent<TransformComponent>();

			auto& transform = entity.GetComponent<TransformComponent>();
			
			auto sprite = SpriteComponent(); 
			auto secret = SecretComponent();

			if (entity.HasComponent<SpriteComponent>())
				sprite = entity.GetComponent<SpriteComponent>();

			if (entity.HasComponent<SecretComponent>())
				secret = entity.GetComponent<SecretComponent>();

			auto& trig = entity.GetComponent<TriggerBoxComponent>();
				
			if (trig.active)
			{
				if (trig.triggerType == TriggerBoxComponent::TriggerType::PUSH_ROCKS)
				{
					if (transform.position.y > secret.startPos.y - sprite.width * Game::gameScale && secret.moveUp)
					{
						transform.position.y--;
					}
					else if (transform.position.y < secret.startPos.y + sprite.width * Game::gameScale && secret.moveDown)
					{
						transform.position.y++;
					}
					else
					{
						secret.found = true;
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
		}
		else
			continue;
	}
}
