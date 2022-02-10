#include "ItemSelectKeyboardSystem.h"
#include "../GameSystems/GamePadSystem.h"

void ItemSelectKeyboardControlSystem::OnKeyPressed(KeyPressedEvent& event)
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
					Game::mSelectedItem = Game::ItemType::BOOMERANG;
				}
				else if ((transform.position.x == 486 && transform.position.y == 190) && GameState::totalBombs > 0 && game.GetGameItems().bombs)
				{
					Game::mSelectedItem = Game::ItemType::BOMB;
				}
				else if (transform.position.x == 586 && transform.position.y == 190 && game.GetGameItems().bow)
				{
					Game::mSelectedItem = Game::ItemType::WOOD_BOW;
				}
				else if (transform.position.x == 686 && transform.position.y == 190 && game.GetGameItems().candle)
				{
					Game::mSelectedItem = Game::ItemType::CANDLE;
				}
				else if (transform.position.x == 386 && transform.position.y == 260)
				{

				}
				else if (transform.position.x == 486 && transform.position.y == 260 && game.GetGameItems().food)
				{
					Game::mSelectedItem = Game::ItemType::FOOD;
				}
				else if (transform.position.x == 586 && transform.position.y == 260 && game.GetGameItems().redPotion)
				{
					Game::mSelectedItem = Game::ItemType::POTION_RED;
				}
				else if (transform.position.x == 686 && transform.position.y == 260 && game.GetGameItems().magicRod)
				{
					Game::mSelectedItem = Game::ItemType::MAGIC_ROD;
				}
				break;
			}
		}

		if (entity.HasTag("selectedItem") || entity.HasTag("hudItem"))
		{
			switch (Game::mSelectedItem)
			{
			case Game::ItemType::BOOMERANG:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 0;
				sprite.srcRect.y += sprite.height * 7;
				break;
			case Game::ItemType::BOMB:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 4;
				sprite.srcRect.y += sprite.height * 7;
				break;

			case Game::ItemType::WOOD_BOW:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 4;
				break;

			case Game::ItemType::CANDLE:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.y += sprite.height * 3;
				break;

			case Game::ItemType::FOOD:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 7;
				sprite.srcRect.y += sprite.height * 1;
				break;

			case Game::ItemType::POTION_RED:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 2;
				sprite.srcRect.y += sprite.height * 2;
				break;

			case Game::ItemType::MAGIC_ROD:
				sprite.srcRect.x = 0;
				sprite.srcRect.y = 0;
				sprite.srcRect.x += sprite.width * 4;
				sprite.srcRect.y += sprite.height * 5;
				break;
			}
		}
	}
}