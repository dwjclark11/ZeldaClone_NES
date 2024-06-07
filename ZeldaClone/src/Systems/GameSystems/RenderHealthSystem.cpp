#include "Systems/GameSystems/RenderHealthSystem.h"

#include "Components/BoxColliderComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/HUDComponent.h"
#include "Game/Game.h"
#include "States/GameState.h"
#include "Utilities/Camera.h"

#include <glm/glm.hpp>
#include <SDL.h>

RenderHealthSystem::RenderHealthSystem()
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<BoxColliderComponent>();
	RequiredComponent<HealthComponent>();
	RequiredComponent<PlayerComponent>();
}

void RenderHealthSystem::Update( SDL_Renderer* renderer, const Camera& camera )
{
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraHeight = camera.GetCameraHeight();
	const auto& cameraWidth = camera.GetCameraWidth();

	for ( const auto& entity : GetSystemEntities() )
	{
		if ( entity.HasTag( "player" ) )
		{
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collider = entity.GetComponent<BoxColliderComponent>();

			// Draw All Hearts
			const SDL_Rect srcRect = { transform.position.x + collider.offset.x - cameraPos.x,
									   transform.position.y + collider.offset.y - cameraPos.y,
									   collider.width * transform.scale.x,
									   collider.height * transform.scale.y };
		}
	}
}

void RenderHealthSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		if ( entity.HasComponent<PlayerComponent>() )
			continue;

		entity.Kill();
	}
}
