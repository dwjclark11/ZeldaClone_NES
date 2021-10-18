#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Game/Game.h"
#include "../../Systems/SoundFXSystem.h"

class HealthSystem : public System
{
public:
	static unsigned int currentHealth;
	static unsigned int maxHearts;
	bool lowHealth = false;
	HealthSystem()
	{
		RequiredComponent<HealthComponent>();
		RequiredComponent<SpriteComponent>();
	}

	void Update()
	{
		auto player = Registry::Instance()->GetEntityByTag("player");
		for (auto entity : GetSystemEntities())
		{
			// If the Player is Dead --> Remove the Enemies from the screen
			if (entity.BelongsToGroup("enemies") && Game::Instance()->GetPlayerDead())
				entity.Kill();


			if (entity.HasTag("player"))
			{
				auto& health = entity.GetComponent<HealthComponent>();
				currentHealth = health.healthPercentage;
				maxHearts = health.maxHearts;

				// Start Low health sound timer
				if (currentHealth <= 2 && !lowHealth)
				{
					health.lowHeathTimer.Start();
					lowHealth = true;
				}
				// If the current health is equal or less than 2 and timer is greater, play sound, reset timer
				if (currentHealth <= 2 && currentHealth > 0 && health.lowHeathTimer.GetTicks() > 250)
				{
					Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "low_health", 0, 5);
					health.lowHeathTimer.Stop();
					lowHealth = false;
				}
			}

			// This is ugly? Better way?
			if (entity.HasTag("heart1") && currentHealth == 1)
			{
				auto& sprite = entity.GetComponent<SpriteComponent>();
				sprite.srcRect.x = 16;
			}

			if (entity.HasTag("heart1") && currentHealth == 0)
			{
				auto& sprite = entity.GetComponent<SpriteComponent>();
				sprite.srcRect.x = 32;
			}

			if (entity.HasTag("heart2") && currentHealth == 3)
			{
				auto& sprite = entity.GetComponent<SpriteComponent>();
				sprite.srcRect.x = 16;
			}
			if (entity.HasTag("heart2") && currentHealth == 2)
			{
				auto& sprite = entity.GetComponent<SpriteComponent>();
				sprite.srcRect.x = 32;
			}

			if (entity.HasTag("heart3") && currentHealth == 5)
			{
				auto& sprite = entity.GetComponent<SpriteComponent>();
				sprite.srcRect.x = 16;
			}
			if (entity.HasTag("heart3") && currentHealth == 4)
			{
				auto& sprite = entity.GetComponent<SpriteComponent>();
				sprite.srcRect.x = 32;
			}

			if (maxHearts >= 4 && (currentHealth >= 6 && currentHealth < 16))
			{
				if (entity.HasTag("heart4") && currentHealth == 7)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart4") && currentHealth == 6)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 5 && (currentHealth >= 8 && currentHealth < 16))
			{
				if (entity.HasTag("heart5") && currentHealth == 9)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart5") && currentHealth == 8)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 6 && (currentHealth >= 10 && currentHealth < 16))
			{
				if (entity.HasTag("heart6") && currentHealth == 11)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart6") && currentHealth == 10)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 7 && (currentHealth >= 12 && currentHealth < 16))
			{
				if (entity.HasTag("heart7") && currentHealth == 13)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart7") && currentHealth == 12)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 8 && (currentHealth >= 14 && currentHealth < 16 ))
			{
				if (entity.HasTag("heart8") && currentHealth == 15)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart8") && currentHealth == 14)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 9 && (currentHealth >= 16 && currentHealth < 18))
			{
				if (entity.HasTag("heart9") && currentHealth == 17)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart9") && currentHealth == 16)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 10 && (currentHealth >= 18 && currentHealth < 20))
			{
				if (entity.HasTag("heart10") && currentHealth == 19)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart10") && currentHealth == 18)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 11 && (currentHealth >= 20 && currentHealth < 22))
			{
				if (entity.HasTag("heart11") && currentHealth == 21)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart11") && currentHealth == 20)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 12 && (currentHealth >= 22 && currentHealth < 24))
			{
				if (entity.HasTag("heart12") && currentHealth == 23)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart12") && currentHealth == 22)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 13 && (currentHealth >= 24 && currentHealth < 26))
			{
				if (entity.HasTag("heart13") && currentHealth == 25)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart13") && currentHealth == 24)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 14 && (currentHealth >= 26 && currentHealth < 28))
			{
				if (entity.HasTag("heart14") && currentHealth == 27)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart14") && currentHealth == 26)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 15 && (currentHealth >= 28 && currentHealth < 30))
			{
				if (entity.HasTag("heart15") && currentHealth == 29)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart15") && currentHealth == 28)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
			if (maxHearts >= 16 && (currentHealth >= 30 && currentHealth < 32))
			{
				if (entity.HasTag("heart16") && currentHealth == 31)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 16;
				}
				if (entity.HasTag("heart16") && currentHealth == 30)
				{
					auto& sprite = entity.GetComponent<SpriteComponent>();
					sprite.srcRect.x = 32;
				}
			}
		}
	}

	void EmptyFunction();
};

