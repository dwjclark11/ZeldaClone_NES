#include "Systems/RenderCollisionSystem.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/TransformComponent.h"
#include <glm/glm.hpp>
#include <SDL.h>

RenderCollisionSystem::RenderCollisionSystem()
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<BoxColliderComponent>();
}

void RenderCollisionSystem::Update( SDL_Renderer* renderer, SDL_Rect& camera )
{
	for ( const auto& entity : GetSystemEntities() )
	{
		const auto& transform = entity.GetComponent<TransformComponent>();
		const auto& collider = entity.GetComponent<BoxColliderComponent>();

		const SDL_Rect srcRect = { transform.position.x + collider.offset.x - camera.x,
								   transform.position.y + collider.offset.y - camera.y,
								   collider.width * transform.scale.x,
								   collider.height * transform.scale.y };

		// Set the color for the trigger boxes
		if ( entity.HasComponent<TriggerBoxComponent>() )
		{
			SDL_SetRenderDrawColor( renderer, 0, 255, 0, 100 );
			SDL_RenderFillRect( renderer, &srcRect );
		}
		else // Set the color for physical collsions
		{
			SDL_SetRenderDrawColor( renderer, 255, 0, 0, 125 );
			SDL_RenderFillRect( renderer, &srcRect );
		}

		SDL_RenderDrawRect( renderer, &srcRect );
	}
}

void RenderCollisionSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		if ( entity.HasComponent<ColliderComponent>() || entity.HasComponent<TriggerBoxComponent>() )
		{
			entity.Kill();
		}
	}
}