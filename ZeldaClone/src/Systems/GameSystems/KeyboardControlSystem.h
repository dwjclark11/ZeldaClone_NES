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
#include <string>
#include <SDL.h>


enum directionE {NONE = 0, UP, RIGHT, DOWN, LEFT, ATTACK};

class KeyboardControlSystem : public System
{

public:
	static directionE dir;
	static bool keyDown;
	

	bool attack;
	
	KeyboardControlSystem()
	{
		RequiredComponent<RigidBodyComponent>();
		RequiredComponent<KeyboardControlComponent>();
		attack = false;
	}
	
	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed); // Callback Function
	}

	// Callback function
	void OnKeyPressed(KeyPressedEvent& event)
	{
		auto entities = GetSystemEntities();
		// Loop all the entities that the system is interested in
		for (auto i = entities.begin(); i != entities.end(); i++)
		{
			if (i->HasTag("player"))
			{

				Entity a = *i;
				auto& playerTransform = a.GetComponent<TransformComponent>();
				auto& playerCollider = a.GetComponent<BoxColliderComponent>();
				auto& playerRigidbody = a.GetComponent<RigidBodyComponent>();
				auto& sprite = a.GetComponent<SpriteComponent>();
				const auto keyboardControl = a.GetComponent<KeyboardControlComponent>();
				// Loop all entities that still need to be checked 
				for (auto j = i; j != entities.end(); j++)
				{
					if (j->HasTag("the_sword") || j->HasTag("the_shield"))
					{
						Entity b = *j;

							// Bypass if we are testing the same entity
							if (a == b)
							{
								continue;
							}

						auto& bTransform = b.GetComponent<TransformComponent>();
						auto& bCollider = b.GetComponent<BoxColliderComponent>();
						auto& bRigidbody = b.GetComponent<RigidBodyComponent>();

						bool shield = false;
						bool sword = false;
						if (b.HasTag("the_shield")) shield = true;
						if (b.HasTag("the_sword")) sword = true;

						if (!Game::Instance()->GetCameraMoving())
						{
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

			/*for (auto entity : GetSystemEntities())
			{
				const auto keyboardControl = entity.GetComponent<KeyboardControlComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();
				auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();

				rigidbody.velocity = glm::vec2(0);
				if (entity.HasTag("player") || entity.HasTag("the_sword") || entity.HasTag("the_shield"))
				{
					bool shield = false;
					if (entity.HasTag("the_shield")) shield = true;

					auto& shieldBox = entity.GetComponent<BoxColliderComponent>();

					if (!Game::Instance()->GetCameraMoving())
					{
						switch (event.symbol)
						{
						case SDLK_w:

							if (shield)
							{
								shieldBox.height = 2;
								shieldBox.width = 24;
								shieldBox.offset = glm::vec2(68, -112);
							}

							rigidbody.velocity = keyboardControl.upVelocity;
							sprite.srcRect.x = sprite.width * 2;
							dir = UP;
							break;

						case SDLK_d:

							if (shield)
							{
								shieldBox.height = 24;
								shieldBox.width = 2;
								shieldBox.offset = glm::vec2(108, -86);
							}

							rigidbody.velocity = keyboardControl.rightVelocity;
							sprite.srcRect.x = sprite.width * 3;
							dir = RIGHT;
							break;

						case SDLK_s:

							if (shield)
							{
								shieldBox.height = 2;
								shieldBox.width = 24;
								shieldBox.offset = glm::vec2(60, -60);
							}

							rigidbody.velocity = keyboardControl.downVelocity;
							sprite.srcRect.x = sprite.width * 0;
							dir = DOWN;
							break;

						case SDLK_a:

							if (shield)
							{
								shieldBox.height = 24;
								shieldBox.width = 2;
								shieldBox.offset = glm::vec2(48, -86);
							}

							rigidbody.velocity = keyboardControl.leftVelocity;
							sprite.srcRect.x = sprite.width * 1;
							dir = LEFT;
							break;
						}
					}
				}
			}*/
		}
	}

	void UpdatePlayer();

};

