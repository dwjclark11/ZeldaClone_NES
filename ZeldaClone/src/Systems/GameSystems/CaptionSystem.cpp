#include "CaptionSystem.h"
#include "../../Components/SpriteComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Components/CaptionComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/TextLabelComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../Game/Game.h"

#include <SDL.h>

int CaptionSystem::NextLine(std::string& str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (i > 18)
			if (str[i] == ' ')
			{
				return i;
			}
				
	}
}

CaptionSystem::CaptionSystem()
{
	RequiredComponent<CaptionComponent>();
}

void CaptionSystem::AddCaption(const Entity& entity, int& spaceX, int& spaceY, char letter)
{
	auto& caption = entity.GetComponent<CaptionComponent>();
	std::string newLetterStr(1, letter);
	Entity newCaption = Registry::Instance().CreateEntity();
	newCaption.Group("caption");
	// Does the std::move make sense hear?
	newCaption.AddComponent<TextLabelComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), std::move(newLetterStr), "game_font", SDL_Color{ 255,255,255,255 }, false);
	newCaption.AddComponent<GameComponent>();
}

void CaptionSystem::Update(const float& dt)
{
	for (const auto& entity : GetSystemEntities())
	{
		auto& caption = entity.GetComponent<CaptionComponent>();
		int xPos = caption.xPos / 1024;
		int yPos = caption.yPos / 672;
		
		const auto& playerPos = Game::Instance().GetPlayerPos();

		// Check to see if the caption has already Finished
		if (caption.finished)
			continue;
				
		if (xPos == playerPos.x && yPos == playerPos.y)
		{
			if (caption.scrollable)
			{
				if (!caption.started)
				{
					caption.started = true;
					caption.scrollTimer.Start();
				}

				// Scoll is based on time
				if (caption.scrollTimer.GetTicks() >= 5000 * dt * caption.currentFrame)
				{
					int spaceX = caption.currentFrame * LETTER_SPACE;   
					int spaceY = 0;

					// Space changes also based on currentFrame amount since the row changes
					char letter = ' ';

					if (caption.caption[caption.currentFrame] != ' ')
						letter = std::toupper(caption.caption[caption.currentFrame]);

					int nextLine = NextLine(caption.caption);

					if (caption.currentFrame > nextLine)
					{
						// This starts it back at zero for the next row
						spaceX = (caption.currentFrame - nextLine) * LETTER_SPACE; 
						spaceY = LETTER_SPACE; 
					}

					AddCaption(entity, spaceX, spaceY, letter);

					// increase the frame index
					caption.currentFrame++;

					// play the text scroll sound
					Mix_Volume(6, 10);
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance().GetAssetManager(), "text_slow", 1, 6);

					// Check completion
					if (caption.currentFrame == caption.caption.length())
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
				if (!caption.started)
				{
					Entity newCaption = Registry::Instance().CreateEntity();
					newCaption.Group("caption");

					// Does the std::move make sense here?
					newCaption.AddComponent<TextLabelComponent>(glm::vec2(caption.xPos, caption.yPos), caption.caption, "game_font", SDL_Color{ 255,255,255,255 }, false);
					newCaption.AddComponent<GameComponent>();
					caption.started = true;
				}
			}
		}
	}
}

