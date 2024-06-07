#pragma once
#include "ECS/ECS.h"
#include "Components/TextLabelComponent.h"
#include "AssetManager/AssetManager.h"
#include "States/GameState.h"
#include "Game/Game.h"
#include "Systems/RenderTextSystem.h"
#include <SDL.h>

class RenderGameOverTextSystem : public System
{
  public:
	RenderGameOverTextSystem() { RequiredComponent<TextLabelComponent>(); }

	void Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera )
	{
		for ( const auto& entity : GetSystemEntities() )
		{
			if ( entity.BelongsToGroup( "game_over" ) )
			{
				const auto& textLabel = entity.GetComponent<TextLabelComponent>();

				SDL_SetRenderDrawColor( renderer, textLabel.color.r, textLabel.color.g, textLabel.color.b, 255 );

				SDL_Surface* surface = TTF_RenderText_Blended(
					assetManager->GetFont( textLabel.assetID ), textLabel.text.c_str(), textLabel.color );

				SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );

				// As soon as we create a texture we can free the surface
				SDL_FreeSurface( surface );

				int labelWidth = 0;
				int labelHeight = 0;

				SDL_QueryTexture( texture, NULL, NULL, &labelWidth, &labelHeight );

				SDL_Rect dstRect = { static_cast<int>( textLabel.position.x - ( textLabel.bIsFixed ? 0 : camera.x ) ),
									 static_cast<int>( textLabel.position.y - ( textLabel.bIsFixed ? 0 : camera.y ) ),
									 labelWidth,
									 labelHeight };

				SDL_RenderCopy( renderer, texture, NULL, &dstRect );

				SDL_DestroyTexture( texture );
			}
		}
	}

	void OnExit()
	{
		for ( auto& entity : GetSystemEntities() )
		{
			entity.Kill();
		}
	}
};
