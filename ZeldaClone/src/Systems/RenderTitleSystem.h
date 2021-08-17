#pragma once

#include "../Components/SpriteComponent.h"
#include "../Components/BackgroundImageComponent.h"
#include "../Components/AnimationComponent.h"
#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <algorithm>
#include <vector>

class RenderTitleSystem : public System
{
public:
	static int titleTimer;
	bool offset;

	RenderTitleSystem()
	{
		RequiredComponent<SpriteComponent>();
		RequiredComponent<BackgroundImageComponent>();
		offset = false;
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
	{
		
		for (auto entity : GetSystemEntities())
		{
			auto transform = entity.GetComponent<BackgroundImageComponent>();
			auto sprite = entity.GetComponent<SpriteComponent>();
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
				static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)), // If the sprite is fixed, subtract 0
				static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
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
	
	void OnExit();
}; 

