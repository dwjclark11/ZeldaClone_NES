#include "Systems/RenderTitleSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/BackgroundImageComponent.h"
#include "Components/AnimationComponent.h"
#include "AssetManager/AssetManager.h"
#include "Utilities/Camera.h"

#include <SDL.h>

RenderTitleSystem::RenderTitleSystem()
{
	RequiredComponent<SpriteComponent>();
	RequiredComponent<BackgroundImageComponent>();
}

void RenderTitleSystem::Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager,
								const Camera& camera )
{
	const auto& cameraPos = camera.GetCameraPos();

	auto entities = GetSystemEntities();
	std::sort( entities.begin(), entities.end(), []( const Entity a, const Entity b ) {
		const auto& spriteA = a.GetComponent<SpriteComponent>();
		const auto& spriteB = b.GetComponent<SpriteComponent>();
		return spriteA.layer > spriteB.layer;
	} );

	for ( auto& entity : entities )
	{
		const auto& transform = entity.GetComponent<BackgroundImageComponent>();
		const auto& sprite = entity.GetComponent<SpriteComponent>();

		SDL_Rect srcRect = sprite.srcRect;

		// Set the destination rect with the x, y position to be rendered
		SDL_Rect dstRect = { static_cast<int>( transform.position.x - ( sprite.bIsFixed ? 0 : cameraPos.x ) ),
							 static_cast<int>( transform.position.y - ( sprite.bIsFixed ? 0 : cameraPos.y ) ),
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
void RenderTitleSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}