#include "KeyboardControlSystem.h"
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


directionE KeyboardControlSystem::dir = directionE::NONE;
bool KeyboardControlSystem::keyDown = false;

void KeyboardControlSystem::UpdatePlayer()
{
	auto player = Registry::Instance()->GetEntityByTag("player");
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& playerCollider = player.GetComponent<BoxColliderComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	auto& playerSprite = player.GetComponent<SpriteComponent>();
	auto& playerControl = player.GetComponent<KeyboardControlComponent>();

	auto shield = Registry::Instance()->GetEntityByTag("the_shield");
	auto& shieldTransform = shield.GetComponent<TransformComponent>();
	auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
	auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

	auto sword = Registry::Instance()->GetEntityByTag("the_sword");
	auto& swordTransform = sword.GetComponent<TransformComponent>();
	auto& swordCollider = sword.GetComponent<BoxColliderComponent>();
	auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

	// reset the sword
	swordRigidbody = glm::vec2(0);
	swordCollider.offset = glm::vec2(64, 60);
	swordCollider.height = 4;
	swordCollider.width = 4;

	// Reset Player velocity and box collider
	playerRigidbody = glm::vec2(0);
	playerCollider.offset = glm::vec2(45, 45);

	if (dir == UP)
	{
		playerSprite.srcRect.x = playerSprite.width * 2;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == RIGHT)
	{
		playerSprite.srcRect.x = playerSprite.width * 3;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == DOWN)
	{
		playerSprite.srcRect.x = playerSprite.width * 0;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == LEFT)
	{
		playerSprite.srcRect.x = playerSprite.width * 1;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}

	// Reset the Shield based on player direction
	shieldRigidbody = glm::vec2(0);
	if (dir == UP)
	{
		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(48, 32);
	}
	else if (dir == RIGHT)
	{
		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(90, 56);
	}
	else if (dir == DOWN)
	{
		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(40, 84);
	}
	else if (dir == LEFT)
	{
		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(30, 50);
	}
}

void KeyboardControlSystem::Update()
{
	if (game.GetFadeAlpha() == 255)
	{
		auto player = Registry::Instance()->GetEntityByTag("player");
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
		auto& playerSprite = player.GetComponent<SpriteComponent>();
		auto& playerControl = player.GetComponent<KeyboardControlComponent>();

		auto shield = Registry::Instance()->GetEntityByTag("the_shield");
		auto& shieldTransform = shield.GetComponent<TransformComponent>();
		auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
		auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

		auto sword = Registry::Instance()->GetEntityByTag("the_sword");
		auto& swordTransform = sword.GetComponent<TransformComponent>();
		auto& swordCollider = sword.GetComponent<BoxColliderComponent>();
		auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

		/*	if (!game.PlayerHold() && event.symbol != SDLK_RSHIFT)
			{*/
			/*switch (event.symbol)
			{
			case SDLK_w:*/
		const Uint8* state = SDL_GetKeyboardState(NULL);

		
		if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_RSHIFT])
		{


			playerRigidbody.velocity = playerControl.upVelocity;
			playerSprite.srcRect.x = playerSprite.width * 2;

			shieldTransform.position = playerTransform.position;
			shieldCollider.height = 2;
			shieldCollider.width = 24;
			shieldCollider.offset = glm::vec2(48, 32);
			shieldRigidbody = playerRigidbody;

			swordTransform.position = playerTransform.position;
			swordCollider.height = 2;
			swordCollider.width = 2;
			swordCollider.offset = glm::vec2(64, 60);
			swordRigidbody = playerRigidbody;

			dir = UP;
		}
		//break;

	/*case SDLK_d:*/
		if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_RSHIFT])
		{
			playerSprite.srcRect.x = playerSprite.width * 3;
			playerRigidbody.velocity = playerControl.rightVelocity;

			shieldTransform.position = playerTransform.position;
			shieldCollider.height = 30;
			shieldCollider.width = 2;
			shieldCollider.offset = glm::vec2(90, 56);
			shieldRigidbody = playerRigidbody;

			swordTransform.position = playerTransform.position;
			swordCollider.height = 2;
			swordCollider.width = 2;
			swordCollider.offset = glm::vec2(64, 60);
			swordRigidbody = playerRigidbody;

			dir = RIGHT;
		}
		//break;

	/*case SDLK_s:*/
		if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_RSHIFT])
		{
			playerRigidbody.velocity = playerControl.downVelocity;
			playerSprite.srcRect.x = playerSprite.width * 0;

			shieldTransform.position = playerTransform.position;
			shieldCollider.height = 2;
			shieldCollider.width = 24;
			shieldCollider.offset = glm::vec2(40, 84);
			shieldRigidbody = playerRigidbody;

			swordTransform.position = playerTransform.position;
			swordCollider.height = 2;
			swordCollider.width = 2;
			swordCollider.offset = glm::vec2(64, 60);
			swordRigidbody = playerRigidbody;

			dir = DOWN;
		}
		//break;

	/*case SDLK_a:*/
		if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_RSHIFT])
		{
			playerRigidbody.velocity = playerControl.leftVelocity;
			playerSprite.srcRect.x = playerSprite.width * 1;

			shieldTransform.position = playerTransform.position;
			shieldCollider.height = 30;
			shieldCollider.width = 2;
			shieldCollider.offset = glm::vec2(30, 50);
			shieldRigidbody = playerRigidbody;

			swordTransform.position = playerTransform.position;
			swordCollider.height = 2;
			swordCollider.width = 2;
			swordCollider.offset = glm::vec2(64, 60);
			swordRigidbody = playerRigidbody;

			dir = LEFT;
		}
	}
}

