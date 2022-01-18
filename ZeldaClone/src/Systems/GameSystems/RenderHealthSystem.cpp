#include "RenderHealthSystem.h"

#include "../../Components/BoxColliderComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/HUDComponent.h"
#include "../../Game/Game.h"
#include "../../States/GameState.h"
#include <glm/glm.hpp>

#include <SDL.h>


RenderHealthSystem::RenderHealthSystem()
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<BoxColliderComponent>();
	RequiredComponent<HealthComponent>();
}


void RenderHealthSystem::Update(SDL_Renderer* renderer, SDL_Rect& camera)
{
	for (const auto& entity : GetSystemEntities())
	{
		if (entity.HasTag("player"))
		{
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collider = entity.GetComponent<BoxColliderComponent>();

			// Draw All Hearts
			const SDL_Rect srcRect = {
				transform.position.x + collider.offset.x - camera.x,
				transform.position.y + collider.offset.y - camera.y,
				collider.width * transform.scale.x,
				collider.height * transform.scale.y
			};
		}
	}
}

void RenderHealthSystem::OnExit()
{
	for (auto& entity : GetSystemEntities())
	{
		if (!entity.HasTag("player"))
			entity.Kill();
	}
}

