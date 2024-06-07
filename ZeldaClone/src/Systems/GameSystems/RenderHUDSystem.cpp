#include "Systems/GameSystems/RenderHUDSystem.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Components/HUDComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "AssetManager/AssetManager.h"
#include "Utilities/GameData.h"
#include "Utilities/Camera.h"
#include "States/State.h"
#include "States/GameStateMachine.h"

#include <SDL.h>
#include <algorithm>
#include <vector>

constexpr int OFFSET = 720;

RenderHUDSystem::RenderHUDSystem()
	: m_Game( Game::Instance() )
	, m_GameData( GameData::GetInstance() )
	, m_bPause{ false }
	, m_bGamePlay{ false }
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<HUDComponenet>();
}

void RenderHUDSystem::Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager )
{
	// Loop all entities that the system is interested in
	for ( const auto& entity : GetSystemEntities() )
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& sprite = entity.GetComponent<SpriteComponent>();

		if ( !m_GameData.HasItem( GameData::GameItems::WOOD_SWORD ) && entity.HasTag( "hudSword" ) )
		{
			continue;
		}
		else if ( m_GameData.HasItem( GameData::GameItems::WOOD_SWORD ) && entity.HasTag( "hudSword" ) )
		{
			if ( m_GameData.HasItem( GameData::GameItems::MAGIC_SWORD ) )
			{
				sprite.srcRect.x = sprite.width * 2;
				sprite.srcRect.y = sprite.height * 0;
			}
			else if ( m_GameData.HasItem( GameData::GameItems::SILVER_SWORD ) )
			{
				sprite.srcRect.x = sprite.width * 1;
				sprite.srcRect.y = sprite.height * 0;
			}
		}

		// Set the src Rect of our original sprite texture
		SDL_Rect srcRect = sprite.srcRect;

		if ( m_Game.GetStateMachine()->GetCurrentState() == "PAUSE" && !m_bPause )
		{
			m_bGamePlay = false;
			transform.position.y += OFFSET;
		}
		else if ( m_Game.GetStateMachine()->GetCurrentState() == "GAMESTATE" ||
				  m_Game.GetStateMachine()->GetCurrentState() == "DUNGEON" && !m_bGamePlay )
		{
			// If we were previously in the m_bPause state, subract the offset from all HUD entities
			if ( m_bPause )
			{
				transform.position.y -= OFFSET;
			}
		}

		if ( entity.HasTag( "hudItem" ) )
		{
			switch ( m_GameData.GetSelectedItem() )
			{
			case GameData::ItemType::BOOMERANG:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 0;
				sprite.srcRect.y += sprite.height * 7;

				break;
			case GameData::ItemType::BOMB:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 4;
				sprite.srcRect.y += sprite.height * 7;
				break;

			case GameData::ItemType::BOW:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 4;
				break;

			case GameData::ItemType::CANDLE:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.y += sprite.height * 3;
				break;

			case GameData::ItemType::FOOD:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 7;
				sprite.srcRect.y += sprite.height * 1;
				break;

			case GameData::ItemType::POTION_RED:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 2;
				sprite.srcRect.y += sprite.height * 2;
				break;

			case GameData::ItemType::MAGIC_ROD:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 4;
				sprite.srcRect.y += sprite.height * 5;
				break;
			}
		}

		// Change the position of the locator to match the player position index
		// See MovementSystem --> Update()
		if ( entity.HasTag( "locator" ) )
		{
			transform.position.x = 10 + ( m_Game.GetPlayer()->GetPlayerPos().x * 16 );
			transform.position.y = 40 + ( m_Game.GetPlayer()->GetPlayerPos().y * 16 );
		}

		// Set the destination rect with the x, y position to be rendered
		SDL_Rect dstRect = { ( transform.position.x ),
							 ( transform.position.y ),
							 ( sprite.width * transform.scale.x ),
							 ( sprite.height * transform.scale.y ) };

		SDL_Texture* tex = assetManager->GetTexture( sprite.assetID );
		SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
		SDL_SetTextureAlphaMod( tex, Game::Instance().GetCamera().GetFadeAlpha() );

		SDL_RenderCopyEx( renderer,
						  // assetManager->GetTexture(sprite.assetID),
						  tex,
						  &srcRect,
						  &dstRect,
						  transform.rotation,
						  NULL,		  // The rotation is done on the center of the sprite, width / 2, height / 2
						  sprite.flip // This is if we want to flip a sprite
		);
	}
	// Reset the HUD states
	if ( m_Game.GetStateMachine()->GetCurrentState() == "PAUSE" )
	{
		m_bPause = true;
		m_bGamePlay = false;
	}

	if ( m_Game.GetStateMachine()->GetCurrentState() == "GAMESTATE" ||
		 m_Game.GetStateMachine()->GetCurrentState() == "DUNGEON" )
	{
		m_bGamePlay = true;
		m_bPause = false;
	}
}

void RenderHUDSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}
