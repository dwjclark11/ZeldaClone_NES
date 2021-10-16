#pragma once

#include "../../ECS/ECS.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/HealthComponent.h"
#include <SDL.h>

class AnimationSystem : public System
{
public:
	
	AnimationSystem()
	{
		RequiredComponent<AnimationComponent>();
		RequiredComponent<SpriteComponent>();
	}

	void Update()
	{
		for (auto entity : GetSystemEntities())
		{

			auto& animation = entity.GetComponent<AnimationComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
			auto health = HealthComponent(); 

			if (entity.HasComponent<HealthComponent>())
				health = entity.GetComponent<HealthComponent>();

			animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
			
			// If the animation is a vertical scroll use this
			if (animation.vertical)
			{
				if (rigidbody.velocity != glm::vec2(0))
				{
					sprite.srcRect.y = animation.currentFrame * sprite.height;
				}
				else if (health.isHurt) // If the enemy is hurt use this frame
					sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
			}
			// If the animation is a horizontal scroll
			else
			{
				sprite.srcRect.x = (animation.currentFrame * sprite.width) + animation.frameOffset;
			}
		}
	}
};