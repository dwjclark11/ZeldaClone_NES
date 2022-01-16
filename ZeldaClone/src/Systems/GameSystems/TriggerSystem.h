#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Systems/GameSystems/RenderTileSystem.h"
#include "../../Events/CollisionEvent.h"
#include "../../Events/TriggerEvent.h"
#include "../../Events/EventManager.h"
#include "../../Game/Game.h"
#include "../../Game/LevelLoader.h"
#include <cmath>
#include "../../Utilities/Timer.h"
#include "../SoundFXSystem.h"
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
		Entity& a = event.a;
		Entity& b = event.b;

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

	void OnEnterTrigger(Entity& player, Entity& trigger)
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
		std::string& colliderFile = trig.colliderFile;
		std::string& enemyFile = trig.enemyFile;
		std::string& levelMusic = trig.levelMusic;
		std::string& tileMapName = trig.tileMapName;
		std::string& tileImageName = trig.tileImageName;
		
		const auto& width = trig.imageWidth;
		const auto& height = trig.imageHeight;
		const auto& camPosX = trig.cameraOffset.x;
		const auto& camPosY = trig.cameraOffset.y;


		Timer loadTimer;
		switch (trig.triggerType)
		{
		case NO_TRIGGER:
			break;

		case SECRET_AREA:
		{
			if (game.GetStairsFinished())
			{
				auto _player = Registry::Instance()->GetEntityByTag("player");

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
					game.GetSystem<RenderSystem>().OnExit();
					game.GetSystem<RenderTileSystem>().OnExit();
					game.GetSystem<RenderCollisionSystem>().OnExit();

					// Check to see if the trigger has "no_file" assiged if it has a file load the assets for the scene
					if (assetFile != "no_file")
					{
						loader.LoadAssetsFromLuaTable(game.GetLuaState(), assetFile);
					}

					if (tileMapName != "no_file" && tileImageName != "no_file")
					{
						Logger::Log("Image: " + tileImageName);
						loader.LoadTilemap(mapFile, tileImageName);
					}

					if (tileImageName != "no_file" && tileMapName == "no_file")
					{
						loader.LoadMap(tileImageName, width, height);
					}

					// Start the new scene's music || stop the music
					if (levelMusic != "stop")
						game.GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), levelMusic, -1);
					else
						game.GetSystem<MusicPlayerSystem>().StopMusic();

					if (entityFile != "no_file")
					{
						loader.LoadEntitiesFromLuaTable(game.GetLuaState(), entityFile);
					}

					// Adjust the camera location to the trigger offset
					game.GetCamera().x = camPosX;
					game.GetCamera().y = camPosY;

					if (colliderFile != "no_file")
						loader.LoadColliders(colliderFile);

					if (enemyFile != "no_file")
					{
						loader.LoadEnemiesFromLuaTable(game.GetLuaState(), enemyFile);
					}

					// Set player Position
					_player.GetComponent<TransformComponent>().position.x = x;
					_player.GetComponent<TransformComponent>().position.y = y;
					game.StartFadeOut() = false;
					game.StartFadeIn() = true;

					trigger.Kill();
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
		case ENTER_DUNGEON:

			break;

		case BURN_BUSHES:

			break;

		case PUSH_ROCKS:

			break;

		case COLLECT_ITEM:
		{
			if (!trig.active)
			{
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "fanfare", 0, 1);
				game.GetPlayerItem() = true;
				trig.active = true;
			}
			break;
		}
		case SHOP_ITEM:
			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "stairs", 0, 1);
			//transform.position = trig.transportOffset;
			break;
		default:
			break;
		}
	}
};