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
#include <string>
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

Entity& CaptionSystem::AddCaption(const Entity& entity, int& spaceX, int& spaceY)
{
	auto& caption = entity.GetComponent<CaptionComponent>();

	Entity newCaption = Registry::Instance().CreateEntity();
	newCaption.Group("caption");
	newCaption.AddComponent<TransformComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), glm::vec2(2, 2), 0);
	newCaption.AddComponent<SpriteComponent>("caption_letters", 16, 16, 5, false, 0, 0);
	newCaption.AddComponent<GameComponent>();
	
	return newCaption;
}

void CaptionSystem::AddCaption(const Entity& entity, int& spaceX, int& spaceY, char letter)
{
	auto& caption = entity.GetComponent<CaptionComponent>();
	std::string newLetterStr(&letter);
	Entity newCaption = Registry::Instance().CreateEntity();
	newCaption.Group("caption");
	newCaption.AddComponent<TextLabelComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), newLetterStr, "game_font", SDL_Color{ 255,255,255,255 }, false);
	newCaption.AddComponent<GameComponent>();
}

Entity& CaptionSystem::AddNumberCaption(const Entity& entity, int spaceX, int spaceY)
{
	auto& caption = entity.GetComponent<CaptionComponent>();

	Entity newCaption = Registry::Instance().CreateEntity();
	newCaption.Group("caption");
	newCaption.AddComponent<TransformComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), glm::vec2(4, 4), 0);
	newCaption.AddComponent<SpriteComponent>("numbers", 8, 8, 5, false, 0, 0);
	newCaption.AddComponent<GameComponent>();
	return newCaption;
}

void CaptionSystem::Update()
{
	for (const auto& entity : GetSystemEntities())
	{
		auto& caption = entity.GetComponent<CaptionComponent>();
		int xPos = caption.xPos / 1024;
		int yPos = caption.yPos / 672;

		// Check to see if the caption has already Finished
		if (caption.finished)
			continue;
				
		if (xPos == Game::Instance().GetPlayerPos().x && yPos == Game::Instance().GetPlayerPos().y)
		{
			if (caption.scrollable)
			{
				if (!caption.started)
				{
					caption.started = true;
					caption.scrollTimer.Start();
				}

				// Scoll is based on time
				if (caption.scrollTimer.GetTicks() >= 100 * caption.currentFrame)
				{
					int spaceX = caption.currentFrame * LETTER_SPACE; // Change this to not a magic number!   
					int spaceY = 0;
					// Space changes also based on currentFrame amount since the row changes

					char letter = ' ';

					if (caption.caption[caption.currentFrame] != ' ')
						letter = std::toupper(caption.caption[caption.currentFrame]);

					int nextLine = NextLine(caption.caption);
					if (caption.currentFrame > nextLine)
					{
						// This starts it back at zero for the next row
						spaceX = (caption.currentFrame - nextLine) * LETTER_SPACE; // * spriteWidth;
						spaceY = LETTER_SPACE; // Change this maybe to row * spriteHeight;
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
			else if (caption.numbers)
			{
				int spaceX = 0; // Change this to not a magic number!   
				int spaceY = 0;

				if (caption.hundreds > 0)
				{
					auto newCaption = AddNumberCaption(entity, spaceX, spaceY);

					spaceX = LETTER_SPACE;
					if (caption.hundreds >= 0 && caption.hundreds <= 9)
					{
						newCaption.GetComponent<SpriteComponent>().srcRect.x = 8 * caption.hundreds;
						newCaption.GetComponent<SpriteComponent>().srcRect.y = 0;
					}
						
					caption.currentFrame++;
					spaceX *= caption.currentFrame;
				}
						
				if (caption.tens >= 0)
				{
					auto newCaption = AddNumberCaption(entity, spaceX, spaceY);
					spaceX = LETTER_SPACE;
					if (caption.tens >= 0 && caption.tens <= 9)
					{
						newCaption.GetComponent<SpriteComponent>().srcRect.x = 8 * caption.tens;
						newCaption.GetComponent<SpriteComponent>().srcRect.y = 0;
					}
					caption.currentFrame++;
					spaceX *= caption.currentFrame;
				}

				if (caption.ones >= 0)
				{
					auto newCaption = AddNumberCaption(entity, spaceX, spaceY);
					spaceX = LETTER_SPACE;
					if (caption.ones >= 0 && caption.ones <= 9)
					{
						newCaption.GetComponent<SpriteComponent>().srcRect.x = 8 * caption.ones;
						newCaption.GetComponent<SpriteComponent>().srcRect.y = 0;
					}

					caption.finished = true;
				}
			}
			else 
			{
				int spaceX = 0; // Change this to not a magic number!   
				int spaceY = 0;

				if (caption.currentFrame > 22)
				{
					// This starts it back at zero for the next row
					spaceX = (caption.currentFrame - 19) * LETTER_SPACE; // * spriteWidth;
					spaceY = 32; // Change this maybe to row * spriteHeight;
				}
				auto newCaption = AddCaption(entity, spaceX, spaceY);
				spaceX = LETTER_SPACE;
						
				char letter = ' ';

				if (caption.caption[caption.currentFrame] != ' ')
					letter = std::toupper(caption.caption[caption.currentFrame]);
				else
				{
					newCaption.GetComponent<SpriteComponent>().srcRect.x = 16 * 26;
					newCaption.GetComponent<SpriteComponent>().srcRect.y = 0;
				}
				// Check to see if the letters are part of the alphabet       
				if (letter >= 65 && letter <= 90)
				{
					newCaption.GetComponent<SpriteComponent>().srcRect.x = 16 * (letter - 65);
					newCaption.GetComponent<SpriteComponent>().srcRect.y = 0;
				}

				// Check to see if the letters are punctuation marks
				if (letter >= 33 && letter <= 47)
				{
					newCaption.GetComponent<SpriteComponent>().srcRect.x = 16 * (letter - 33);
					newCaption.GetComponent<SpriteComponent>().srcRect.y = 16;
				}
				// increase the frame index
				caption.currentFrame++;
				spaceX *= caption.currentFrame;

				// Check completion
				if (caption.currentFrame == caption.caption.length())
				{
					caption.finished = true;
					continue;
				}
			}
		}
	}
}

