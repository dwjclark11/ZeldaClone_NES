#pragma once
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include <glm/glm.hpp>
#include <SDL.h>

class RenderCollisionSystem : public System
{
public:
	RenderCollisionSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<BoxColliderComponent>();
	}

	void Update(SDL_Renderer* renderer, SDL_Rect& camera)
	{
		auto entities = GetSystemEntities();
		for (auto entity : entities)
		{
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collider = entity.GetComponent<BoxColliderComponent>();

			const SDL_Rect srcRect = {
				transform.position.x + collider.offset.x - camera.x,
				transform.position.y + collider.offset.y - camera.y,
				collider.width * transform.scale.x,
				collider.height * transform.scale.y
			};

			// Set the color for the trigger boxes
			if (entity.BelongsToGroup("trigger"))
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 50);
				SDL_RenderFillRect(renderer, &srcRect);
			}
			else // Set the color for physical collsions
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
				SDL_RenderFillRect(renderer, &srcRect);
			}
			SDL_RenderDrawRect(renderer, &srcRect);
		}
	}

	void OnExit()
	{
		auto entities = GetSystemEntities();
		for (auto i = entities.begin(); i != entities.end() - 1; i++)
		{
			Entity entity = *i;
			if (entity.BelongsToGroup("colliders"))
				Registry::Instance()->RemoveEntityFromSystems(*i);
		}

		for (auto i = entities.begin(); i != entities.end() - 1; i++)
		{
			Entity entity = *i;
			if (entity.BelongsToGroup("trigger"))
				Registry::Instance()->RemoveEntityFromSystems(*i);
		}
	}
};