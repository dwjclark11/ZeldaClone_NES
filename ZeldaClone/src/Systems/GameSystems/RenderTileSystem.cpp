#include "Systems/GameSystems/RenderTileSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TileComponent.h"
#include "AssetManager/AssetManager.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/Camera.h"

#include <SDL.h>
#include <algorithm>
#include <vector>

RenderTileSystem::RenderTileSystem()
	: m_Game( Game::Instance() )
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<TileComponent>();
}

void RenderTileSystem::Update()
{
	const auto& camera = m_Game.GetCamera();
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraHeight = camera.GetCameraHeight();
	const auto& cameraWidth = camera.GetCameraWidth();
	// Create a temporary struct to help sort the entities
	struct RenderableEntity
	{
		TransformComponent transformComponent;
		SpriteComponent spriteComponent;
	};

	// Create a vector to push back the system entities values
	std::vector<RenderableEntity> renderableEntities;

	// Loop through all of the system entities
	for ( const auto& entity : GetSystemEntities() )
	{
		if ( entity.HasComponent<TileComponent>() ) // Add the other groups
		{
			RenderableEntity renderableEntity;
			renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
			renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();

			// If the entities are outside of the camera's view, cull them out to save on processing.
			// There is no need to render entities that we cannot see on the screen.

			bool isEntityOutsideCameraView =
				( renderableEntity.transformComponent.position.x +
						  ( renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width ) <
					  cameraPos.x ||
				  renderableEntity.transformComponent.position.x > cameraPos.x + cameraWidth ||
				  renderableEntity.transformComponent.position.y +
						  ( renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height ) <
					  cameraPos.y ||
				  renderableEntity.transformComponent.position.y > cameraPos.y + cameraHeight );

			// Bypass Rendering entities, if we are outside the camera view and are not fixed sprites
			if ( isEntityOutsideCameraView && !renderableEntity.spriteComponent.bIsFixed )
			{
				continue;
			}

			renderableEntities.emplace_back( renderableEntity );
		}
	}

	// Sort the new vector using a lambda function that compares each renderableEntity and returns a bool value
	// This is to sort the entities by layer or Z index
	std::sort(
		renderableEntities.begin(), renderableEntities.end(), []( const RenderableEntity a, const RenderableEntity b ) {
			return a.spriteComponent.layer < b.spriteComponent.layer;
		} );

	// Loop all entities that the system is interested in
	for ( const auto& entity : renderableEntities )
	{
		const auto& transform = entity.transformComponent;
		const auto& sprite = entity.spriteComponent;

		// Set the destination rect with the x, y position to be rendered
		SDL_Rect dstRect = {
			static_cast<int>( transform.position.x -
							  ( sprite.bIsFixed ? 0 : cameraPos.x ) ), // If the sprite is fixed, subtract 0
			static_cast<int>( transform.position.y - ( sprite.bIsFixed ? 0 : cameraPos.y ) ),
			static_cast<int>( sprite.width * transform.scale.x ),
			static_cast<int>( sprite.height * transform.scale.y ) };

		auto& assetManager = m_Game.GetAssetManager();
		// If the player is dead change the background tile color to red
		if ( m_Game.GetPlayer()->GetPlayerDead() )
		{
			SDL_SetTextureColorMod( assetManager->GetTexture( sprite.assetID ), 255, 0, 0 );
		}
		else
			SDL_SetTextureColorMod( assetManager->GetTexture( sprite.assetID ), 255, 255, 255 );

		SDL_Texture* tex = assetManager->GetTexture( sprite.assetID );
		SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( tex, Game::Instance().GetCamera().GetFadeAlpha() );

		SDL_RenderCopyEx( m_Game.GetRenderer(),
						  tex,
						  &sprite.srcRect,
						  &dstRect,
						  transform.rotation,
						  NULL,		  // The rotation is done on the screnter of the sprite, width / 2, height / 2
						  sprite.flip // This is if we want to flup a sprite
		);
	}
}

void RenderTileSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}
