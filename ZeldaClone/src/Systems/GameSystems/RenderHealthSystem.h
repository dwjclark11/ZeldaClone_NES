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

				if (health.addHeart)
				{
					loadedHearts += 1;
					Logger::Log("Loader Hearts " + std::to_string(loadedHearts));
					health.addHeart = false;
				}

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
	
					if (numHearts <= 8)
					{	
						xPos = 724 + (numHearts - 3) * 32;
						yPos = 95;
					}
					else if (numHearts > 8 && numHearts <= 16)
					{	
						xPos = 628 + (numHearts - 8) * 32;
						yPos = 128;
						Logger::Log(std::to_string(health.maxHearts));
					}
					
					// Set the tag to the next heart!
					tag += std::to_string(numHearts);

					// Create a new player Heart
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
				// Draw All Hearts
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

