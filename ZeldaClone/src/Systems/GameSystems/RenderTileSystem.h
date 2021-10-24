
#pragma once
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/TileComponent.h"
#include "../../AssetManager/AssetManager.h"
#include "../../ECS/ECS.h"
#include <SDL.h>
#include <algorithm>
#include <vector>

struct RenderTileSystem : public System
{
	RenderTileSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();
		RequiredComponent<TileComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
	{
		// Create a temporary struct to help sort the entities
		struct RenderableEntity
		{
			TransformComponent transformComponent;
			SpriteComponent spriteComponent;
		};

		// Create a vector to push back the system entities values
		std::vector<RenderableEntity> renderableEntities;

		// Loop through all of the system entities
		for (auto entity : GetSystemEntities())
		{
			if (entity.BelongsToGroup("tiles") || entity.BelongsToGroup("map")) //Add the other groups
			{
				RenderableEntity renderableEntity;
				renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
				renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();

				// If the entities are outside of the camera's view, cull them out to save on processing. 
				// There is no need to render entities that we cannot see on the screen.

				bool isEntityOutsideCameraView = (
					renderableEntity.transformComponent.position.x + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.x ||
					renderableEntity.transformComponent.position.x > camera.x + camera.w ||
					renderableEntity.transformComponent.position.y + (renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height) < camera.y ||
					renderableEntity.transformComponent.position.y > camera.y + camera.h
					);

				// Bypass Rendering entities, if we are outside the camera view and are not fixed sprites
				if (isEntityOutsideCameraView && !renderableEntity.spriteComponent.isFixed)
				{
					continue;
				}

				renderableEntities.emplace_back(renderableEntity);
			}

			// Sort the new vector using a lambda function that compares each renderableEntity and returns a bool value
			// This is to sort the entities by layer or Z index
			std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity a, const RenderableEntity b)
				{
					return a.spriteComponent.layer < b.spriteComponent.layer;
				});

			// Loop all entities that the system is interested in
			for (auto entity : renderableEntities)
			{
				const auto transform = entity.transformComponent;
				const auto sprite = entity.spriteComponent;

				// Set the src Rect of our original sprite texture
				SDL_Rect srcRect = sprite.srcRect;

				// Set the destination rect with the x, y position to be rendered
				SDL_Rect dstRect = {
					static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)), // If the sprite is fixed, subtract 0
					static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
					static_cast<int>(sprite.width * transform.scale.x),
					static_cast<int>(sprite.height * transform.scale.y)
				};

				// If the player is dead change the background tile color to red
				if (Game::Instance()->GetPlayerDead())
					SDL_SetTextureColorMod(assetManager->GetTexture(sprite.assetID), 255, 0, 0);

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
			 entity.Kill();
			 if (entity.BelongsToGroup("map"))
				Logger::Err("Deleted map" + std::to_string(entity.GetID()));
		}
		auto entities = GetSystemEntities();

		for (auto i = entities.begin(); i != entities.end() - 1; i++)
		{
			Entity entity = *i;

			Registry::Instance()->RemoveEntityFromSystems(*i);
		}
	}
};