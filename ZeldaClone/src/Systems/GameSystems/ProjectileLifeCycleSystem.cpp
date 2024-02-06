#include "ProjectileLifeCycleSystem.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/ProjectileComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Game/Game.h"
#include "../../Game/Player.h"
#include "../SoundFXSystem.h"
#include "../../Utilities/GameData.h"
#include "../../Utilities/Utility.h"

void ProjectileLifeCycleSystem::UpdateBombAnimation(Entity entity)
{
	auto explosion = entity.registry->CreateEntity();
	explosion.AddComponent<TransformComponent>(entity.GetComponent<TransformComponent>().position, glm::vec2(4, 4), 0.0);
	explosion.AddComponent<SpriteComponent>("items", 16, 16, 3, false, 80, 112);
	explosion.AddComponent<RigidBodyComponent>(glm::vec2(0));
	explosion.AddComponent<ProjectileComponent>(false, 1, 500);
	explosion.AddComponent<BoxColliderComponent>(16, 16);
	explosion.AddComponent<AnimationComponent>(3, 10, false, false, 80);
	explosion.AddComponent<GameComponent>();

	explosion.Group("explode");
	entity.Kill();

	m_Game.GetSoundPlayer().PlaySoundFX("bomb_blow", 0, SoundChannel::ANY);
}

void ProjectileLifeCycleSystem::UpdateBeanAnimation(Entity entity)
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

ProjectileLifeCycleSystem::ProjectileLifeCycleSystem()
	: m_Game(Game::Instance())
{
	RequiredComponent<ProjectileComponent>();
}

void ProjectileLifeCycleSystem::Update()
{
	for (auto& entity : GetSystemEntities())
	{
		// Get the transform position of the entity to check against the player pos
		auto& transform = entity.GetComponent<TransformComponent>();
		const int checkX = transform.position.x / PANEL_WIDTH;
		const int checkY = transform.position.y / PANEL_HEIGHT;
		const auto& playerPos = m_Game.GetPlayer()->GetPlayerPos();

		if (checkX != playerPos.x || checkY != playerPos.y)
		{
			entity.Kill();
			continue;
		}

		auto& projectile = entity.GetComponent<ProjectileComponent>();

		if ((SDL_GetTicks() - projectile.startTime) > projectile.duration)
		{
			if (!entity.BelongsToGroup("bomber") && !entity.BelongsToGroup("beam"))
			{
				entity.Kill();
			}
			else if (entity.BelongsToGroup("bomber"))
			{
				UpdateBombAnimation(entity);
			}
			else if (entity.BelongsToGroup("beam"))
			{
				UpdateBeanAnimation(entity);
			}	
		}
	}
}
