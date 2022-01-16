#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/SpriteComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Components/CaptionComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/GameComponent.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../Game/Game.h"

#include <SDL.h>

class CaptionSystem : public System
{
private:
	
public:
	CaptionSystem()
	{
		RequiredComponent<CaptionComponent>();
	}

	void Update()
	{
		for (auto& entity : GetSystemEntities())
		{
			auto& caption = entity.GetComponent<CaptionComponent>();
			int xPos = caption.xPos / 1024;
			int yPos = caption.yPos / 672;

			if (xPos == Game::Instance()->GetPlayerPos().x && yPos == Game::Instance()->GetPlayerPos().y)
			{
				// Check to see if the caption has already Finished
				if (caption.finished)
					continue;
				if (caption.scrollable /*&& caption.caption != "no_caption"*/)
				{
					if (!caption.started)
					{
						caption.started = true;
						caption.scrollTimer.Start();
					}

					// Scoll is based on time
					if (caption.scrollTimer.GetTicks() >= 200 * caption.currentFrame)
					{
						int spaceX = caption.currentFrame * 32; // Change this to not a magic number!   
						int spaceY = 0;
						// Space changes also based on currentFrame amount since the row changes
						if (caption.currentFrame > 20)
						{
							// This starts it back at zero for the next row
							spaceX = (caption.currentFrame - 19) * 32; // * spriteWidth;
							spaceY = 32; // Change this maybe to row * spriteHeight;
						}
						Entity newCaption = Registry::Instance()->CreateEntity();
						newCaption.Group("caption");
						newCaption.AddComponent<TransformComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), glm::vec2(2, 2), 0);
						newCaption.AddComponent<SpriteComponent>("caption_letters", 16, 16, 5, false, 0, 0);
						newCaption.AddComponent<GameComponent>();

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

						// play the text scroll sound
						Mix_Volume(6, 10);
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 1, 6);

						// Check completion
						if (caption.currentFrame == caption.numFrames)
						{
							caption.finished == true;
							caption.scrollTimer.Stop();
							continue;
						}
					}
				}
				else
				{
					int spaceX = 0; // Change this to not a magic number!   
					int spaceY = 0;
					Logger::Log("ones: " + std::to_string(caption.ones));
					Logger::Log("tens: " + std::to_string(caption.tens));
					Logger::Log("hundy: " + std::to_string(caption.hundreds));
					if (caption.hundreds > 0)
					{
						Entity newCaption = Registry::Instance()->CreateEntity();
						newCaption.Group("caption");
						newCaption.AddComponent<TransformComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), glm::vec2(4, 4), 0);
						newCaption.AddComponent<SpriteComponent>("numbers", 8, 8, 5, false, 0, 0);
						newCaption.AddComponent<GameComponent>();
						spaceX = 32;
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
						Entity newCaption = Registry::Instance()->CreateEntity();
						newCaption.Group("caption");
						newCaption.AddComponent<TransformComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), glm::vec2(4, 4), 0);
						newCaption.AddComponent<SpriteComponent>("numbers", 8, 8, 5, false, 0, 0);
						newCaption.AddComponent<GameComponent>();
						spaceX = 32;
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
						Entity newCaption = Registry::Instance()->CreateEntity();
						newCaption.Group("caption");
						newCaption.AddComponent<TransformComponent>(glm::vec2(caption.xPos + spaceX, caption.yPos + spaceY), glm::vec2(4, 4), 0);
						newCaption.AddComponent<SpriteComponent>("numbers", 8, 8, 5, false, 0, 0);
						newCaption.AddComponent<GameComponent>();
						spaceX = 32;
						if (caption.ones >= 0 && caption.ones <= 9)
						{
							newCaption.GetComponent<SpriteComponent>().srcRect.x = 8 * caption.ones;
							newCaption.GetComponent<SpriteComponent>().srcRect.y = 0;
						}
						caption.currentFrame++;
						spaceX *= caption.currentFrame;
						caption.finished = true;
					}
				}
			}
		}
	}
};