#include "Systems/GameSystems/AnimationSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TransformComponent.h"
#include "Components/AIComponent.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/Utility.h"
#include "States/State.h"
#include "States/GameStateMachine.h"

#include <SDL.h>

AnimationSystem::AnimationSystem()
{
	RequiredComponent<AnimationComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<TransformComponent>();
}

void AnimationSystem::Update()
{
	auto& pPlayer = Game::Instance().GetPlayer();
	glm::vec2 playerPos{ 0.f };

	if ( pPlayer )
		playerPos = pPlayer->GetPlayerPos();

	for ( const auto& entity : GetSystemEntities() )
	{
		auto& transform = entity.GetComponent<TransformComponent>();

		if ( Game::Instance().GetStateMachine()->GetCurrentState() == "GAMESTATE" )
		{
			if ( pPlayer )
			{
				int entX = transform.position.x / PANEL_WIDTH;
				int entY = transform.position.y / PANEL_HEIGHT;

				if ( entX != playerPos.x || entY != playerPos.y )
					continue;
			}
		}

		auto& animation = entity.GetComponent<AnimationComponent>();
		auto& sprite = entity.GetComponent<SpriteComponent>();

		HealthComponent health{};
		AIComponent ai{};
		RigidBodyComponent rigidbody{};

		if ( animation.numFrames <= 0 || animation.bFinished )
			continue;

		// Check to see if the entity has a rigid body component
		if ( entity.HasComponent<RigidBodyComponent>() )
			rigidbody = entity.GetComponent<RigidBodyComponent>();

		// Set the cuurent frame
		animation.currentFrame =
			( ( SDL_GetTicks() - animation.startTime ) * animation.frameSpeedRate / 1000 ) % animation.numFrames;

		// Check to see if the entity has a Health Component
		if ( entity.HasComponent<HealthComponent>() )
			health = entity.GetComponent<HealthComponent>();

		// Check for AI component
		if ( entity.HasComponent<AIComponent>() )
		{
			// If the animation is a horizontal scroll
			if ( rigidbody.velocity != glm::vec2( 0 ) || health.bIsHurt || health.healthPercentage <= 0 )
			{
				if ( !animation.vertical )
				{
					sprite.srcRect.x = ( animation.currentFrame * sprite.width ) + animation.frameOffset;
				}
				else
				{
					sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
				}
			}
		}
		else
		{
			auto playerTag = entity.HasTag( "player" );

			// If the animation is a vertical scroll use this
			if ( animation.vertical && playerTag )
			{
				if ( rigidbody.velocity != glm::vec2( 0 ) )
				{
					sprite.srcRect.y = animation.currentFrame * sprite.height;
				}
				else if ( health.bIsHurt || health.healthPercentage <= 0 ) // If the enemy is hurt use this frame
					sprite.srcRect.y = animation.currentFrame * sprite.height + animation.frameOffset;
			}
			else if ( animation.vertical && !playerTag )
			{
				sprite.srcRect.y = animation.currentFrame * sprite.height;
			}

			// If the animation is a horizontal scroll
			if ( !animation.vertical )
			{
				sprite.srcRect.x = ( animation.currentFrame * sprite.width ) + animation.frameOffset;
			}
		}

		// If not looped and at the last frame --> set finished
		if ( !animation.isLooped && animation.currentFrame >= animation.numFrames - 1 )
			animation.bFinished = true;
	}
}
