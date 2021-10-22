#pragma once
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../AssetManager/AssetManager.h"
#include "../../ECS/ECS.h"
#include <SDL.h>
#include <algorithm>
#include <vector>
#include "../../Game/Game.h"
#include "../Systems/PauseSystems/ItemSelectKeyboardSystem.h"
#include "../../Components/HUDComponent.h"

const int OFFSET = 720;

struct RenderHUDSystem : public System
{
	bool pause;
	bool game;
	RenderHUDSystem()
	{
		// All Systems need a required component to help separate what entities need to 
		// use that system
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();
		RequiredComponent<HUDComponenet>();
		pause = false;
		game = false;
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager)
	{
		// Loop all entities that the system is interested in
		for (auto entity : GetSystemEntities())
		{
			if (!Game::Instance()->GetGameItems().woodSword && entity.HasTag("hudSword"))
			{
				continue;
			}

			if (entity.BelongsToGroup("hud"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();

				// Set the src Rect of our original sprite texture
				SDL_Rect srcRect = sprite.srcRect;

				if (Game::Instance()->GetStateMachine()->GetCurrentState() == "PAUSE" && !pause)
				{
					game = false;
					transform.position.y += OFFSET;
				}
				else if (Game::Instance()->GetStateMachine()->GetCurrentState() == "GAMESTATE" || Game::Instance()->GetStateMachine()->GetCurrentState() == "DUNGEON" && !game)
				{
					// If we were previously in the pause state, subract the offset from all HUD entities
					if (pause)
					{
						transform.position.y -= OFFSET;
					}
				}

				if (entity.HasTag("hudItem"))
				{
					switch (ItemSelectKeyboardControlSystem::itemType)
					{
					case BOOMERANG:
						sprite.srcRect.x = 0;
						sprite.srcRect.y = 0;
						sprite.srcRect.x += sprite.width * 0;
						sprite.srcRect.y += sprite.height * 7;

						break;
					case BOMB:
						sprite.srcRect.x = 0;
						sprite.srcRect.y = 0;
						sprite.srcRect.x += sprite.width * 4;
						sprite.srcRect.y += sprite.height * 7;
						break;

					case WOOD_BOW:
						sprite.srcRect.x = 0;
						sprite.srcRect.y = 0;
						sprite.srcRect.x += sprite.width * 4;
						break;

					case CANDLE:
						sprite.srcRect.x = 0;
						sprite.srcRect.y = 0;
						sprite.srcRect.y += sprite.height * 3;
						break;

					case FOOD:
						sprite.srcRect.x = 0;
						sprite.srcRect.y = 0;
						sprite.srcRect.x += sprite.width * 7;
						sprite.srcRect.y += sprite.height * 1;
						break;

					case POTION_RED:
						sprite.srcRect.x = 0;
						sprite.srcRect.y = 0;
						sprite.srcRect.x += sprite.width * 2;
						sprite.srcRect.y += sprite.height * 2;
						break;

					case MAGIC_ROD:
						sprite.srcRect.x = 0;
						sprite.srcRect.y = 0;
						sprite.srcRect.x += sprite.width * 4;
						sprite.srcRect.y += sprite.height * 5;
						break;
					
					}
				}

				// Change the position of the locator to match the player position index
				// See MovementSystem --> Update()
				if (entity.HasTag("locator"))
				{
					transform.position.x = 10 + (Game::Instance()->GetPlayerPos().x * 16);
					transform.position.y = 40 + (Game::Instance()->GetPlayerPos().y * 16);
				}

				// Set the destination rect with the x, y position to be rendered
				SDL_Rect dstRect = {
					(transform.position.x),
					(transform.position.y),
					(sprite.width * transform.scale.x),
					(sprite.height * transform.scale.y)
				};

				SDL_RenderCopyEx(
					renderer,
					assetManager->GetTexture(sprite.assetID),
					&srcRect,
					&dstRect,
					transform.rotation,
					NULL,					// The rotation is done on the center of the sprite, width / 2, height / 2
					sprite.flip				// This is if we want to flup a sprite
				);
			}
		}
		// Reset the HUD states
		if (Game::Instance()->GetStateMachine()->GetCurrentState() == "PAUSE")
		{
			pause = true;
			game = false;
		}

		if (Game::Instance()->GetStateMachine()->GetCurrentState() == "GAMESTATE" || Game::Instance()->GetStateMachine()->GetCurrentState() == "DUNGEON")
		{
			game = true;
			pause = false;
		}

	}

	void OnExit()
	{
		/*for (auto entity : GetSystemEntities())
		{
			 entity.Kill();
		}*/
		auto entities = GetSystemEntities();

		for (auto i = entities.begin(); i != entities.end() - 1; i++)
		{
			Entity entity = *i;

			Registry::Instance()->RemoveEntityFromSystems(*i);
		}
	}
};
