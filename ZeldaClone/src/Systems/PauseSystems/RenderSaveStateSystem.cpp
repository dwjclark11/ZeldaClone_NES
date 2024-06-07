#include "Systems/PauseSystems/RenderSaveStateSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SaveComponent.h"
#include "AssetManager/AssetManager.h"
#include "States/GameState.h"

#include <SDL.h>

RenderSaveStateSystem::RenderSaveStateSystem()
{
	// All Systems need a required component to help separate what entities need to
	// use that system
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<SaveComponent>();
}

void RenderSaveStateSystem::Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager )
{
	// Loop all entities that the system is interested in
	for ( auto& entity : GetSystemEntities() )
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
						  NULL,
						  sprite.flip );
	}
}

void RenderSaveStateSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}