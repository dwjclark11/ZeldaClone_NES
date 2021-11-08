#include "ProjectileLifeCycleSystem.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Game/Game.h"
#include "../SoundFXSystem.h"

ProjectileLifeCycleSystem::ProjectileLifeCycleSystem()
	: game(*Game::Instance())
{
	RequiredComponent<ProjectileComponent>();
}

void ProjectileLifeCycleSystem::Update()
{
	for (auto entity : GetSystemEntities())
	{
		auto& projectile = entity.GetComponent<ProjectileComponent>();

		if ((SDL_GetTicks() - projectile.startTime) > projectile.duration && !entity.BelongsToGroup("bomber") && !entity.BelongsToGroup("beam"))
		{
			entity.Kill();
		}
			
		if ((SDL_GetTicks() - projectile.startTime) > projectile.duration && entity.BelongsToGroup("bomber"))
		{
			Entity explosion = entity.registry->CreateEntity();
			explosion.AddComponent<TransformComponent>(entity.GetComponent<TransformComponent>().position, glm::vec2(4, 4), 0.0);
			explosion.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 80, 112);
			explosion.AddComponent<RigidBodyComponent>(glm::vec2(0));
			explosion.AddComponent<ProjectileComponent>(false, 1, 500);
			explosion.AddComponent<BoxColliderComponent>(16, 16);
			explosion.AddComponent<AnimationComponent>(3, 10, false, false, 80);
			explosion.AddComponent<GameComponent>();
			explosion.Group("projectile");
			entity.Kill();

			game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "bomb_blow", 0, 1);
		}
		if ((SDL_GetTicks() - projectile.startTime) > projectile.duration && entity.BelongsToGroup("beam"))
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& animation = entity.GetComponent<AnimationComponent>();
			auto& projectile = entity.GetComponent<ProjectileComponent>();
			auto& rigid = entity.GetComponent<RigidBodyComponent>();
			
			rigid.velocity = glm::vec2(0);
			sprite.srcRect.y = 128;
			animation.vertical = false;
			animation.numFrames = 4;
			if (SDL_GetTicks() - projectile.startTime > projectile.duration + 500)
				entity.Kill();
		}
	}

}
