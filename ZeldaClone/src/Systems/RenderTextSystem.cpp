#include "RenderTextSystem.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/SettingsComponent.h"
#include "../AssetManager/AssetManager.h"
#include "../States/GameState.h"
#include "../Game/Game.h"
#include "../Utilities/Utility.h"
#include <SDL.h>

RenderTextSystem::RenderTextSystem()
	: game(Game::Instance())
{
	RequiredComponent<TextLabelComponent>();
}

void RenderTextSystem::Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera)
{
	for (const auto& entity : GetSystemEntities())
	{
		auto& textLabel = entity.GetComponent<TextLabelComponent>();

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

void RenderTextSystem::OnExit()
{
	for (auto& entity : GetSystemEntities())
	{
		entity.Kill();
	}
}

void RenderTextSystem::OnExitSettings()
{
	for (auto& entity : GetSystemEntities())
	{
		if (entity.HasComponent<SettingsComponent>())
		{
			entity.Kill();
		}
	}
}