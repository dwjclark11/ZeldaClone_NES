#include "Systems/PauseSystems/RenderPauseSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PauseComponent.h"
#include "AssetManager/AssetManager.h"
#include "States/GameState.h"
#include "Game/Game.h"
#include "Utilities/Camera.h"

#include <SDL.h>

RenderPauseSystem::RenderPauseSystem()
{
	// All Systems need a required component to help separate what entities need to
	// use that system
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<PauseComponent>();
}

void RenderPauseSystem::Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager )
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
		Game::Instance().GetCamera().FadeScreen();

		SDL_Texture* tex = assetManager->GetTexture( sprite.assetID );
		SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( tex, Game::Instance().GetCamera().GetFadeAlpha() );

		SDL_RenderCopyEx( renderer, tex, &srcRect, &dstRect, transform.rotation, NULL, sprite.flip );
	}
}

void RenderPauseSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}
