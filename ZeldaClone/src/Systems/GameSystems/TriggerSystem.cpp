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
#include "../../Systems/RenderTextSystem.h"
#include "../../Events/CollisionEvent.h"
#include "../../Events/TriggerEvent.h"
#include "../../Events/EventManager.h"
#include "../../Game/Game.h"
#include "../../Game/Player.h"

#include "../../Utilities/Timer.h"
#include "../../Utilities/GameData.h"
#include "../../Utilities/Camera.h"
#include "../SoundFXSystem.h"
#include "../../States/GameState.h"


bool TriggerSystem::CheckInventory(ItemComponent::SpecialItemType& item)
{
	// Check to see the item is in the inventory
	switch (item)
	{
	case ItemComponent::SpecialItemType::WOOD_SWORD:
		return gameData.HasItem(GameData::GameItems::WOOD_SWORD);
	case ItemComponent::SpecialItemType::STEEL_SWORD:
		return gameData.HasItem(GameData::GameItems::SILVER_SWORD);
	case ItemComponent::SpecialItemType::MAGIC_SWORD:
		return gameData.HasItem(GameData::GameItems::MAGIC_SWORD);
	case ItemComponent::SpecialItemType::RED_CANDLE:
		return gameData.HasItem(GameData::GameItems::CANDLE);
	case ItemComponent::SpecialItemType::WOOD_BOW:
		return gameData.HasItem(GameData::GameItems::BOW);
	case ItemComponent::SpecialItemType::ARROWS:
		return gameData.HasItem(GameData::GameItems::WOOD_ARROWS);
	case ItemComponent::SpecialItemType::MAGIC_ARROWS:
		return gameData.HasItem(GameData::GameItems::MAGIC_ARROWS);
	default:
		return false;
		break;
	}

	// If the item is not in the inventory return false
	return false;
}

