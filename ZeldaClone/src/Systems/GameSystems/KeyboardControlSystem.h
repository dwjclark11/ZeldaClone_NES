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
		auto entities = GetSystemEntities();

		// Loop all the entities that the system is interested in
		for (auto i = entities.begin(); i != entities.end(); i++)
		{
			if (!game.GetPlayerItem())
			{
				if (i->HasTag("player"))
				{
					Entity a = *i;
					auto& playerTransform = a.GetComponent<TransformComponent>();
					auto& playerCollider = a.GetComponent<BoxColliderComponent>();
					auto& playerRigidbody = a.GetComponent<RigidBodyComponent>();
					auto& sprite = a.GetComponent<SpriteComponent>();
					const auto& keyboardControl = a.GetComponent<KeyboardControlComponent>();
					//Logger::Log("x: " + std::to_string(playerTransform.position.x) + ", y: " + std::to_string(playerTransform.position.y));
					// Loop all entities that still need to be checked 
					for (auto j = i; j != entities.end(); j++)
					{
						if (j->HasTag("the_sword") || j->HasTag("the_shield"))
						{
							Entity b = *j;

							// Bypass if we are testing the same entity
							if (a == b)
								continue;

							auto& bTransform = b.GetComponent<TransformComponent>();
							auto& bCollider = b.GetComponent<BoxColliderComponent>();
							auto& bRigidbody = b.GetComponent<RigidBodyComponent>();

							bool shield = false;
							bool sword = false;
							if (b.HasTag("the_shield")) shield = true;
							if (b.HasTag("the_sword")) sword = true;

							playerRigidbody.velocity = glm::vec2(0);
							bRigidbody.velocity = playerRigidbody.velocity;
							switch (event.symbol)
							{
							case SDLK_w:
								bTransform.position = playerTransform.position;
								playerRigidbody.velocity = keyboardControl.upVelocity;
								bRigidbody = playerRigidbody;
								sprite.srcRect.x = sprite.width * 2;
								if (shield)
								{
									bCollider.height = 2;
									bCollider.width = 24;
									bCollider.offset = glm::vec2(48, 32);
								}
								else if (sword)
								{
									bCollider.height = 2;
									bCollider.width = 2;
									bCollider.offset = glm::vec2(64, 60);
								}
								dir = UP;
								break;

							case SDLK_d:
								bTransform.position = playerTransform.position;
								playerRigidbody.velocity = keyboardControl.rightVelocity;
								bRigidbody = playerRigidbody;
								sprite.srcRect.x = sprite.width * 3;
								if (shield)
								{
									bCollider.height = 30;
									bCollider.width = 2;
									bCollider.offset = glm::vec2(90, 56);
								}
								else if (sword)
								{
									bCollider.height = 2;
									bCollider.width = 2;
									bCollider.offset = glm::vec2(64, 60);
								}
								dir = RIGHT;
								break;

							case SDLK_s:
								bTransform.position = playerTransform.position;
								playerRigidbody.velocity = keyboardControl.downVelocity;
								bRigidbody = playerRigidbody;
								sprite.srcRect.x = sprite.width * 0;
								if (shield)
								{
									bCollider.height = 2;
									bCollider.width = 24;
									bCollider.offset = glm::vec2(40, 84);
								}
								else if (sword)
								{
									bCollider.height = 2;
									bCollider.width = 2;
									bCollider.offset = glm::vec2(64, 60);
								}
								dir = DOWN;
								break;

							case SDLK_a:
								bTransform.position = playerTransform.position;
								playerRigidbody.velocity = keyboardControl.leftVelocity;
								bRigidbody = playerRigidbody;
								sprite.srcRect.x = sprite.width * 1;
								if (shield)
								{
									bCollider.height = 30;
									bCollider.width = 2;
									bCollider.offset = glm::vec2(30, 50);
								}
								else if (sword)
								{
									bCollider.height = 2;
									bCollider.width = 2;
									bCollider.offset = glm::vec2(64, 60);
								}

								dir = LEFT;
								break;
							}
						}
					}
				}
			}
		}
	}
	void UpdatePlayer();

};

