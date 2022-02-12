#include "HealthSystem.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Components/HUDComponent.h"
#include "../../Game/Game.h"
#include "../../Systems/SoundFXSystem.h"

unsigned int HealthSystem::currentHealth = 6;
unsigned int HealthSystem::maxHearts = 3;
unsigned int HealthSystem::numHearts = 3;
int HealthSystem::loadedHearts = 0;

HealthSystem::HealthSystem()
	: game(Game::Instance()), reg(Registry::Instance())
{
	RequiredComponent<HealthComponent>();
	RequiredComponent<SpriteComponent>();
}

void HealthSystem::Update()
{
	for (auto& entity : GetSystemEntities())
	{
		if (entity.HasTag("player"))
		{
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collider = entity.GetComponent<BoxColliderComponent>();
			auto& health = entity.GetComponent<HealthComponent>();

			if (health.addHeart)
			{
				loadedHearts += 1;
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
				}

				// Set the tag to the next heart!
				tag += std::to_string(numHearts);
				// Create a new player Heart
				Entity newHeart = Registry::Instance().CreateEntity();
				newHeart.AddComponent<SpriteComponent>("hud_hearts", 16, 16, 5, true, 0, 0);
				newHeart.AddComponent<TransformComponent>(glm::vec2(xPos, yPos), glm::vec2(4, 4), 0.0);
				newHeart.AddComponent<HealthComponent>(0);
				newHeart.AddComponent<HUDComponenet>();
				newHeart.Tag(tag);
				newHeart.Group("hud");

				health.maxHearts += 1;
				health.healthPercentage = health.maxHearts * 2;

			}
			// If the Player is Dead --> Remove the Enemies from the screen
			if (entity.BelongsToGroup("enemies") && game.GetPlayerDead())
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
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "low_health", 0, 5);
					health.lowHeathTimer.Stop();
					lowHealth = false;
				}
			}
			// Change the heart sprite based on the current health
			for (int i = 1; i <= maxHearts; i++)
			{
				std::string tag = "heart" + std::to_string(i);
				auto currentHeart = reg.GetEntityByTag(tag);
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
}