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
#include <filesystem>
#include "../../Game/LevelLoader.h"

class MenuKeyboardControlSystem : public System
{
private:
	class Game& game;
	bool eliminate;
public:
	MenuKeyboardControlSystem()
		: game(Game::Instance()), eliminate(false)
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
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y < 200) transform.position.y = 584;
						break;

					case SDLK_DOWN:
						transform.position.y += sprite.height * 6;
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y > 584) transform.position.y = 200;
						break;

					case SDLK_SPACE:
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0)
							{
								game.GetPlayerNum() = 1;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState(glm::vec2(7168, 4416)));
							}

						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0)
							{
								game.GetPlayerNum() = 2;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player3Name.size() != 0)
							{
								game.GetPlayerNum() = 3;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							game.GetStateMachine()->PopState();
							game.GetStateMachine()->PushState(new NameState());
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
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						
						if (!eliminate)
						{
							if (transform.position.y < 200)
								transform.position.y = 680;
						}
						else
						{
							if (transform.position.y < 200)
								transform.position.y = 392;
						}

						break;

					case SDLK_DOWN:
						transform.position.y += sprite.height * 6;
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						
						if (!eliminate)
						{
							if (transform.position.y > 680) 
								transform.position.y = 200;
						}
						else
						{
							if (transform.position.y > 392) 
								transform.position.y = 200;
						}
						
						break;

					case SDLK_SPACE:
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0 && !eliminate)
							{
								game.GetPlayerNum() = 1;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}
							else if (MenuState::player1Name.size() != 0 && eliminate)
							{
								// Remove file from Saved files
								const std::string save1File = "./Assets/SavedFiles/save1.lua";
								if (std::filesystem::remove(save1File))
								{
									Logger::Log("Player: " + MenuState::player1Name + " was eliminated");
									LevelLoader loader;
									loader.EliminatePlayerToDefault(1, MenuState::player1Name);
									MenuState::player1Name = "";
									eliminate = false;
								}
								else
									Logger::Err("Error, File could not be deleted");
							}
						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0 && !eliminate)
							{
								game.GetPlayerNum() = 2;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}
							else if (MenuState::player2Name.size() != 0 && eliminate)
							{
								// Remove file from Saved files
								const std::string save2File = "./Assets/SavedFiles/save2.lua";
								if (std::filesystem::remove(save2File))
								{
									Logger::Log("Player: " + MenuState::player2Name + " was eliminated");
									LevelLoader loader;
									loader.EliminatePlayerToDefault(2, MenuState::player2Name);
									MenuState::player2Name = "";
									eliminate = false;
								}
								else
									Logger::Err("Error, File could not be deleted");
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player3Name.size() != 0 && !eliminate)
							{
								game.GetPlayerNum() = 3;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}
							else if (MenuState::player3Name.size() != 0 && eliminate)
							{
								// Remove file from Saved files
								const std::string save3File = "./Assets/SavedFiles/save3.lua";
								if (std::filesystem::remove(save3File))
								{
									Logger::Log("Player: " + MenuState::player3Name + " was eliminated");
									LevelLoader loader;
									loader.EliminatePlayerToDefault(3, MenuState::player3Name);
									MenuState::player3Name = "";
									eliminate = false;
								}
								else
									Logger::Err("Error, File could not be deleted");
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							game.GetStateMachine()->PopState();
							game.GetStateMachine()->PushState(new NameState());
						}
						else if (transform.position.y == 584)
						{
							Logger::Log("Eliminate");
							if (MenuState::player1Name.size() != 0 || MenuState::player2Name.size() != 0 || MenuState::player3Name.size() != 0)
							{
								eliminate = !eliminate;
								transform.position.y = 200;
							}
						}
						else if (transform.position.y == 680)
						{
							game.GetStateMachine()->PopState();
							game.GetStateMachine()->PushState(new EditorState());
						}
						break;
					}
					case SDLK_BACKSPACE:
					{
						if (eliminate)
							eliminate = false;
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
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y < 200) transform.position.y = 584;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::downPressed && !GamePadSystem::buttonDirDown)
					{
						transform.position.y += sprite.height * 6;
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y > 584) transform.position.y = 200;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0)
							{
								game.GetPlayerNum() = 1;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0)
							{
								game.GetPlayerNum() = 2;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player1Name.size() != 0)
							{
								game.GetPlayerNum() = 3;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							game.GetStateMachine()->PopState();
							game.GetStateMachine()->PushState(new NameState());
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
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y < 200) transform.position.y = 680;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::downPressed && !GamePadSystem::buttonDirDown)
					{
						transform.position.y += sprite.height * 6;
						game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
						if (transform.position.y > 680) transform.position.y = 200;
						GamePadSystem::buttonDirDown = true;
					}

					if (GamePadSystem::aPressed && !GamePadSystem::buttonDown)
					{
						if (transform.position.y == 200)
						{
							if (MenuState::player1Name.size() != 0)
							{
								game.GetPlayerNum() = 1;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 296)
						{
							if (MenuState::player2Name.size() != 0)
							{
								game.GetPlayerNum() = 2;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}

						}
						else if (transform.position.y == 392)
						{
							if (MenuState::player1Name.size() != 0)
							{
								game.GetPlayerNum() = 3;
								game.GetStateMachine()->PopState();
								game.GetStateMachine()->PushState(new GameState());
							}
						}
						else if (transform.position.y == 488)
						{
							if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
							{
								MenuState::slotsFull = true;
								break;
							}
							game.GetStateMachine()->PopState();
							game.GetStateMachine()->PushState(new NameState());
						}
						else if (transform.position.y == 584)
						{

						}
						else if (transform.position.y == 680)
						{
							game.GetStateMachine()->PopState();
							game.GetStateMachine()->PushState(new EditorState());
						}
						GamePadSystem::buttonDown = true;
					}
				}
			}
		}
	}
};
