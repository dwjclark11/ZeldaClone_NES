#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/HUDComponent.h"
#include "../../Game/Game.h"
#include "../../States/GameState.h"
#include <glm/glm.hpp>
#include <SDL.h>

class RenderHealthSystem : public System
{
public:
	static unsigned int numHearts;
	static int loadedHearts;

	RenderHealthSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<BoxColliderComponent>();
		RequiredComponent<HealthComponent>();
	}

	void Update(SDL_Renderer* renderer, SDL_Rect& camera)
	{
		auto entities = GetSystemEntities();
		for (auto entity : entities)
		{
			if (entity.HasTag("player"))
			{


				const auto& transform = entity.GetComponent<TransformComponent>();
				const auto& collider = entity.GetComponent<BoxColliderComponent>();
				auto& health = entity.GetComponent<HealthComponent>();

				if (loadedHearts > 3)
				{
					numHearts++;
					loadedHearts--;
				}

				if (health.maxHearts != numHearts && numHearts <= 16)
				{
					int xPos = 0;
					int yPos = 0;
					std::string tag = "heart";
					if (numHearts == 4)
					{
						xPos = 756;
						yPos = 95;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 5)
					{
						xPos = 788;
						yPos = 95;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 6)
					{
						xPos = 820;
						yPos = 95;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 7)
					{
						xPos = 852;
						yPos = 95;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 8)
					{
						xPos = 884;
						yPos = 95;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 9)
					{
						xPos = 660;
						yPos = 128;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 10)
					{
						xPos = 692;
						yPos = 128;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 11)
					{
						xPos = 724;
						yPos = 128;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 12)
					{
						xPos = 756;
						yPos = 128;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 13)
					{
						xPos = 788;
						yPos = 128;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 14)
					{
						xPos = 820;
						yPos = 128;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 15)
					{
						xPos = 852;
						yPos = 128;
						tag += std::to_string(numHearts);
					}
					else if (numHearts == 16)
					{
						xPos = 884;
						yPos = 128;
						tag += std::to_string(numHearts);
					}

					Entity newHeart = Registry::Instance()->CreateEntity();
					newHeart.AddComponent<SpriteComponent>("hud_hearts", 16, 16, 5, true, 0, 0);
					newHeart.AddComponent<TransformComponent>(glm::vec2(xPos, yPos), glm::vec2(4, 4), 0.0);
					newHeart.AddComponent<HealthComponent>(0);
					newHeart.AddComponent<HUDComponenet>();
					newHeart.Tag(tag);
					newHeart.Group("hud");

					health.maxHearts += 1;
					health.healthPercentage = health.maxHearts * 2;
				}

				if (entity.HasTag("player"))
				{
					if (health.healthPercentage <= 0)
					{
						entity.Kill();
						GameState::playerDead = true;
					}
				}

				const SDL_Rect srcRect = {
					transform.position.x + collider.offset.x - camera.x,
					transform.position.y + collider.offset.y - camera.y,
					collider.width * transform.scale.x,
					collider.height * transform.scale.y
				};

			}
		}
	}

	void OnExit()
	{
		for (auto entity : GetSystemEntities())
		{
			if (!entity.HasTag("player"))
				entity.Kill();
			else
			{
				//GameState::playerDead = false;
				//entity.GetComponent<HealthComponent>().healthPercentage = 6;
			}
				
		}
	}

	void EmptyFunction();
};

