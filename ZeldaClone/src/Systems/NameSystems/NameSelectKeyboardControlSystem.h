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
public:

	unsigned int slot;

	NameSelectKeyboardControlSystem()
		: game(*Game::Instance())
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
					if (transform.position.y < 200) transform.position.y = 584;
					break;

				case SDLK_DOWN:
					transform.position.y += sprite.height * transform.scale.y * 2;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 584) transform.position.y = 200;
					break;

				case SDLK_RIGHT:
					transform.position.x += sprite.width * transform.scale.x;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.x > 708) transform.position.x = 260;
					break;

				case SDLK_LEFT:
					transform.position.x -= sprite.width * transform.scale.x;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.x < 260) transform.position.x = 708;

					break;
				default:
					break;
				}

				switch (event.symbol)
				{

				case SDLK_SPACE:
	
					if (text.text.size() < 6)
					{
						if (transform.position.x == 260 && transform.position.y == 200)
						{
							char a = 'A';
							text.text += a;
						}
						else if (transform.position.x == 324 && transform.position.y == 200)
						{
							char b = 'B';
							text.text += b;
						}
						else if (transform.position.x == 388 && transform.position.y == 200)
						{
							char c = 'C';
							text.text += c;
						}
						else if (transform.position.x == 452 && transform.position.y == 200)
						{
							char d = 'D';
							text.text += d;
						}
						else if (transform.position.x == 516 && transform.position.y == 200)
						{
							char e = 'E';
							text.text += e;
						}
						else if (transform.position.x == 580 && transform.position.y == 200)
						{
							char f = 'F';
							text.text += f;
						}
						else if (transform.position.x == 644 && transform.position.y == 200)
						{
							char g = 'G';
							text.text += g;
						}
						else if (transform.position.x == 708 && transform.position.y == 200)
						{
							char h = 'H';
							text.text += h;
						}
						else if (transform.position.x == 260 && transform.position.y == 328)
						{
							char i = 'I';
							text.text += i;
						}
						else if (transform.position.x == 324 && transform.position.y == 328)
						{
							char j = 'J';
							text.text += j;
						}
						else if (transform.position.x == 388 && transform.position.y == 328)
						{
							char k = 'K';
							text.text += k;
						}
						else if (transform.position.x == 452 && transform.position.y == 328)
						{
							char l = 'L';
							text.text += l;
						}
						else if (transform.position.x == 516 && transform.position.y == 328)
						{
							char m = 'M';
							text.text += m;
						}
						else if (transform.position.x == 580 && transform.position.y == 328)
						{
							char n = 'N';
							text.text += n;
						}
						else if (transform.position.x == 644 && transform.position.y == 328)
						{
							char o = 'O';
							text.text += o;
						}
						else if (transform.position.x == 708 && transform.position.y == 328)
						{
							char p = 'P';
							text.text += p;
						}
						else if (transform.position.x == 260 && transform.position.y == 456)
						{
							char q = 'Q';
							text.text += q;
						}
						else if (transform.position.x == 324 && transform.position.y == 456)
						{
							char r = 'R';
							text.text += r;
						}
						else if (transform.position.x == 388 && transform.position.y == 456)
						{
							char s = 'S';
							text.text += s;
						}
						else if (transform.position.x == 452 && transform.position.y == 456)
						{
							char t = 'T';
							text.text += t;
						}
						else if (transform.position.x == 516 && transform.position.y == 456)
						{
							char u = 'U';
							text.text += u;
						}
						else if (transform.position.x == 580 && transform.position.y == 456)
						{
							char v = 'V';
							text.text += v;
						}
						if (transform.position.x == 644 && transform.position.y == 456)
						{
							char w = 'W';
							text.text += w;
						}
						else if (transform.position.x == 708 && transform.position.y == 456)
						{
							char x = 'X';
							text.text += x;
						}
						else if (transform.position.x == 260 && transform.position.y == 584)
						{
							char y = 'Y';
							text.text += y;
						}
						else if (transform.position.x == 324 && transform.position.y == 584)
						{
							char z = 'Z';
							text.text += z;
						}
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
						//loader.SaveSlotData(slot);
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