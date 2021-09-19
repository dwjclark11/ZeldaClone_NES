#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../Components/HealthComponent.h"
#include "../Systems/GameSystems/RenderTileSystem.h"
#include "../Events/CollisionEvent.h"
#include "../Events/TriggerEvent.h"
#include "../Events/EventManager.h"
#include "../../Game/Game.h"
#include "../../Game/LevelLoader.h"
#include <cmath>
#include "../../Utilities/Timer.h"

#include "../../States/GameState.h"

class TriggerSystem : public System
{
private: 
sol::state lua;
LevelLoader loader;

public: 

	TriggerSystem()
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
			OnPlayerTriggerA(a, b);
		}
		if (b.BelongsToGroup("trigger") && a.HasTag("player"))
		{
			OnPlayerTriggerB(a, b);	
		}
	}

	void OnPlayerTriggerA( Entity trigger, Entity player)
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		auto& transform = player.GetComponent<TransformComponent>();
		auto& rigidbody = player.GetComponent<RigidBodyComponent>();
		Logger::Log("trig num: " + std::to_string(trig.triggerType));

		std::string assetFile = trig.assetFile;
		std::string mapFile = "Assets/Tilemaps/Maps/" + trig.tileMapName + ".map";
		std::string tileFile = "Assets/Tilemaps/Tiles/" + trig.tileImageName + ".png";

		switch (trig.triggerType)
		{
		case NO_TRIGGER:
			Logger::Log("No Trig?");
			break;

		case SECRET_AREA:
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);

			Game::Instance()->GetSystem<RenderCollisionSystem>().OnExit();
			Game::Instance()->GetSystem<RenderSystem>().OnExit();
			Game::Instance()->GetSystem<RenderTileSystem>().OnExit();
			
			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;
			Game::Instance()->GetCamera().x = trig.cameraOffset.x;
			Game::Instance()->GetCamera().y = trig.cameraOffset.y;
			rigidbody.velocity = glm::vec2(0);

			
			if (assetFile != "no_file")
			{
				loader.LoadAssetsFromLuaTable(Game::Instance()->GetLuaState(), assetFile);
				Logger::Log("Load Asset File");
			}
			if (trig.colliderFile != "no_file")
			{
				loader.LoadColliders(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), trig.colliderFile);
				Logger::Log("Load Collider File");
			}
			if (trig.enemyFile != "no_file")
			{
				loader.LoadEnemiesFromLuaTable(Game::Instance()->GetLuaState(), trig.enemyFile, Game::Instance()->GetAssetManager());
				Logger::Log("Load Enemy File");
			}

			if (trig.tileMapName != "no_file" && trig.tileImageName != "no_file")loader.LoadTilemap(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), mapFile, tileFile);
			else loader.LoadMap(Game::Instance()->GetAssetManager(), trig.tileImageName, trig.imageWidth, trig.imageHeight);
			Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), trig.levelMusic, -1);
			break;

		case ENTER_DUNGEON:
			/* Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new DungeonState(trig.level)); */
			
			// Test --> If this works --> Make it into a function in Game
			Game::Instance()->GetSystem<RenderCollisionSystem>().OnExit();
			Game::Instance()->GetSystem<RenderSystem>().OnExit();
			Game::Instance()->GetSystem<RenderTileSystem>().OnExit();
			
			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;
			//loader.LoadAssetsFromLuaTable(lua, trig.assetFile);
			//loader.LoadEnemiesFromLuaTable(lua, trig.enemyFile, Game::Instance()->GetAssetManager());
			//loader.LoadColliders(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), trig.colliderFile);
			
			break;

		case BURN_BUSHES:

			break;

		case PUSH_ROCKS:

			break;

		case CAMERA_RIGHT:

			break;

		case RETURN_WORLD:
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position = trig.transportOffset;
			/*Game::Instance()->GetLevelWidth() = 16;
			Game::Instance()->GetLevelHeight() = 8;
			Game::Instance()->GetStateMachine()->PopState();
			Game::Instance()->GetStateMachine()->PushState(new GameState(trig.cameraOffset));*/
			break;
		default:
			
			break;
		}
	}
	void OnPlayerTriggerB(Entity player, Entity trigger)
	{
		auto& trig = trigger.GetComponent<TriggerBoxComponent>();
		auto& transform = player.GetComponent<TransformComponent>();
		auto& rigidbody = player.GetComponent<RigidBodyComponent>();

		std::string assetFile = trig.assetFile;
		std::string mapFile = "Assets/Tilemaps/Maps/" + trig.tileMapName + ".map";
		std::string tileFile = "Assets/Tilemaps/Tiles/" + trig.tileImageName + ".png";

		Logger::Log("trig num: " + std::to_string(trig.triggerType));
		switch (trig.triggerType)
		{
		case NO_TRIGGER:
			break;

		case SECRET_AREA:
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			Game::Instance()->GetSystem<RenderCollisionSystem>().OnExit();
			Game::Instance()->GetSystem<RenderSystem>().OnExit();
			Game::Instance()->GetSystem<RenderTileSystem>().OnExit();

			transform.position.x = trig.transportOffset.x;
			transform.position.y = trig.transportOffset.y;
			Game::Instance()->GetCamera().x = trig.cameraOffset.x;
			Game::Instance()->GetCamera().y = trig.cameraOffset.y;
			rigidbody.velocity = glm::vec2(0);

			

			if (assetFile != "no_file")
			{
				Logger::Log("Load Asset File: " + assetFile);
				loader.LoadAssetsFromLuaTable(Game::Instance()->GetLuaState(), assetFile);
			}
			if (trig.colliderFile != "no_file")
			{
				loader.LoadColliders(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), trig.colliderFile);
				Logger::Log("Load Collider File");
			}
			if (trig.enemyFile != "no_file")
			{
				loader.LoadEnemiesFromLuaTable(Game::Instance()->GetLuaState(), trig.enemyFile, Game::Instance()->GetAssetManager());
				Logger::Log("Load Enemy File");
			}

			if (trig.tileMapName != "no_file" && trig.tileImageName != "no_file")loader.LoadTilemap(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(), mapFile, tileFile);
			else
			{
				loader.LoadMap(Game::Instance()->GetAssetManager(), trig.tileImageName, trig.imageWidth, trig.imageHeight);
				Logger::Log("Load Image File: " + trig.tileImageName);

			}

			Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), trig.levelMusic, -1);
			break;

		case ENTER_DUNGEON:

			break;

		case BURN_BUSHES:

			break;

		case PUSH_ROCKS:

			break;

		case CAMERA_RIGHT:

			break;

		case RETURN_WORLD:
			Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "stairs", 0, 1);
			transform.position = trig.transportOffset;

			//Game::Instance()->GetStateMachine()->PopState();
			//Game::Instance()->GetStateMachine()->PushState(new GameState(trig.cameraOffset));
			break;
		default:

			break;
		}
	}
};
