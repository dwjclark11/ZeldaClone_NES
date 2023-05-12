#include "RenderSystem.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Components/PlayerComponent.h"
#include "../../Components/PauseComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/SettingsComponent.h"
#include "../../Components/AIComponent.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Game/Game.h"

#include <SDL.h>
#include <algorithm>
#include <vector>
#include "../../Utilities/Camera.h"

RenderSystem::RenderSystem()
	: game(Game::Instance())
{
	// Entities polled must have these component
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<GameComponent>();
}

void RenderSystem::Update()
{
	auto& camera = game.GetCamera();
	const auto& cameraPos = camera.GetCameraPos();
	const auto& cameraHeight = camera.GetCameraHeight();
	const auto& cameraWidth = camera.GetCameraWidth();
	// Create a temporary struct to help sort the entities
	struct RenderableEntity
	{
		TransformComponent transformComponent;
		SpriteComponent spriteComponent;
	};

	// Create a vector to push back the system entities values
	std::vector<RenderableEntity> renderableEntities;

	// Loop through all of the system entities
	for (const auto& entity : GetSystemEntities())
	{
		RenderableEntity renderableEntity;
		renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
		renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();

		// If the entities are outside of the camera's view, cull them out to save on processing. 
		// There is no need to render entities that we cannot see on the screen.

		bool isEntityOutsideCameraView = (
			renderableEntity.transformComponent.position.x + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < cameraPos.x ||
			renderableEntity.transformComponent.position.x > cameraPos.x + cameraWidth ||
			renderableEntity.transformComponent.position.y + (renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height) < cameraPos.y ||
			renderableEntity.transformComponent.position.y > cameraPos.y + cameraHeight
			);

		// Bypass Rendering entities, if we are outside the camera view and are not fixed sprites
		if (isEntityOutsideCameraView && !renderableEntity.spriteComponent.isFixed)
			continue;

		renderableEntities.emplace_back(renderableEntity);
	}

	// Sort the new vector using a lambda function that compares each renderableEntity and returns a bool value
	// This is to sort the entities by layer or Z index
	std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity a, const RenderableEntity b)
		{
			return a.spriteComponent.layer < b.spriteComponent.layer;
		});

	//auto entities = GetSystemEntities();
	//std::sort(entities.begin(), entities.end(), [](Entity a, Entity b) {
	//	const auto& sprite_a = a.GetComponent<SpriteComponent>();
	//	const auto& sprite_b = b.GetComponent<SpriteComponent>();
	//	return sprite_a.layer < sprite_b.layer;
	//});

	//for (const auto& entity : entities)
	//{
	//	const auto& sprite = entity.GetComponent<SpriteComponent>();
	//	const auto& transform = entity.GetComponent<TransformComponent>();

	//	bool isEntityOutsideCameraView = (
	//	transform.position.x + (transform.scale.x * sprite.width) < cameraPos.x ||
	//		transform.position.x > cameraPos.x + cameraWidth ||
	//		transform.position.y + (transform.scale.y * sprite.height) < cameraPos.y ||
	//		transform.position.y > cameraPos.y + cameraHeight
	//	);

	//// Bypass Rendering entities, if we are outside the camera view and are not fixed sprites
	//if (isEntityOutsideCameraView && !sprite.isFixed)
	//	continue;

	//	// Set the src Rect of our original sprite texture
	//	SDL_Rect srcRect = sprite.srcRect;

	//	// Set the destination rect with the x, y position to be rendered
	//	SDL_Rect dstRect = {
	//		static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : cameraPos.x)), // If the sprite is fixed, subtract 0
	//		static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : cameraPos.y)),
	//		static_cast<int>(sprite.width * transform.scale.x),
	//		static_cast<int>(sprite.height * transform.scale.y)
	//	};

	//	camera.FadeScreen();

	//	SDL_Texture* tex = game.GetAssetManager()->GetTexture(sprite.assetID);
	//	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	//	SDL_SetTextureAlphaMod(tex, camera.GetFadeAlpha());

	//	SDL_RenderCopyEx(
	//		game.GetRenderer(),
	//		tex,
	//		&srcRect,
	//		&dstRect,
	//		transform.rotation,
	//		NULL,					// The rotation is done on the screnter of the sprite, width / 2, height / 2
	//		sprite.flip				// This is if we want to flup a sprite
	//	);
	//}

	// Loop all entities that the system is interested in
	for (const auto& entity : renderableEntities)
	{
		const auto& transform = entity.transformComponent;
		const auto& sprite = entity.spriteComponent;

		// Set the src Rect of our original sprite texture
		SDL_Rect srcRect = sprite.srcRect;

		// Set the destination rect with the x, y position to be rendered
		SDL_Rect dstRect = {
			static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : cameraPos.x)), // If the sprite is fixed, subtract 0
			static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : cameraPos.y)),
			static_cast<int>(sprite.width * transform.scale.x),
			static_cast<int>(sprite.height * transform.scale.y)
		};
		
		camera.FadeScreen();

		SDL_Texture* tex = game.GetAssetManager()->GetTexture(sprite.assetID);
		SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(tex, camera.GetFadeAlpha());

		SDL_RenderCopyEx(
			game.GetRenderer(),
			tex,
			&srcRect,
			&dstRect,
			transform.rotation,
			NULL,					// The rotation is done on the screnter of the sprite, width / 2, height / 2
			sprite.flip				// This is if we want to flup a sprite
		);
	}
}


void RenderSystem::OnExit()
{
	for (auto& entity : GetSystemEntities())
	{
		if (entity.HasComponent<PlayerComponent>())
			continue;

		if (entity.BelongsToGroup("NPC"))
		{
			if (entity.HasComponent<AnimationComponent>())
				entity.RemoveComponent<AnimationComponent>();
			
			entity.Kill();
			continue;
		}

		if (entity.HasComponent<AIComponent>())
			entity.GetComponent<AIComponent>().GarbageCollect();
			
		entity.Kill();		
	}
}


