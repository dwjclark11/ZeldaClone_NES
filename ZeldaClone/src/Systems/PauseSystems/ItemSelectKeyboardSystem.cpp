#include "ItemSelectKeyboardSystem.h"
#include "../GameSystems/GamePadSystem.h"

ItemType ItemSelectKeyboardControlSystem::itemType = EMPTY;

void ItemSelectKeyboardControlSystem::Update()
{
	for (const auto& entity : GetSystemEntities())
	{
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();

		if (entity.HasTag("pauseSelector"))
		{
			if (GamePadSystem::upPressed && !GamePadSystem::buttonDirDown)
			{
				transform.position.y -= ((sprite.height * transform.scale.y) + 6);
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y < 190) transform.position.y = 260;
				GamePadSystem::buttonDirDown = true;
			}

			if (GamePadSystem::downPressed && !GamePadSystem::buttonDirDown)
			{
				transform.position.y += ((sprite.height * transform.scale.y) + 6);
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y > 260) transform.position.y = 190;
				GamePadSystem::buttonDirDown = true;
			}

			if (GamePadSystem::rightPressed && !GamePadSystem::buttonDirDown)
			{
				transform.position.x += 100;
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.x > 686) transform.position.x = 386;
				GamePadSystem::buttonDirDown = true;
			}

			if (GamePadSystem::leftPressed && !GamePadSystem::buttonDirDown)
			{
				transform.position.x -= 100;
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.x < 386) transform.position.x = 686;
				GamePadSystem::buttonDirDown = true;
			}

			if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
			{
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
