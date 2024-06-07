#include "Systems/NameSystems/RenderNameStateTextSystem.h"
#include "Components/TextLabelComponent.h"
#include "Components/RegisterNameComponent.h"
#include "AssetManager/AssetManager.h"
#include "Game/Game.h"

#include <SDL.h>

RenderNameStateTextSystem::RenderNameStateTextSystem()
{
	RequiredComponent<TextLabelComponent>();
}

void RenderNameStateTextSystem::Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager,
										const SDL_Rect& camera )
{
	for ( auto& entity : GetSystemEntities() )
	{
		auto& textLabel = entity.GetComponent<TextLabelComponent>();

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

void RenderNameStateTextSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}