#include "RenderHUDSystem.h"
#include "../../Game/Game.h"
#include "../../Components/HUDComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../AssetManager/AssetManager.h"
#include <SDL.h>
#include <algorithm>
#include <vector>

RenderHUDSystem::RenderHUDSystem()
	: game(Game::Instance())
{
	// All Systems need a required component to help separate what entities need to 
	// use that system
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<HUDComponenet>();
	pause = false;
	gamePlay = false;
}

void RenderHUDSystem::Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager)
{
	// Loop all entities that the system is interested in
	for (const auto& entity : GetSystemEntities())
	{
		if (!game.GetGameItems().woodSword && entity.HasTag("hudSword"))
		{
			continue;
		}

		if (entity.HasComponent<HUDComponenet>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();

			// Set the src Rect of our original sprite texture
			SDL_Rect srcRect = sprite.srcRect;

			if (game.GetStateMachine()->GetCurrentState() == "PAUSE" && !pause)
			{
				gamePlay = false;
				transform.position.y += OFFSET;
			}
			else if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE" || game.GetStateMachine()->GetCurrentState() == "DUNGEON" && !gamePlay)
			{
				// If we were previously in the pause state, subract the offset from all HUD entities
				if (pause)
				{
					transform.position.y -= OFFSET;
				}
			}

			if (entity.HasTag("hudItem"))
			{
				switch (Game::mSelectedItem)
				{
				case Game::ItemType::BOOMERANG:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 0;
					sprite.srcRect.y += sprite.height * 7;

					break;
				case Game::ItemType::BOMB:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 4;
					sprite.srcRect.y += sprite.height * 7;
					break;

				case Game::ItemType::WOOD_BOW:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 4;
					break;

				case Game::ItemType::CANDLE:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.y += sprite.height * 3;
					break;

				case Game::ItemType::FOOD:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 7;
					sprite.srcRect.y += sprite.height * 1;
					break;

				case Game::ItemType::POTION_RED:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 2;
					sprite.srcRect.y += sprite.height * 2;
					break;

				case Game::ItemType::MAGIC_ROD:
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
				transform.position.x = 10 + (game.GetPlayerPos().x * 16);
				transform.position.y = 40 + (game.GetPlayerPos().y * 16);
			}

			// Set the destination rect with the x, y position to be rendered
			SDL_Rect dstRect = {
				(transform.position.x),
				(transform.position.y),
				(sprite.width * transform.scale.x),
				(sprite.height * transform.scale.y)
			};

			SDL_Texture* tex = assetManager->GetTexture(sprite.assetID);
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(tex, Game::Instance().GetFadeAlpha());

			SDL_RenderCopyEx(
				renderer,
				//assetManager->GetTexture(sprite.assetID),
				tex,
				&srcRect,
				&dstRect,
				transform.rotation,
				NULL,					// The rotation is done on the center of the sprite, width / 2, height / 2
				sprite.flip				// This is if we want to flip a sprite
			);
		}
	}
	// Reset the HUD states
	if (game.GetStateMachine()->GetCurrentState() == "PAUSE")
	{
		pause = true;
		gamePlay = false;
	}

	if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE" || game.GetStateMachine()->GetCurrentState() == "DUNGEON")
	{
		gamePlay = true;
		pause = false;
	}

}

void RenderHUDSystem::OnExit()
{
	for (auto& entity : GetSystemEntities())
	{
		entity.Kill();
	}
}
