#include "Systems/GameSystems/CaptionSystem.h"
#include "Components/SpriteComponent.h"
#include "Logger/Logger.h"
#include "AssetManager/AssetManager.h"
#include "Components/CaptionComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/GameComponent.h"
#include "Systems/SoundFXSystem.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/GameData.h"
#include "Utilities/Utility.h"

#include <SDL.h>

constexpr int LETTER_SPACE = 32;

// This is a hack, I have made an improved version that works much better
int CaptionSystem::NextLine( std::string& str )
{
	for ( int i = 0; i < str.length(); i++ )
	{
		if ( i > 18 )
		{
			if ( str[ i ] == ' ' )
			{
				return i;
			}
		}
	}
	return -1;
}

CaptionSystem::CaptionSystem()
{
	RequiredComponent<CaptionComponent>();
}

void CaptionSystem::AddCaption( const Entity& entity, int& spaceX, int& spaceY, char letter )
{
	auto& caption = entity.GetComponent<CaptionComponent>();
	std::string newLetterStr{ letter };
	Entity newCaption = Registry::Instance().CreateEntity();
	newCaption.Group( "caption" );
	// Does the std::move make sense hear?
	newCaption.AddComponent<TextLabelComponent>( glm::vec2( caption.xPos + spaceX, caption.yPos + spaceY ),
												 newLetterStr,
												 "game_font",
												 SDL_Color{ 255, 255, 255, 255 },
												 false );
	newCaption.AddComponent<GameComponent>();
}

void CaptionSystem::Update( const float& dt )
{
	bool add_space{ false };
	int nextLine = -1;

	for ( const auto& entity : GetSystemEntities() )
	{
		auto& caption = entity.GetComponent<CaptionComponent>();
		int xPos = caption.xPos / PANEL_HEIGHT;
		int yPos = caption.yPos / PANEL_WIDTH;

		const auto& playerPos = Game::Instance().GetPlayer()->GetPlayerPos();

		// Check to see if the caption has already Finished
		if ( caption.finished )
			continue;

		if ( xPos == playerPos.x && yPos == playerPos.y )
		{
			if ( caption.scrollable )
			{
				if ( nextLine == -1 )
				{
					nextLine = NextLine( caption.caption );
				}

				if ( !caption.started )
				{
					caption.started = true;
					caption.scrollTimer.Start();
				}

				// Scoll is based on time
				if ( caption.scrollTimer.GetTicks() >= 5000 * dt * caption.currentFrame )
				{
					int spaceX = caption.currentFrame * LETTER_SPACE;
					int spaceY = 0;

					// Space changes also based on currentFrame amount since the row changes
					char letter = ' ';

					if ( caption.caption[ caption.currentFrame ] != ' ' )
						letter = std::toupper( caption.caption[ caption.currentFrame ] );
					if ( letter == ' ' )
					{
						add_space = true;
						++caption.currentFrame;
						continue;
					}

					if ( caption.currentFrame > nextLine )
					{
						// This starts it back at zero for the next row
						spaceX = ( caption.currentFrame - nextLine ) * LETTER_SPACE;
						spaceY = LETTER_SPACE;
					}

					// AddCaption(entity, spaceX, spaceY, letter);
					ConvertLetter( letter, caption.xPos + spaceX + ( add_space ? 24 : 0 ), caption.yPos + spaceY );
					// increase the frame index
					caption.currentFrame++;

					// play the text scroll sound
					Mix_Volume( 6, 10 );
					Game::Instance().GetSoundPlayer().PlaySoundFX( "text_slow", 1, SoundChannel::TEXT );

					// Check completion
					if ( caption.currentFrame == caption.caption.length() )
					{
						caption.finished = true;
						caption.scrollTimer.Stop();
						continue;
					}
				}
			}
			else // This is for numbers or anything that does not require scrolling
			{
				auto& caption = entity.GetComponent<CaptionComponent>();
				if ( !caption.started )
				{
					if ( caption.is_number )
					{
						ConvertNumber( caption.caption, caption.xPos, caption.yPos );
					}
					else
					{
						ConvertName( caption.caption, caption.xPos, caption.yPos );
					}

					caption.started = true;
				}
			}
		}
	}
}
