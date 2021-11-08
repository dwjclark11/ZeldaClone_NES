#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/GameSystems/RenderTileSystem.h"
#include "../Events/CollisionEvent.h"
#include "../Events/TriggerEvent.h"
#include "../Events/EventManager.h"
#include "../../Game/Game.h"
#include "../../Game/LevelLoader.h"
#include <cmath>
#include "../../Utilities/Timer.h"

#include "../../States/GameState.h"
#include <windows.h>

class TriggerSystem : public System
{
private:
	sol::state lua;
	LevelLoader loader;
	class Game& game;
public:

	TriggerSystem()
		: game(*Game::Instance())
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<CollisionEvent>(this, &TriggerSystem::OnTrigger);
	}

	void OnTrigger(CollisionEvent& event)
	{
		Entity a = event.a;
		Entity b = event.b;

		// Check to see if the player has activated a trigger
		if (a.BelongsToGroup("trigger") && b.HasTag("player"))
		{
			OnEnterTrigger(b, a);
		}

		if (b.BelongsToGroup("trigger") && a.HasTag("player"))
		{
			OnEnterTrigger(a, b);
		}
	}

	void OnEnterTrigger(Entity player, Entity trigger)
	{
		// Get needed componenets from the triger
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		auto& trigTransform = trigger.GetComponent<TransformComponent>();
		// Get the needed componenets from the player
		auto& transform = player.GetComponent<TransformComponent>();
		auto& rigidbody = player.GetComponent<RigidBodyComponent>();
		auto& sprite = player.GetComponent<SpriteComponent>();

		// Set variables for the files to load assets/tilemaps/enemies/ etc.
		std::string assetFile = trig.assetFile;
		std::string mapFile = "Assets/Tilemaps/Maps/" + trig.tileMapName + ".map";
		std::string tileFile = "Assets/Tilemaps/Tiles/" + trig.tileImageName + ".png";
		std::string entityFile = trig.entityFileName;

		switch (trig.triggerType)
		{
		case NO_TRIGGER:
			break;

		case SECRET_AREA:
			
			// Remove all the prior assets/entities from the current scene
			game.GetSystem<RenderSystem>().OnExit();
			game.GetSystem<RenderCollisionSystem>().OnExit();
			game.GetSystem<RenderTileSystem>().OnExit();

			// Play stairs sound
			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "stairs", 0, 1);

			// Adjust the player position to the trigger transport position
			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;

			// Adjust the camera location to the trigger offset
			game.GetCamera().x = trig.cameraOffset.x;
			game.GetCamera().y = trig.cameraOffset.y;

			// Stop the player movement during the scene transition
			rigidbody.velocity = glm::vec2(0);

			// Check to see if the trigger has "no_file" assiged if it has a file load the assets for the scene
			if (assetFile != "no_file")
			{
				Logger::Log("Asset File");
				loader.LoadAssetsFromLuaTable(game.GetLuaState(), assetFile);
			}
			if (trig.colliderFile != "no_file")
			{
				loader.LoadColliders(game.GetAssetManager(), game.GetRenderer(), trig.colliderFile);
				Logger::Log("Load Collider File");
			}

			if (trig.enemyFile != "no_file")
			{
				loader.LoadEnemiesFromLuaTable(game.GetLuaState(), trig.enemyFile, game.GetAssetManager());
			}

			if (trig.tileMapName != "no_file" && trig.tileImageName != "no_file")
			{
				loader.LoadTilemap(game.GetAssetManager(), game.GetRenderer(), mapFile, tileFile);
			}
			else
			{
				loader.LoadMap(game.GetAssetManager(), trig.tileImageName, trig.imageWidth, trig.imageHeight);
			}

			// Start the new scene's music || stop the music
			if (trig.levelMusic != "stop")
				game.GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), trig.levelMusic, -1);
			else
				game.GetSystem<MusicPlayerSystem>().StopMusic();

			if (entityFile != "no_file")
			{
				loader.LoadEntitiesFromLuaTable(game.GetLuaState(), entityFile);
				Logger::Log("Load Entity File");
			}

			break;

		case ENTER_DUNGEON:

			break;

		case BURN_BUSHES:

			break;

		case PUSH_ROCKS:

			break;

		case COLLECT_ITEM:

			// Play the fanfare sound
			if (!trig.active)
			{
				Logger::Log("Trigger B");
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "fanfare", 0, 1);
				trig.active = true;
				game.GetPlayerItem() = true;
			}

			break;

		case RETURN_WORLD:
			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "stairs", 0, 1);
			transform.position = trig.transportOffset;

			//game.GetStateMachine()->PopState();
			//game.GetStateMachine()->PushState(new GameState(trig.cameraOffset));
			break;
		default:

			break;
		}
	}

};