#include "RenderTitleSystem.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BackgroundImageComponent.h"
#include "../Components/AnimationComponent.h"
#include "../AssetManager/AssetManager.h"
#include "../Utilities/Camera.h"

#include <SDL.h>

int RenderTitleSystem::titleTimer = 0;

RenderTitleSystem::RenderTitleSystem()
{
	RequiredComponent<SpriteComponent>();
	RequiredComponent<BackgroundImageComponent>();
	offset = false;
}

void RenderTitleSystem::Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const Camera& camera)
{
	const auto& cameraPos = camera.GetCameraPos();
	for (auto& entity : GetSystemEntities())
	{
		const auto& transform = entity.GetComponent<BackgroundImageComponent>();
		const auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& animation = entity.GetComponent<AnimationComponent>();
		// Set the src Rect of our original sprite texture

		if (titleTimer >= 100 && entity.BelongsToGroup("title"))
		{
			if (!offset)
			{
				animation.frameOffset = 768;
				animation.numFrames = 9;

			}
			//Logger::Err(std::to_string(animation.currentFrame));
			if (animation.currentFrame == 8 && !offset)
			{
				animation.frameOffset = 3072;
				animation.numFrames = 3;
				offset = true;
			}
		}

		SDL_Rect srcRect = sprite.srcRect;

		// Set the destination rect with the x, y position to be rendered
		SDL_Rect dstRect = {
			static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : cameraPos.x)), // If the sprite is fixed, subtract 0
			static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : cameraPos.y)),
			static_cast<int>(sprite.width * transform.scale.x),
			static_cast<int>(sprite.height * transform.scale.y)
		};

		SDL_RenderCopyEx(
			renderer,
			assetManager->GetTexture(sprite.assetID),
			&srcRect,
			&dstRect,
			transform.rotation,
			NULL,					// The rotation is done on the screnter of the sprite, width / 2, height / 2
			sprite.flip				// This is if we want to flup a sprite
		);
	}
}
void RenderTitleSystem::OnExit()
{

	for (auto& entity : GetSystemEntities())
	{
		entity.Kill();
	}


	titleTimer = 0;
	offset = false;
}