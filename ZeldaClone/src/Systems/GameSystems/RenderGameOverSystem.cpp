#include "Systems/GameSystems/RenderGameOverSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "AssetManager/AssetManager.h"

#include <SDL.h>

RenderGameOverSystem::RenderGameOverSystem()
{
	// All Systems need a required component to help separate what entities need to
	// use that system
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
}

void RenderGameOverSystem::Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager )
{

	// Loop all entities that the system is interested in
	for ( const auto& entity : GetSystemEntities() )
	{
		if ( entity.BelongsToGroup( "game_over" ) )
		{
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& sprite = entity.GetComponent<SpriteComponent>();

			// Set the src Rect of our original sprite texture
			SDL_Rect srcRect = sprite.srcRect;

			// Set the destination rect with the x, y position to be rendered
			SDL_Rect dstRect = { static_cast<int>( transform.position.x ),
								 static_cast<int>( transform.position.y ),
								 static_cast<int>( sprite.width * transform.scale.x ),
								 static_cast<int>( sprite.height * transform.scale.y ) };

			SDL_RenderCopyEx( renderer,
							  assetManager->GetTexture( sprite.assetID ),
							  &srcRect,
							  &dstRect,
							  transform.rotation,
							  NULL,		  // The rotation is done on the screnter of the sprite, width / 2, height / 2
							  sprite.flip // This is if we want to flup a sprite
			);
		}
	}
}

void RenderGameOverSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		if ( entity.BelongsToGroup( "game_over" ) )
		{
			entity.Kill();
		}
	}
}