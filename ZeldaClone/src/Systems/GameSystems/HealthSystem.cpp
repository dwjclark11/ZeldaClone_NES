#include "Systems/GameSystems/HealthSystem.h"
#include "Components/HealthComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PlayerComponent.h"
#include "AssetManager/AssetManager.h"
#include "Components/HUDComponent.h"
#include "Game/Game.h"
#include "Systems/SoundFXSystem.h"
#include "Game/Player.h"
#include "Utilities/GameData.h"

HealthSystem::HealthSystem()
	: m_Game{ Game::Instance() }
	, m_Registry{ Registry::Instance() }
	, m_bLowHealth{ false }
{
	RequiredComponent<HealthComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<PlayerComponent>();
}

void HealthSystem::Update()
{
	auto& gameData = GameData::GetInstance();

	for ( auto& entity : GetSystemEntities() )
	{
		const auto& transform = entity.GetComponent<TransformComponent>();
		const auto& collider = entity.GetComponent<BoxColliderComponent>();
		auto& health = entity.GetComponent<HealthComponent>();

		if ( health.bAddHeart )
		{
			gameData.IncrementLoadedHearts();
			health.bAddHeart = false;
		}

		if ( gameData.GetLoadedHearts() > 3 )
		{
			gameData.IncrementNumHearts();
			gameData.DecrementLoadedHearts();
		}

		if ( health.maxHearts != gameData.GetNumHearts() && gameData.GetNumHearts() <= 16 )
		{
			int xPos = 0;
			int yPos = 0;
			std::string tag = "heart";

			if ( gameData.GetNumHearts() <= 8 )
			{
				xPos = 724 + ( gameData.GetNumHearts() - 3 ) * 32;
				yPos = 95;
			}
			else if ( gameData.GetNumHearts() > 8 && gameData.GetNumHearts() <= 16 )
			{
				xPos = 628 + ( gameData.GetNumHearts() - 8 ) * 32;
				yPos = 128;
			}

			// Set the tag to the next heart!
			tag += std::to_string( gameData.GetNumHearts() );
			// Create a new player Heart
			Entity newHeart = Registry::Instance().CreateEntity();
			newHeart.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hud_hearts",
																	 .width = 16,
																	 .height = 16,
																	 .layer = 5,
																	 .bIsFixed = true,
																	 .srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

			newHeart.AddComponent<TransformComponent>( glm::vec2( xPos, yPos ), glm::vec2( 4, 4 ), 0.0 );
			newHeart.AddComponent<HealthComponent>( 0 );
			newHeart.AddComponent<HUDComponenet>();
			newHeart.Tag( tag );
			newHeart.Group( "hud" );

			health.maxHearts += 1;
			health.healthPercentage = health.maxHearts * 2;
		}

		// Start Low health sound timer
		if ( health.healthPercentage <= 2 && !m_bLowHealth )
		{
			health.lowHeathTimer.Start();
			m_bLowHealth = true;
		}

		// If the current health is equal or less than 2 and timer is greater, play sound, reset timer
		if ( m_bLowHealth && health.lowHeathTimer.GetTicks() > 250 )
		{
			m_Game.GetSoundPlayer().PlaySoundFX( "low_health", 0, SoundChannel::ANY );
			health.lowHeathTimer.Stop();
			m_bLowHealth = false;
		}

		// Change the heart sprite based on the current health
		for ( int i = 1; i <= health.maxHearts; i++ )
		{
			std::string tag = "heart" + std::to_string( i );
			auto currentHeart = m_Registry.GetEntityByTag( tag );
			auto& sprite = currentHeart.GetComponent<SpriteComponent>();

			// Compare current Health
			if ( health.healthPercentage >= i * 2 )
				sprite.srcRect.x = 0;
			else if ( health.healthPercentage == i * 2 - 1 )
				sprite.srcRect.x = 16;
			else if ( health.healthPercentage <= i * 2 - 2 )
				sprite.srcRect.x = 32;
		}
	}
}