KeyboardControlSystem::KeyboardControlSystem()
	: game(*Game::Instance())
{
	RequiredComponent<RigidBodyComponent>();
	RequiredComponent<KeyboardControlComponent>();
	attack = false;
}

void KeyboardControlSystem::SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
{
	if (!game.GetCameraMoving() && !game.GetPlayerItem() && !game.GetPlayerDead())
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed); // Callback Function
	}
}

void KeyboardControlSystem::OnKeyPressed(KeyPressedEvent& event)
{
	if (game.GetFadeAlpha() == 255)
	{
		auto player = Registry::Instance()->GetEntityByTag("player");
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
		auto& playerSprite = player.GetComponent<SpriteComponent>();
		auto& playerControl = player.GetComponent<KeyboardControlComponent>();

		auto shield = Registry::Instance()->GetEntityByTag("the_shield");
		auto& shieldTransform = shield.GetComponent<TransformComponent>();
		auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
		auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

		auto sword = Registry::Instance()->GetEntityByTag("the_sword");
		auto& swordTransform = sword.GetComponent<TransformComponent>();
		auto& swordCollider = sword.GetComponent<BoxColliderComponent>();
		auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

		if (!game.PlayerHold() && event.symbol != SDLK_RSHIFT)
		{
			/*switch (event.symbol)
			{
			case SDLK_w:*/
			if (event.symbol == SDLK_w)
			{


				playerRigidbody.velocity = playerControl.upVelocity;
				playerSprite.srcRect.x = playerSprite.width * 2;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 2;
				shieldCollider.width = 24;
				shieldCollider.offset = glm::vec2(48, 32);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				dir = UP;
			}
			//break;

		/*case SDLK_d:*/
			if (event.symbol == SDLK_d)
			{
				playerSprite.srcRect.x = playerSprite.width * 3;
				playerRigidbody.velocity = playerControl.rightVelocity;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 30;
				shieldCollider.width = 2;
				shieldCollider.offset = glm::vec2(90, 56);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				dir = RIGHT;
			}
			//break;

		/*case SDLK_s:*/
			if (event.symbol == SDLK_s)
			{
				playerRigidbody.velocity = playerControl.downVelocity;
				playerSprite.srcRect.x = playerSprite.width * 0;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 2;
				shieldCollider.width = 24;
				shieldCollider.offset = glm::vec2(40, 84);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				dir = DOWN;
			}
			//break;

		/*case SDLK_a:*/
			if (event.symbol == SDLK_a)
			{
				playerRigidbody.velocity = playerControl.leftVelocity;
				playerSprite.srcRect.x = playerSprite.width * 1;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 30;
				shieldCollider.width = 2;
				shieldCollider.offset = glm::vec2(30, 50);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				dir = LEFT;
			}
			//break;
		//}
		}
	}
}

