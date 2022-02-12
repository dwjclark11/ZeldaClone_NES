#include "TriggerSystem.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SecretComponent.h"
#include "../../Systems/GameSystems/RenderTileSystem.h"
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
			secretTrigger.transportOffset, secretTrigger.cameraOffset,
			secretTrigger.levelMusic, secretTrigger.assetFile,
			secretTrigger.enemyFile, secretTrigger.colliderFile,
			secretTrigger.tileMapName, secretTrigger.tileImageName,
			secretTrigger.entityFileName, secretTrigger.imageWidth, secretTrigger.imageHeight, secretTrigger.triggerFile);

		secretArea.AddComponent<TransformComponent>(secretTransform.position, secretTransform.scale, secretTransform.rotation);
		secretArea.AddComponent<SpriteComponent>(secret.newSpriteAssetID, secret.spriteWidth, secret.spriteHeight, 1, false, secret.spriteSrcX, secret.spriteSrcY);
		secretArea.AddComponent<GameComponent>();

		Logger::Log("Blasted open " + secret.locationID + " Secret!");
		secret.found = true;
		game.SetSecretFound(secret.locationID, true);
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "secret", 0, -1);

		loader.SaveSecrets();
	
	}
	else if (secret.found && startup)
	{
		TriggerBoxComponent::TriggerType trigType = loader.ConvertStringToTriggerType(secret.newTrigger);

		auto secretArea = Registry::Instance().CreateEntity();
		secretArea.Group("trigger");
		secretArea.AddComponent<BoxColliderComponent>(secretCollider.width, secretCollider.height, secretCollider.offset);
		secretArea.AddComponent<TriggerBoxComponent>(trigType,
			secretTrigger.transportOffset, secretTrigger.cameraOffset,
			secretTrigger.levelMusic, secretTrigger.assetFile,
			secretTrigger.enemyFile, secretTrigger.colliderFile,
			secretTrigger.tileMapName, secretTrigger.tileImageName,
			secretTrigger.entityFileName, secretTrigger.imageWidth, secretTrigger.imageHeight, secretTrigger.triggerFile);

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
	if (a.HasComponent<TriggerBoxComponent>() && b.HasTag("player"))
	{
		OnEnterTrigger(b, a);
	}

	if (b.HasComponent<TriggerBoxComponent>() && a.HasTag("player"))
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
	}
}

