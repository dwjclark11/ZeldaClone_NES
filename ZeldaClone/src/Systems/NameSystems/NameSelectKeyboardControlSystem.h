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
#include "../../States/EditorState.h"
#include "../../States/NameState.h"
#include "../../States/MenuState.h"
#include "../../Game/LevelLoader.h"
#include "../../Game/Game.h"
#include "../../Systems/SoundFXSystem.h"
#include <string>
#include <SDL.h>


class NameSelectKeyboardControlSystem : public System
{
private:
	class Game& game;
	int row;
	int col;

public:

	unsigned int slot;
	
	NameSelectKeyboardControlSystem()
		: game(*Game::Instance())
		, row(0)
		, col(0)
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();
		this->slot = 0;

	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &NameSelectKeyboardControlSystem::OnKeyPressed); // Callback Function
	}

	// Callback function
	void OnKeyPressed(KeyPressedEvent& event)
	{
		for (auto entity : GetSystemEntities())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& text = entity.GetComponent<TextLabelComponent>();

			if (entity.HasTag("box"))
			{
				switch (event.symbol)
				{
				case SDLK_UP:
					transform.position.y -= sprite.height * transform.scale.y * 2;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					row--;
					if (transform.position.y < 200)
					{
						transform.position.y = 584;
						row = 3;
					}
					Logger::Log("row: " + std::to_string(row));
					break;

				case SDLK_DOWN:
					transform.position.y += sprite.height * transform.scale.y * 2;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					row++;
					if (transform.position.y > 584)
					{
						transform.position.y = 200;
						row = 0;
					}
					Logger::Log("row: " + std::to_string(row));
					break;

				case SDLK_RIGHT:
					transform.position.x += sprite.width * transform.scale.x;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					col++;
					if (transform.position.x > 708)
					{
						transform.position.x = 260;
						col = 0;
					}
					Logger::Log("col: " + std::to_string(col));
					break;

				case SDLK_LEFT:
					transform.position.x -= sprite.width * transform.scale.x;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					col--;
					if (transform.position.x < 260)
					{
						transform.position.x = 708;
						col = 7;
					}
					Logger::Log("col: " + std::to_string(col));
					break;
				default:
					break;
				}

				switch (event.symbol)
				{

				case SDLK_SPACE:
	
					if (text.text.size() < 6)
					{
						bool valid = true;
						// Make sure that the row and col have a valid letter based on the 
						// name-letters sprite
						if (row == 3 && col > 1)
						{
							valid = false;
						}

						// Create a new char based on ASCII Upper characters and the letter position
						// on the sprite
						char newChar = (row * 8) + col + 65;

						if (valid)
							text.text += newChar;

					}
					if (transform.position.x == 452 && transform.position.y == 584)
					{
						if (text.text.size() > 0)
							text.text.erase(text.text.size() - 1);
					}
					if (transform.position.x == 580 && transform.position.y == 584)
					{
						LevelLoader loader;
						NameState::name = text.text.c_str();
						
						// Reset the column and the row after name is entered
						row = 0;
						col = 0;
						loader.SavePlayerNameToLuaTable(std::to_string(slot), NameState::name);
						game.GetStateMachine()->PopState();
						game.GetStateMachine()->PushState(new MenuState());
					}

					break;
				default:
					break;
				}
			}
		}	
	}

	void Update()
	{
		if (MenuState::player1Name.size() == 0)
			slot = 1;
		else if (MenuState::player2Name.size() == 0)
			slot = 2;
		else if (MenuState::player3Name.size() == 0)
			slot = 3;
	}
};