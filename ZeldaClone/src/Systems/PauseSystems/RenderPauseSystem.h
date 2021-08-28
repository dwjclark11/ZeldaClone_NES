#pragma once
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/PauseComponent.h"
#include "../AssetManager/AssetManager.h"
#include "../../States/GameState.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <algorithm>
#include <vector>

struct RenderPauseSystem : public System
{
	RenderPauseSystem()
	{
		// All Systems need a required component to help separate what entities need to 
		// use that system
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();
		RequiredComponent<PauseComponent>();

	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager)
	{
		// Loop all entities that the system is interested in
		for (auto entity : GetSystemEntities())
		{
			if (entity.HasTag("bombItem") && GameState::totalBombs == 0)
			{
				entity.Kill();
			}
			

			if (entity.BelongsToGroup("pause") || entity.HasTag("pauseSelector"))
			{
				const auto transform = entity.GetComponent<TransformComponent>();
				const auto sprite = entity.GetComponent<SpriteComponent>();

				// Set the src Rect of our original sprite texture
				SDL_Rect srcRect = sprite.srcRect;

				// Set the destination rect with the x, y position to be rendered
				SDL_Rect dstRect = {
					static_cast<int>(transform.position.x),
					static_cast<int>(transform.position.y),
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
	}

	void OnExit()
	{
		for (auto entity : GetSystemEntities())
		{
			if (!entity.HasTag("selectedItem"))
			{
				entity.Kill();
			}	
		}
	}
};
