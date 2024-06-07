#include "Systems/RenderTextSystem.h"
#include "Components/TextLabelComponent.h"
#include "Components/SettingsComponent.h"
#include "AssetManager/AssetManager.h"
#include "States/GameState.h"
#include "Game/Game.h"
#include "Utilities/Utility.h"
#include "Utilities/Camera.h"

#include <SDL.h>

RenderTextSystem::RenderTextSystem()
	: m_Game( Game::Instance() )
{
	RequiredComponent<TextLabelComponent>();
}

void RenderTextSystem::Update()
{
	const auto& camera = m_Game.GetCamera();
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraHeight = camera.GetCameraHeight();
	const auto& cameraWidth = camera.GetCameraWidth();

	for ( const auto& entity : GetSystemEntities() )
	{
		auto& textLabel = entity.GetComponent<TextLabelComponent>();

		SDL_SetRenderDrawColor( m_Game.GetRenderer(), textLabel.color.r, textLabel.color.g, textLabel.color.b, 255 );

		SDL_Surface* surface = TTF_RenderText_Blended(
			m_Game.GetAssetManager()->GetFont( textLabel.assetID ), textLabel.text.c_str(), textLabel.color );

		SDL_Texture* texture = SDL_CreateTextureFromSurface( m_Game.GetRenderer(), surface );

		// As soon as we create a texture we can free the surface
		SDL_FreeSurface( surface );

		int labelWidth = 0;
		int labelHeight = 0;

		SDL_QueryTexture( texture, NULL, NULL, &labelWidth, &labelHeight );

		SDL_Rect dstRect = { ( textLabel.position.x - ( textLabel.bIsFixed ? 0 : cameraPos.x ) ),
							 ( textLabel.position.y - ( textLabel.bIsFixed ? 0 : cameraPos.y ) ),
							 labelWidth,
							 labelHeight };

		SDL_RenderCopy( m_Game.GetRenderer(), texture, NULL, &dstRect );

		SDL_DestroyTexture( texture );
	}
}

void RenderTextSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}

void RenderTextSystem::OnExitSettings()
{
	for ( auto& entity : GetSystemEntities() )
	{
		if ( entity.HasComponent<SettingsComponent>() )
		{
			entity.Kill();
		}
	}
}