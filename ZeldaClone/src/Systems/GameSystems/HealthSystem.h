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
			// Change the heart sprite based on the current health
			for (int i = 1; i <= maxHearts; i++)
			{
				std::string tag = "heart" + std::to_string(i);
				auto currentHeart = Registry::Instance()->GetEntityByTag(tag);
				auto& sprite = currentHeart.GetComponent<SpriteComponent>();
				
				// Compare current Health
				if (currentHealth >= i * 2)
					sprite.srcRect.x = 0;
				else if (currentHealth == i * 2 - 1)
					sprite.srcRect.x = 16;
				else if (currentHealth <= i * 2 - 2)
					sprite.srcRect.x = 32;
			}
		} 
	}
	void EmptyFunction();
};