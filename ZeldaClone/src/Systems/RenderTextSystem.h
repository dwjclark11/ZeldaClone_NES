#pragma once
#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include "../AssetManager/AssetManager.h"
#include "../States/GameState.h"
#include "../Game/Game.h"
#include "../Utilities/Utility.h"
#include <SDL.h>

class RenderTextSystem : public System
{
private:
	class Game& game;
public: 
	RenderTextSystem()
		: game(Game::Instance())
	{
		RequiredComponent<TextLabelComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera)
	{
		for (auto entity : GetSystemEntities())
		{
			
			auto& textLabel = entity.GetComponent<TextLabelComponent>();
			
			if (entity.HasTag("rupees"))
			{
				textLabel.text = std::to_string(GameState::totalRupees);
			}
			if (entity.HasTag("keys"))
			{
				textLabel.text = std::to_string(GameState::totalKeys);
			}
			if (entity.HasTag("bombs"))
			{
				textLabel.text = std::to_string(GameState::totalBombs);
			}

			if (game.GetStateMachine()->GetCurrentState() == "PAUSE")
			{
				if (entity.HasTag("rupees")) textLabel.position.y = 800;
				if (entity.HasTag("keys")) textLabel.position.y = 832;
				if (entity.HasTag("bombs")) textLabel.position.y = 864;
				if (entity.HasTag("lifeText")) textLabel.position.y = 810;
			}
			else if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE" || game.GetStateMachine()->GetCurrentState() == "DUNGEON")
			{
				if (entity.HasTag("rupees")) textLabel.position.y = 40;
				if (entity.HasTag("keys")) textLabel.position.y = 72;
				if (entity.HasTag("bombs")) textLabel.position.y = 104;
				if (entity.HasTag("lifeText")) textLabel.position.y = 50;
			}

			SDL_SetRenderDrawColor(renderer, textLabel.color.r, textLabel.color.g, textLabel.color.b, 255);

			SDL_Surface* surface = TTF_RenderText_Blended(
				assetManager->GetFont(textLabel.assetID),
				textLabel.text.c_str(), 
				textLabel.color
			);

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

			// As soon as we create a texture we can free the surface
			SDL_FreeSurface(surface);

			int labelWidth = 0;
			int labelHeight = 0;

			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

			SDL_Rect dstRect = {
				(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
				(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
				labelWidth,
				labelHeight
			};

			SDL_RenderCopy(renderer, texture, NULL, &dstRect);

			SDL_DestroyTexture(texture);
		}

	}
	void OnExit()
	{
		for (auto& entity : GetSystemEntities())
		{
			 entity.Kill();
		}
	}
};