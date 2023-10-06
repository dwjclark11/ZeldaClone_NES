#include "AnimationSystem.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/AIComponent.h"
#include "../../Game/Game.h"
#include "../../Game/Player.h"
#include <SDL.h>
#include "../../Utilities/Utility.h"

AnimationSystem::AnimationSystem()
{
	RequiredComponent<AnimationComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<TransformComponent>();
}

void AnimationSystem::Update()
{
	for (const auto& entity : GetSystemEntities())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		
		if (Game::Instance().GetStateMachine()->GetCurrentState() == "GAMESTATE")
		{
			auto& playerPos = Game::Instance().GetPlayer()->GetPlayerPos();
			int entX = transform.position.x / PANEL_WIDTH;
			int entY = transform.position.y / PANEL_HEIGHT;

			if (entX != playerPos.x || entY != playerPos.y)
				continue;
		}
	
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
			// If the animation is a horizontal scroll
			if (rigidbody.velocity != glm::vec2(0))
			{
				if (!animation.vertical)
				{
					sprite.srcRect.x = (animation.currentFrame * sprite.width) + animation.frameOffset;
				}
				else
				{
					sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
				}
			}
			else if (health.isHurt || health.healthPercentage <= 0) // If the enemy is hurt use this frame
				sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
		}
		else
		{
			auto playerTag = entity.HasTag("player");
			
			// If the animation is a vertical scroll use this
			if (animation.vertical && playerTag)
			{
				if (rigidbody.velocity != glm::vec2(0))
				{
					sprite.srcRect.y = animation.currentFrame * sprite.height;
				}
				else if (health.isHurt || health.healthPercentage <= 0) // If the enemy is hurt use this frame
					sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
			}
			else if (animation.vertical && !playerTag)
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