void TriggerSystem::SetInventory(ItemComponent::SpecialItemType& item)
{
	const auto& player = game.GetPlayer();
	const auto& playerEnt = player->GetPlayer();
	auto& playerHealth = playerEnt.GetComponent<HealthComponent>();

	switch (item)
	{
	case ItemComponent::SpecialItemType::NOT_SPECIAL:
		break;

	case ItemComponent::SpecialItemType::WOOD_SWORD:
		gameData.AddItem(GameData::GameItems::WOOD_SWORD);
		break;

	case ItemComponent::SpecialItemType::STEEL_SWORD:
		gameData.AddItem(GameData::GameItems::SILVER_SWORD);
		break;

	case ItemComponent::SpecialItemType::MAGIC_SWORD:
		gameData.AddItem(GameData::GameItems::MAGIC_SWORD);
		break;

	case ItemComponent::SpecialItemType::FULL_HEART:
	{
		playerHealth.addHeart = true;
		break;
	}
	case ItemComponent::SpecialItemType::RAFT:
		gameData.AddItem(GameData::GameItems::RAFT);
		break;
	case ItemComponent::SpecialItemType::POWER_BRACLET:
		gameData.AddItem(GameData::GameItems::POWER_BRACLET);
		break;
	case ItemComponent::SpecialItemType::RED_CANDLE:
		gameData.AddItem(GameData::GameItems::CANDLE);
		break;
	case ItemComponent::SpecialItemType::WOOD_BOOMERANG:
		gameData.AddItem(GameData::GameItems::BOOMERANG);
		break;
	case ItemComponent::SpecialItemType::MAGIC_BOOMERANG:
		gameData.AddItem(GameData::GameItems::MAGIC_BOOMERANG);
		break;
	case ItemComponent::SpecialItemType::LADDER:
		gameData.AddItem(GameData::GameItems::LADDER);
		break;
	case ItemComponent::SpecialItemType::WOOD_BOW:
		gameData.AddItem(GameData::GameItems::BOW);
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
		gameData.SetSecretFound(secret.locationID, true);
		game.GetSoundPlayer().PlaySoundFX("secret", 0, SoundChannel::ANY);
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

void TriggerSystem::ChangeScene(Entity& player, Entity& trigger)
{
	const auto& player1 = game.GetPlayer();

	if (player1->GetStairsFinished() || game.GetCamera().CurtainClosed())
	{
		
		const auto& playerEnt = player1->GetPlayer();
		auto& playerTransform = playerEnt.GetComponent<TransformComponent>();
		auto& rigidbody = playerEnt.GetComponent<RigidBodyComponent>();
		auto& sprite = playerEnt.GetComponent<SpriteComponent>();
		auto& scene = trigger.GetComponent<SceneChangeComponent>();
		auto& camera = game.GetCamera();

		// Stop the player movement during the scene transition
		rigidbody.velocity = glm::vec2(0);
		// Get needed componenets from the triger
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		float x = trig.transportOffset.x;
		float y = trig.transportOffset.y;

		if (trig.collectedTimer.GetTicks() == 0)
		{
			trig.collectedTimer.Start();
		}

		// If the Scene has not faded, start the fade transition
		if (camera.GetFadeAlpha() > 0 && !camera.CurtainClosed())
		{
			if (!camera.FadeOutStarted())
				camera.StartFadeOut(true);
		}
		else
		{
			// Set player Position
			playerTransform.position.x = x;
			playerTransform.position.y = y;

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
				game.GetMusicPlayer().PlayMusic(scene.levelMusic, -1);
			else
				game.GetMusicPlayer().StopMusic();

			// If there is an entity file, NPCs, Store Items, etc. Load it
			if (scene.entityFileName != "no_file")
				loader.LoadEntitiesFromLuaTable(game.GetLuaState(), scene.entityFileName);

			// Adjust the camera location to the trigger offset
			camera.SetCameraPosition(trig.cameraOffset.x, trig.cameraOffset.y);
			//camera.SetCameraPosFromSceneChange(trig.cameraOffset.x, trig.cameraOffset.y);
			camera.SetPrevCamPosition(trig.cameraOffset.x, trig.cameraOffset.y);

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

			// Check to see if there is a collider file
			if (scene.colliderFile != "no_file")
				loader.LoadColliders(scene.colliderFile);

			// Remove the current trigger
			//trigger.Kill();

			// Finish the fade screen 
			if (!camera.CurtainClosed())
			{
				camera.StartFadeOut(false);
				camera.StartFadeIn(true);
			}
			
			// Set player sprite back to start
			sprite.srcRect.y = 0;
			
			if (camera.CurtainClosed())
				camera.StartCurtainOpen();
			else
				game.GetPlayer()->SetStairsFinished(false);
		}
	}
	else
	{
		game.GetPlayer()->SetPlayerOnStairs(true);
	}
}

void TriggerSystem::PushRocks(Entity& player, Entity& trigger)
{
	auto& playerRigidBody = player.GetComponent<RigidBodyComponent>();
	auto& rockTransform = trigger.GetComponent<TransformComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();

	if (!trig.active && !secret.found)
	{
		if (playerRigidBody.dir == RigidBodyComponent::Dir::UP)
		{
			trig.active = true;
			game.GetSoundPlayer().PlaySoundFX("secret", 0, SoundChannel::ANY);
			secret.moveUp = true;
		}
		else if (playerRigidBody.dir == RigidBodyComponent::Dir::DOWN)
		{
			trig.active = true;
			game.GetSoundPlayer().PlaySoundFX("secret", 0, SoundChannel::ANY);
			secret.moveDown = true;
		}
	}
}

void TriggerSystem::RideRaft(Entity& player, Entity& trigger)
{
	auto& trigs = trigger.GetComponent<TriggerBoxComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	const auto& pTran = player.GetComponent<TransformComponent>();

	if (gameData.HasItem(GameData::GameItems::RAFT))
	{
		if ((secret.moveDown && playerRigidbody.dir == RigidBodyComponent::Dir::DOWN) || 
			(secret.moveUp && playerRigidbody.dir == RigidBodyComponent::Dir::UP))
		{
			if (!trigs.active)
			{
				StopPlayerMovement(player, trigger);
				game.GetSoundPlayer().PlaySoundFX("secret", 0, SoundChannel::ANY);
				auto raft = Registry::Instance().CreateEntity();
				raft.Tag("raft");
				raft.AddComponent<TransformComponent>(pTran);
				raft.AddComponent<SpriteComponent>("items", 16, 16, 1, false, 0, 16);
				raft.AddComponent<GameComponent>();
				raft.GetComponent<TransformComponent>().position.x += 32;
				raft.GetComponent<TransformComponent>().position.y += 32;
			}

			trigs.active = true;
			game.GetPlayer()->SetOnRaft(true);
		}
	}
	else
	{
		StopPlayerMovement(player, trigger);
	}
}

void TriggerSystem::Transport(Entity& player, Entity& trigger)
{
	if (game.GetCamera().GetFadeAlpha() > 0)
	{
		game.GetCamera().StartFadeOut(true);
		return;
	}

	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	playerTransform.position = trig.transportOffset;
	game.GetCamera().SetCameraPosition(trig.cameraOffset.x, trig.cameraOffset.y);
	
	game.GetSoundPlayer().PlaySoundFX("stairs", 0, SoundChannel::ANY);

	// Finish the fade screen 
	game.GetCamera().StartFadeOut(false);
	game.GetCamera().StartFadeIn(true);
}

void TriggerSystem::UseLadder(Entity& player, Entity& trigger)
{
	if (!gameData.HasItem(GameData::GameItems::LADDER))
	{
		StopPlayerMovement(player, trigger);
		return;
	}

	auto& trigs = trigger.GetComponent<TriggerBoxComponent>();
	auto& trigTransform = trigger.GetComponent<TransformComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();

	if (!trigs.active)
	{
		auto xOffset = 64;
		auto yOffset = 64;
		switch (playerRigidbody.dir)
		{
		case RigidBodyComponent::Dir::UP:
			xOffset = 32;
			yOffset = -64;
			break;
		case RigidBodyComponent::Dir::RIGHT:
			xOffset = 64;
			yOffset = 32;
			break;
		case RigidBodyComponent::Dir::DOWN:
			xOffset = 32;
			yOffset = 64;
			break;
		case RigidBodyComponent::Dir::LEFT:
			xOffset = -16;
			yOffset = 32;
			break;
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

void TriggerSystem::BombSecret(Entity& trigger)
{
	if (trigger.HasComponent<SecretComponent>())
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
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
}

void TriggerSystem::UnlockDoor(Entity& trigger)
{
	// Unlock the door if keys are available
	if (gameData.GetTotalKeys() > 0)
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		// Play door open sound
		game.GetSoundPlayer().PlaySoundFX("door_unlock", 0, SoundChannel::ANY);
		// Decrement total number of keys
		gameData.UseKey();

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
}

void TriggerSystem::BuyShopItem(Entity& trigger)
{
	// Check to see if the trigger has an Item Components
	if (trigger.HasComponent<ItemComponent>() && !gameData.RupeeScrollActive())
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		auto& shopItem = trigger.GetComponent<ItemComponent>();
		// Check to see if the player has enough rupees
		if (gameData.GetTotalRupees() < shopItem.price)
		{
			// Maybe make a cancel sound?
			Logger::Log("Player Does not Have Enough Rupees!");
			return;
		}

		// Check to see if key item and if it is already in the inventory
		if (shopItem.special != ItemComponent::SpecialItemType::NOT_SPECIAL)
		{
			// Check to see if the item is in the inventory
			if (CheckInventory(shopItem.special))
			{
				Logger::Log("Item is Already in the inventory!");
				return;
			}
		}

		// Test --> Collect the item
		if (!trig.active)
		{
			if (!gameData.BuyItem(shopItem.price))
			{
				Logger::Log("Player Rupees is too low!");
				return;
			}

			game.GetSoundPlayer().PlaySoundFX("fanfare", 0, SoundChannel::ANY);
			game.GetPlayer()->SetPlayerItem(true);
			trig.active = true;
		}
	}
	else
	{
		Logger::Log("TRIGGER: __SHOP_ITEM: All Shop Items must have an ItemComponent!");
	}
}

void TriggerSystem::EnterFairyCircle(Entity& player, Entity& trigger)
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
			if (checkX != game.GetPlayer()->GetPlayerPos().x || checkY != game.GetPlayer()->GetPlayerPos().y)
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
}

void TriggerSystem::UpdateFairyCircle(Entity& player, Entity& trigger)
{
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
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
			game.GetSoundPlayer().PlaySoundFX("refill_hearts", 0, SoundChannel::LOW_HEALTH);
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
		return;
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
		if (checkX != game.GetPlayer()->GetPlayerPos().x || checkY != game.GetPlayer()->GetPlayerPos().y)
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

void TriggerSystem::UpdateRaft(Entity& player, Entity& trigger)
{
	auto raft = Registry::Instance().GetEntityByTag("raft");
	auto& raftTransform = raft.GetComponent<TransformComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();

	if (secret.moveUp && playerRigidbody.dir == RigidBodyComponent::Dir::UP)
	{
		playerTransform.position.y -= 200 * game.GetDeltaTime();
		raftTransform.position.y -= 200 * game.GetDeltaTime();
		if (playerTransform.position.y <= trig.transportOffset.y)
		{
			trig.active = false;
			game.GetPlayer()->SetOnRaft(false);
			raft.Kill();
		}
	}
	else if (secret.moveDown && playerRigidbody.dir == RigidBodyComponent::Dir::DOWN)
	{
		playerTransform.position.y += 200 * game.GetDeltaTime();
		raftTransform.position.y += 200 * game.GetDeltaTime();

		if (playerTransform.position.y >= trig.transportOffset.y)
		{
			trig.active = false;
			game.GetPlayer()->SetOnRaft(false);
			raft.Kill();
		}
	}
}

void TriggerSystem::UpdateMovingRocks(Entity& player, Entity& trigger)
{
	auto& transform = trigger.GetComponent<TransformComponent>();
	auto& secret = trigger.GetComponent<SecretComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	auto&sprite = trigger.GetComponent<SpriteComponent>();

	// Move the Block/Rock Up until it reaches its max position
	if (transform.position.y > secret.startPos.y - sprite.width * game.GetGameScale() && secret.moveUp)
	{
		transform.position.y--;
	}
	// Move the Block/Rock Down until it reaches its max position
	else if (transform.position.y < secret.startPos.y + sprite.width * game.GetGameScale() && secret.moveDown)
	{
		transform.position.y++;
	}
	else
	{
		secret.found = true;

		// Check to see if there is a Trap door or something else to remove
		const auto& removeTag = trig.entityRemoveTag;
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
						game.GetSoundPlayer().PlaySoundFX("door_unlock", 0, SoundChannel::ANY);
					}
				}
				// Remove the entity
				removedEntity.Kill();
			}
		}
	}
}

