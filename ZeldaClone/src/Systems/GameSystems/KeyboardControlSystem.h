#pragma once

#include "../../ECS/ECS.h"
#include "../../Game/Game.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../States/GameState.h"

#include <string>
#include <SDL.h>
#include "../../Utilities/Timer.h"

enum directionE {NONE = 0, UP, RIGHT, DOWN, LEFT, ATTACK};

class KeyboardControlSystem : public System
{
private:
	class Game& game;
public:
	static directionE dir;
	static bool keyDown;
	bool attack;
	
	KeyboardControlSystem()
		: game(*Game::Instance())
	{
		RequiredComponent<RigidBodyComponent>();
		RequiredComponent<KeyboardControlComponent>();
		attack = false;
	}
	
	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		if (!game.GetCameraMoving() && !game.GetPlayerItem() && !game.GetPlayerDead())
		{
			eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed); // Callback Function
		}
	}

	// Callback function
	void OnKeyPressed(KeyPressedEvent& event)
	{
		if (game.GetFadeAlpha() == 255)
		{
			auto player				= Registry::Instance()->GetEntityByTag("player");
			auto& playerTransform	= player.GetComponent<TransformComponent>();
			auto& playerCollider	= player.GetComponent<BoxColliderComponent>();
			auto& playerRigidbody	= player.GetComponent<RigidBodyComponent>();
			auto& playerSprite		= player.GetComponent<SpriteComponent>();
			auto& playerControl		= player.GetComponent<KeyboardControlComponent>();
			
			auto shield				= Registry::Instance()->GetEntityByTag("the_shield");
			auto& shieldTransform	= shield.GetComponent<TransformComponent>();
			auto& shieldCollider	= shield.GetComponent<BoxColliderComponent>();
			auto& shieldRigidbody	= shield.GetComponent<RigidBodyComponent>();

			auto sword				= Registry::Instance()->GetEntityByTag("the_sword");
			auto& swordTransform	= sword.GetComponent<TransformComponent>();
			auto& swordCollider		= sword.GetComponent<BoxColliderComponent>();
			auto& swordRigidbody	= sword.GetComponent<RigidBodyComponent>();

			if (!game.GetPlayerItem())
			{
				switch (event.symbol)
				{
				case SDLK_w:
									
					playerRigidbody.velocity	= playerControl.upVelocity;
					playerSprite.srcRect.x		= playerSprite.width * 2;

					shieldTransform.position	= playerTransform.position;
					shieldCollider.height		= 2;
					shieldCollider.width		= 24;
					shieldCollider.offset		= glm::vec2(48, 32);
					shieldRigidbody				= playerRigidbody;

					swordTransform.position		= playerTransform.position;
					swordCollider.height		= 2;
					swordCollider.width			= 2;
					swordCollider.offset		= glm::vec2(64, 60);
					swordRigidbody				= playerRigidbody;

					dir = UP;
					break;

				case SDLK_d:
					playerSprite.srcRect.x		= playerSprite.width * 3;
					playerRigidbody.velocity	= playerControl.rightVelocity;

					shieldTransform.position	= playerTransform.position;
					shieldCollider.height		= 30;
					shieldCollider.width		= 2;
					shieldCollider.offset		= glm::vec2(90, 56);
					shieldRigidbody				= playerRigidbody;

					swordTransform.position		= playerTransform.position;
					swordCollider.height		= 2;
					swordCollider.width			= 2;
					swordCollider.offset		= glm::vec2(64, 60);
					swordRigidbody				= playerRigidbody;

					dir = RIGHT;
					break;

				case SDLK_s:
					playerRigidbody.velocity	= playerControl.downVelocity;
					playerSprite.srcRect.x		= playerSprite.width * 0;
									
					shieldTransform.position	= playerTransform.position;
					shieldCollider.height		= 2;
					shieldCollider.width		= 24;
					shieldCollider.offset		= glm::vec2(40, 84);
					shieldRigidbody				= playerRigidbody;

					swordTransform.position		= playerTransform.position;
					swordCollider.height		= 2;
					swordCollider.width			= 2;
					swordCollider.offset		= glm::vec2(64, 60);
					swordRigidbody				= playerRigidbody;

					dir = DOWN;
					break;

				case SDLK_a:
					playerRigidbody.velocity	= playerControl.leftVelocity;
					playerSprite.srcRect.x		= playerSprite.width * 1;
									
					shieldTransform.position	= playerTransform.position;
					shieldCollider.height		= 30;
					shieldCollider.width		= 2;
					shieldCollider.offset		= glm::vec2(30, 50);
					shieldRigidbody				= playerRigidbody;

					swordTransform.position		= playerTransform.position;
					swordCollider.height		= 2;
					swordCollider.width			= 2;
					swordCollider.offset		= glm::vec2(64, 60);
					swordRigidbody				= playerRigidbody;

					dir = LEFT;
					break;
				}
			}
		}
	}

	void UpdatePlayer();

};

