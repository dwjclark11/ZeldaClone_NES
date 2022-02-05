#pragma once
#include "../../ECS/ECS.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/TextLabelComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Systems/SoundFXSystem.h"
#include "../../States/GameState.h"
#include "../../Game/LevelLoader.h"
#include "../../Game/Game.h"
#include <string>
#include <SDL.h>

enum ItemType
{
	EMPTY = 0,
	BOOMERANG,
	BOMB,
	POTION_BLUE,
	POTION_RED,
	CANDLE,
	MAGIC_CANDLE,
	WOOD_BOW,
	MAGIC_BOW,
	FOOD,
	WHISTLE,
	MAGIC_ROD
};


class ItemSelectKeyboardControlSystem : public System
{
private:
	class Game& game;
public:

	unsigned int slot;
	static ItemType itemType;

	ItemSelectKeyboardControlSystem()
		: game(Game::Instance())
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();

		this->slot = 0;
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &ItemSelectKeyboardControlSystem::OnKeyPressed); // Callback Function
	}

	// Callback function
	void OnKeyPressed(KeyPressedEvent& event)
	{
		for (const auto& entity : GetSystemEntities())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			if (entity.HasTag("pauseSelector"))
			{
				switch (event.symbol)
				{
				case SDLK_UP:
					transform.position.y -= ((sprite.height * transform.scale.y) + 6);
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 190) transform.position.y = 260;
					break;

				case SDLK_DOWN:
					transform.position.y += ((sprite.height * transform.scale.y) + 6);
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 260) transform.position.y = 190;
					break;

				case SDLK_RIGHT:
					transform.position.x += 100;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.x > 686) transform.position.x = 386;
					break;

				case SDLK_LEFT:
					transform.position.x -= 100;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.x < 386) transform.position.x = 686;

					break;

				case SDLK_SPACE:

					if (transform.position.x == 386 && transform.position.y == 190 && game.GetGameItems().woodBoomerang)
					{
						itemType = BOOMERANG;
					}
					else if ((transform.position.x == 486 && transform.position.y == 190) && GameState::totalBombs > 0 && game.GetGameItems().bombs)
					{
						itemType = BOMB;
					}
					else if (transform.position.x == 586 && transform.position.y == 190 && game.GetGameItems().bow)
					{
						itemType = WOOD_BOW;
					}
					else if (transform.position.x == 686 && transform.position.y == 190 && game.GetGameItems().candle)
					{
						itemType = CANDLE;
					}
					else if (transform.position.x == 386 && transform.position.y == 260)
					{
						
					}
					else if (transform.position.x == 486 && transform.position.y == 260 && game.GetGameItems().food)
					{
						itemType = FOOD;
					}
					else if (transform.position.x == 586 && transform.position.y == 260 && game.GetGameItems().redPotion)
					{
						itemType = POTION_RED;
					}
					else if (transform.position.x == 686 && transform.position.y == 260 && game.GetGameItems().magicRod)
					{
						itemType = MAGIC_ROD;
					}
					break;
				}
			}

			if (entity.HasTag("selectedItem") || entity.HasTag("hudItem"))
			{
				switch (itemType)
				{
				case BOOMERANG:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 0;
					sprite.srcRect.y += sprite.height * 7;
					break;
				case BOMB:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 4;
					sprite.srcRect.y += sprite.height * 7;
					break;

				case WOOD_BOW:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 4;
					break;

				case CANDLE:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.y += sprite.height * 3;
					break;

				case FOOD:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 7;
					sprite.srcRect.y += sprite.height * 1;
					break;

				case POTION_RED:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 2;
					sprite.srcRect.y += sprite.height * 2;
					break;
					
				case MAGIC_ROD:
					sprite.srcRect.x = 0;
					sprite.srcRect.y = 0;
					sprite.srcRect.x += sprite.width * 4;
					sprite.srcRect.y += sprite.height * 5;
					break;
				}
			}
		}
	}

	void Update();


};