void TriggerSystem::UpdateLadder(Entity& player, Entity& trigger)
{
	auto& transform = trigger.GetComponent<TransformComponent>();
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();
	auto& sprite = trigger.GetComponent<SpriteComponent>();
	auto& playerTransform = player.GetComponent<TransformComponent>();

	if (playerTransform.position.x < transform.position.x - 64 ||
		playerTransform.position.x > transform.position.x + 32)
	{
		auto ladder = Registry::Instance().GetEntityByTag("ladder");
		trig.active = false;
		ladder.Kill();
	}
}

TriggerSystem::TriggerSystem()
	: game(Game::Instance())
	, gameData(GameData::GetInstance())
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
		return;
	}
	else if (!b.HasComponent<SecretComponent>() && b.HasComponent<TriggerBoxComponent>() && a.HasTag("player"))
	{
		OnEnterTrigger(a, b);
		return;
	}

	// Check for secret Component 
	if (b.HasComponent<SecretComponent>())
	{
		auto& trig = b.GetComponent<TriggerBoxComponent>();
		if (trig.triggerType == TriggerBoxComponent::TriggerType::BOMB_SECRET && a.BelongsToGroup("explode"))
		{
			OnEnterTrigger(a, b);
			return;
		}
			
		else if (trig.triggerType == TriggerBoxComponent::TriggerType::BURN_BUSHES && a.BelongsToGroup("fire"))
		{
			OnEnterTrigger(a, b);
			return;
		}
			
		else if (trig.triggerType == TriggerBoxComponent::TriggerType::PUSH_ROCKS && a.HasTag("player"))
		{
			OnEnterTrigger(a, b);
			return;
		}
		else if (trig.triggerType == TriggerBoxComponent::TriggerType::RAFT && a.HasTag("player"))
		{
			OnEnterTrigger(a, b);
			return;
		}
			
	}
	else if (a.HasComponent<SecretComponent>())
	{
		auto& trig = a.GetComponent<TriggerBoxComponent>();
		if (trig.triggerType == TriggerBoxComponent::TriggerType::BOMB_SECRET && b.BelongsToGroup("explode"))
		{
			OnEnterTrigger(b, a);
			return;
		}
		else if (trig.triggerType == TriggerBoxComponent::TriggerType::BURN_BUSHES && b.BelongsToGroup("fire"))
		{
			OnEnterTrigger(b, a);
			return;
		}
		else if (trig.triggerType == TriggerBoxComponent::TriggerType::PUSH_ROCKS && b.HasTag("player"))
		{
			OnEnterTrigger(b, a);
			return;
		}

		else if (trig.triggerType == TriggerBoxComponent::TriggerType::RAFT && b.HasTag("player"))
		{
			OnEnterTrigger(b, a);
			return;
		}
	}
}

