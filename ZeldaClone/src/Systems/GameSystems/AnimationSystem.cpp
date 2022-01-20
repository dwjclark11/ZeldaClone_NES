#include "AnimationSystem.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/AIComponent.h"
#include <SDL.h>

AnimationSystem::AnimationSystem()
{
	RequiredComponent<AnimationComponent>();
	RequiredComponent<SpriteComponent>();
}

void AnimationSystem::Update()
{
	for (const auto& entity : GetSystemEntities())
	{
		auto& animation = entity.GetComponent<AnimationComponent>();
		auto& sprite = entity.GetComponent<SpriteComponent>();


		auto health = HealthComponent();
		auto ai = AIComponent();
		auto rigidbody = RigidBodyComponent();

		// Check to see if the entity has a rigid body component
		if (entity.HasComponent<RigidBodyComponent>())
			rigidbody = entity.GetComponent<RigidBodyComponent>();

		// Set the cuurent frame
		animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;

		// Check to see if the entity has a Health Component
		if (entity.HasComponent<HealthComponent>())
			health = entity.GetComponent<HealthComponent>();

		// Check for AI component
		if (entity.HasComponent<AIComponent>())
		{
			ai = entity.GetComponent<AIComponent>();

			if (rigidbody.velocity != glm::vec2(0))
			{
				sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
			}
			else if (health.isHurt) // If the enemy is hurt use this frame
				sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
		}
		else
		{
			// If the animation is a vertical scroll use this
			if (animation.vertical && (entity.HasTag("player")))
			{
				if (rigidbody.velocity != glm::vec2(0))
				{
					sprite.srcRect.y = animation.currentFrame * sprite.height;
				}
				else if (health.isHurt) // If the enemy is hurt use this frame
					sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
			}
			else if (animation.vertical && !entity.HasTag("player"))
			{
				sprite.srcRect.y = animation.currentFrame * sprite.height;
			}

			// If the animation is a horizontal scroll
			if (!animation.vertical)
			{
				sprite.srcRect.x = (animation.currentFrame * sprite.width) + animation.frameOffset;
			}
		}
	}
}