void TriggerSystem::OnEnterTrigger(Entity& player, Entity& trigger)
{
	// Get needed componenets from the triger
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	float x = trig.transportOffset.x;
	float y = trig.transportOffset.y;
	// Get the needed componenets from the player

	// Set variables for the files to load assets/tilemaps/enemies/ etc.
	std::string& assetFile = trig.assetFile;
	std::string mapFile = "Assets/Tilemaps/Maps/" + trig.tileMapName + ".map";
	std::string tileFile = "Assets/Tilemaps/Tiles/" + trig.tileImageName + ".png";
	std::string& entityFile = trig.entityFileName;
	std::string& triggerFile = trig.triggerFile;
	std::string& colliderFile = trig.colliderFile;
	std::string& enemyFile = trig.enemyFile;
	std::string& levelMusic = trig.levelMusic;
	std::string& tileMapName = trig.tileMapName;
	std::string& tileImageName = trig.tileImageName;

	const auto& width = trig.imageWidth;
	const auto& height = trig.imageHeight;
	const auto& camPosX = trig.cameraOffset.x;
	const auto& camPosY = trig.cameraOffset.y;

	auto& playerTransform = player.GetComponent<TransformComponent>();
	Timer loadTimer;
	switch (trig.triggerType)
	{
	case TriggerBoxComponent::TriggerType::NO_TRIGGER:
		Logger::Err("TRIGGER_SYSTEM: __LINE: 174 - There is not trigger set for this entity");
		break;

	case TriggerBoxComponent::TriggerType::SECRET_AREA: // Change this to scene transition TriggerType::CHANGE_SCENE?
	{
		if (game.GetStairsFinished())
		{
			auto _player = Registry::Instance().GetEntityByTag("player");

			auto& rigidbody = _player.GetComponent<RigidBodyComponent>();
			auto& sprite = _player.GetComponent<SpriteComponent>();

			// Stop the player movement during the scene transition
			rigidbody.velocity = glm::vec2(0);

			if (trig.collectedTimer.GetTicks() == 0)
			{
				trig.collectedTimer.Start();
			}

			if (game.GetFadeAlpha() > 0)
			{
				game.StartFadeOut() = true;
			}
			else
			{
				//// Remove all the prior assets/entities from the current scene
				Registry::Instance().GetSystem<RenderSystem>().OnExit();
				Registry::Instance().GetSystem<RenderTileSystem>().OnExit();
				Registry::Instance().GetSystem<RenderCollisionSystem>().OnExit();
				
		
				// Check to see if the trigger has "no_file" assiged if it has a file load the assets for the scene
				if (assetFile != "no_file")
					loader.LoadAssetsFromLuaTable(game.GetLuaState(), assetFile);

				// load the tilemap only if there is an image and a corresponding map
				if (tileMapName != "no_file" && tileImageName != "no_file")
				{

					loader.LoadTilemap(mapFile, tileImageName);
				}

				// If there is only an image name than it is a full map image, not tiles --> Just load the map
				if (tileImageName != "no_file" && tileMapName == "no_file")
					loader.LoadMap(tileImageName, width, height);

				// Start the new scene's music || stop the music
				if (levelMusic != "stop")
					Registry::Instance().GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), levelMusic, -1);
				else
					Registry::Instance().GetSystem<MusicPlayerSystem>().StopMusic();

				// If there is an entity file, NPCs, Store Items, etc. Load it
				if (entityFile != "no_file")
					loader.LoadEntitiesFromLuaTable(game.GetLuaState(), entityFile);

				// Adjust the camera location to the trigger offset
				game.GetCamera().x = camPosX;
				game.GetCamera().y = camPosY;

				// Check to see if there is a collider file
				if (colliderFile != "no_file")
					loader.LoadColliders(colliderFile);
				// Check to see if there is a trigger file
				if (trig.triggerFile != "no_file")
				{
					Logger::Log("Trig " + trig.triggerFile);
					loader.LoadTriggers(game.GetLuaState(), trig.triggerFile);
				}

				// Check to see if there is an enemy file
				if (enemyFile != "no_file")
					loader.LoadEnemiesFromLuaTable(game.GetLuaState(), enemyFile);

				loader.ReadInSecrets(game.GetLuaState());
				
				// Set player Position
				_player.GetComponent<TransformComponent>().position.x = x;
				_player.GetComponent<TransformComponent>().position.y = y;

				// Finish the fade screen 
				game.StartFadeOut() = false;
				game.StartFadeIn() = true;
				// Remove the current trigger
				trigger.Kill();
				Logger::Log("Image: " + tileImageName);
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
			game.StartFadeOut() = true;
		}
		else
		{
			playerTransform.position = trig.transportOffset;
			game.GetCamera().x = trig.cameraOffset.x;
			game.GetCamera().y = trig.cameraOffset.y;
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "stairs", 0, -1);
			// Finish the fade screen 
			game.StartFadeOut() = false;
			game.StartFadeIn() = true;
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
			game.GetPlayerItem() = true;
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
				game.GetPlayerItem() = true;
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
	default:
		break;
	}
}

void TriggerSystem::Update()
{
	for (const auto& entity : GetSystemEntities())
	{
		if (entity.HasComponent<TriggerBoxComponent>() && entity.HasComponent<SecretComponent>() && entity.HasComponent<SpriteComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& secret = entity.GetComponent<SecretComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& trig = entity.GetComponent<TriggerBoxComponent>();
				
			if (trig.active && !secret.found)
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
		}
		else
			continue;
	}
}
