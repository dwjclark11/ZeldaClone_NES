#pragma once
#include "../ECS/ECS.h"
#include "../Events/EventManager.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Logger/Logger.h"
#include "../AssetManager/AssetManager.h"
#include "../States/EditorState.h"
#include "../States/NameState.h"
#include "../Systems/SoundFXSystem.h"
#include "../GameSystems/GamePadSystem.h"
#include <string>
#include <SDL.h>


class MenuKeyboardControlSystem : public System
{
public:
	MenuKeyboardControlSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<SpriteComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &MenuKeyboardControlSystem::OnKeyPressed); // Callback Function
	}

	// Callback function
	void OnKeyPressed(KeyPressedEvent& event)
	{
		for (auto entity : GetSystemEntities())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			if (entity.HasTag("selector"))
			{
				if (!Game::isDebug)
				{
					switch (event.symbol)
					{
					case SDLK_UP:
						transform.position.y -= sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y < 200) transform.position.y = 584;
						break;

					case SDLK_DOWN:
						transform.position.y += sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y > 584) transform.position.y = 200;
						break;

					case SDLK_SPACE:
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 1;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 2;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player3Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 3;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							Game::Instance()->GetStateMachine()->PopState();
							Game::Instance()->GetStateMachine()->PushState(new NameState());
						}
						else if (transform.position.y == 584)
						{

						}
						break;
					}
					default:
						break;
					}
				}
				else
				{
					switch (event.symbol)
					{
					case SDLK_UP:
						transform.position.y -= sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y < 200) transform.position.y = 680;
						break;

					case SDLK_DOWN:
						transform.position.y += sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y > 680) transform.position.y = 200;
						break;

					case SDLK_SPACE:
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 1;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 2;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player3Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 3;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							Game::Instance()->GetStateMachine()->PopState();
							Game::Instance()->GetStateMachine()->PushState(new NameState());
						}
						else if (transform.position.y == 584)
						{

						}
						else if (transform.position.y == 680)
						{
							Game::Instance()->GetStateMachine()->PopState();
							Game::Instance()->GetStateMachine()->PushState(new EditorState());
						}
						break;
					}
					default:
						break;
					}
				}
			}
		}
	}

	void Update()
	{
		for (auto entity : GetSystemEntities())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			if (entity.HasTag("selector"))
			{
				if (!Game::isDebug)
				{
					if (GamePadSystem::upPressed && !GamePadSystem::buttonDirDown)
					{
						transform.position.y -= sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y < 200) transform.position.y = 584;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::downPressed && !GamePadSystem::buttonDirDown)
					{
						transform.position.y += sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y > 584) transform.position.y = 200;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 1;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 2;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player1Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 3;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							Game::Instance()->GetStateMachine()->PopState();
							Game::Instance()->GetStateMachine()->PushState(new NameState());
						}
						else if (transform.position.y == 584)
						{

						}
						GamePadSystem::buttonDown = true;
					}
				}
				else
				{
					if (GamePadSystem::upPressed && !GamePadSystem::buttonDirDown)
					{
						transform.position.y -= sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y < 200) transform.position.y = 680;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::downPressed && !GamePadSystem::buttonDirDown)
					{
						transform.position.y += sprite.height * 6;
						Game::Instance()->GetSystem<SoundFXSystem>().PlaySoundFX(Game::Instance()->GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y > 680) transform.position.y = 200;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 1;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 2;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player1Name.size() != 0)
							{
								Game::Instance()->GetPlayerNum() = 3;
								Game::Instance()->GetStateMachine()->PopState();
								Game::Instance()->GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							Game::Instance()->GetStateMachine()->PopState();
							Game::Instance()->GetStateMachine()->PushState(new NameState());
						}
						else if (transform.position.y == 584)
						{

						}
						else if (transform.position.y == 680)
						{
							Game::Instance()->GetStateMachine()->PopState();
							Game::Instance()->GetStateMachine()->PushState(new EditorState());
						}
						GamePadSystem::buttonDown = true;
					}
				}
			}
		}
	}
};