void TriggerSystem::OnEnterTrigger(Entity& player, Entity& trigger)
{
	// Get needed componenets from the triger
	auto& trig = trigger.GetComponent<TriggerBoxComponent>();

	switch (trig.triggerType)
	{
	case TriggerBoxComponent::TriggerType::NO_TRIGGER:
		Logger::Err("TRIGGER_SYSTEM: __LINE: 174 - There is not trigger set for this entity");
		break;
	case TriggerBoxComponent::TriggerType::SCENE_CHANGE: 
		ChangeScene(player, trigger);
		break;
	case TriggerBoxComponent::TriggerType::TRANSPORT:
		Transport(player, trigger);
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
		PushRocks(player, trigger);
		break;
	case TriggerBoxComponent::TriggerType::COLLECT_ITEM:
	{
		if (!trig.active)
		{
			auto& item = trigger.GetComponent<ItemComponent>();
			std::string soundID = "fanfare";
			if (item.special == ItemComponent::SpecialItemType::TRIFORCE_PIECE)
			{
				game.GetPlayer()->SetObtainedTriforce(true);
				soundID = "triforce_fanfare";
				game.GetMusicPlayer().PlayMusic(soundID, 0);
			}
			else
			{
				game.GetSoundPlayer().PlaySoundFX(soundID, 0, SoundChannel::ANY);
			}

			game.GetPlayer()->SetPlayerItem(true);
			trig.active = true;
		}
		break;
	}
	case TriggerBoxComponent::TriggerType::BOMB_SECRET:
		BombSecret(trigger);
		break;
	case TriggerBoxComponent::TriggerType::LOCKED_DOOR:
		UnlockDoor(trigger);
		break;
	case TriggerBoxComponent::TriggerType::SHOP_ITEM:
		BuyShopItem(trigger);
		break;
	case TriggerBoxComponent::TriggerType::RAFT:
		RideRaft(player, trigger);
		break;
	case TriggerBoxComponent::TriggerType::LADDER:
		UseLadder(player, trigger);
		break;
	case TriggerBoxComponent::TriggerType::TRAP_DOOR:
		Logger::Err("Touching a trap door!!");
		break;
	case TriggerBoxComponent::TriggerType::FAIRY_CIRCLE:
		EnterFairyCircle(player, trigger);
		break;
	default:
		break;
	}
}

void TriggerSystem::Update(const float& dt)
{
	for (auto& entity : GetSystemEntities())
	{
		auto& trig = entity.GetComponent<TriggerBoxComponent>();

		if (!trig.active)
			continue;

		// Get the player
		const auto& player = game.GetPlayer();
		auto& playerEnt = player->GetPlayer();
	
		if (entity.HasComponent<SecretComponent>())
		{
			auto& secret = entity.GetComponent<SecretComponent>();
			// If the secret has already been found, nothing to do
			if (secret.found)
				continue;

			switch (trig.triggerType)
			{
			case TriggerBoxComponent::TriggerType::PUSH_ROCKS:
				UpdateMovingRocks(playerEnt, entity);
				break;
			case TriggerBoxComponent::TriggerType::RAFT:
				UpdateRaft(playerEnt, entity);
				break;
			case TriggerBoxComponent::TriggerType::LADDER:
				UpdateLadder(playerEnt, entity);
				break;
			}
		}
		else if (trig.triggerType == TriggerBoxComponent::TriggerType::FAIRY_CIRCLE)
		{
			UpdateFairyCircle(playerEnt, entity);
		}
	}
}
