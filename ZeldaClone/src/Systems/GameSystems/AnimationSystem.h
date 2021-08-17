#pragma once

#include "../../ECS/ECS.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/RigidBodyComponent.h"
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

			animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
			
			if (animation.vertical)
			{
				if (rigidbody.velocity != glm::vec2(0))
				{
					sprite.srcRect.y = animation.currentFrame * sprite.height;
				}
			}
			else
			{
				sprite.srcRect.x = (animation.currentFrame * sprite.width) + animation.frameOffset;
			}
		}
	}
};