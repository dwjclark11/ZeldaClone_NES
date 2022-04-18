#include "GamePadSystem.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/GamePadComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TextLabelComponent.h"
#include "../../Components/SettingsComponent.h"
#include "../SoundFXSystem.h"
#include "KeyboardControlSystem.h"
#include <filesystem>
#include "../../States/PauseState.h"
#include "../../States/MenuState.h"
#include "../../States/GameState.h"
#include "../../States/NameState.h"
#include "../../States/EditorState.h"
#include "../../States/SettingsState.h"
#include "../../Game/Game.h"
#include "../../Game/LevelLoader.h"
#include "../../Utilities/Utility.h"

bool GamePadSystem::paused = false;

GamePadSystem::GamePadSystem()
	: gameController(nullptr)
	, game(Game::Instance())

{
	RequiredComponent<GamePadComponent>();
}

void GamePadSystem::Init()
{
	if (SDL_NumJoysticks() < 1)
	{
		Logger::Err("Gamepad Not Connected");
	}
	else
	{
		// Load Joystick
		gameController = SDL_GameControllerOpen(0);
		if (gameController == nullptr)
		{
			Logger::Err("Unable to open game controller!");
		}
	}
}

void GamePadSystem::GameStateBtns(GamePadButtonPressedEvent& event)
{
	if (game.GetFadeAlpha() == 255 && !game.PlayerHold() && event.button != game.GetBtnBindings().at(Game::Action::ATTACK))
	{
		auto player = Registry::Instance().GetEntityByTag("player");
		auto& playerControl = player.GetComponent<KeyboardControlComponent>();
		

		if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_UP))
		{
			ChangePlayerAttrib(playerControl.upVelocity, glm::vec2(48, 32), glm::vec2(64, 60), 24, 2, 2, 2, 2);
			
			KeyboardControlSystem::dir = UP;
		}

		if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_RIGHT))
		{
			ChangePlayerAttrib(playerControl.rightVelocity, glm::vec2(90, 56), glm::vec2(64, 60), 2, 30, 3, 2, 2);

			KeyboardControlSystem::dir = RIGHT;
		}
		if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
		{
			ChangePlayerAttrib(playerControl.downVelocity, glm::vec2(40, 84), glm::vec2(64, 60), 24, 2, 0, 2, 2);

			KeyboardControlSystem::dir = DOWN;
		}
		if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_LEFT))
		{
			ChangePlayerAttrib(playerControl.leftVelocity, glm::vec2(30, 50), glm::vec2(64, 60), 30, 2, 1, 2, 2);

			KeyboardControlSystem::dir = LEFT;
		}
	}
}

void GamePadSystem::MenuStateBtns(GamePadButtonPressedEvent& event)
{
	auto entity = Registry::Instance().GetEntityByTag("selector");

	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();

	if (!Game::isDebug)
	{
		if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_UP))
		{
			transform.position.y -= sprite.height * 6;
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
			if (transform.position.y < 200) transform.position.y = 584;
					
		}

		if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
		{
			transform.position.y += sprite.height * 6;
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
			if (transform.position.y > 584) transform.position.y = 200;
			//GamePadSystem::buttonDirDown = true;
		}

		if (event.button == game.GetBtnBindings().at(Game::Action::SELECT))
		{
			if (transform.position.y == 200)
			{
				if (MenuState::player1Name.size() != 0)
				{
					game.SetPlayerNum(1);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}

			}
			else if (transform.position.y == 296)
			{
				if (MenuState::player2Name.size() != 0)
				{
					game.SetPlayerNum(2);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}

			}
			else if (transform.position.y == 392)
			{
				if (MenuState::player1Name.size() != 0)
				{
					game.SetPlayerNum(3);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}
			}
			else if (transform.position.y == 488)
			{
				if (MenuState::player1Name.size() != 0 && MenuState::player2Name.size() != 0 && MenuState::player3Name.size() != 0)
				{
					MenuState::slotsFull = true;
				}
				game.GetStateMachine()->PopState();
				game.GetStateMachine()->PushState(new NameState());
			}
			else if (transform.position.y == 584)
			{

			}
		}
	}
	else
	{
		if (event.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		{
			transform.position.y -= sprite.height * 6;
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
			if (!MenuState::eliminate)
			{
				if (transform.position.y < 200) 
					transform.position.y = 776;
			}
			else
			{
				if (transform.position.y < 200)
					transform.position.y = 392;
			}
		}

		if (event.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		{
			transform.position.y += sprite.height * 6;
			Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
			
			if (!MenuState::eliminate)
			{
				if (transform.position.y > 776) 
					transform.position.y = 200;
			}
			else
			{ 
				if (transform.position.y > 392)
					transform.position.y = 200;
			}
		}

		if (event.button == game.GetBtnBindings().at(Game::Action::SELECT))
		{
			if (transform.position.y == 200)
			{
				if (MenuState::player1Name.size() != 0 &&!MenuState::eliminate)
				{
					game.SetPlayerNum(1);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}
				else if (MenuState::player1Name.size() != 0 && MenuState::eliminate)
				{
					// Remove file from Saved files
					const std::string save1File = "./Assets/SavedFiles/save1.lua";
					if (std::filesystem::remove(save1File))
					{
						Logger::Log("Player: " + MenuState::player1Name + " was eliminated");
						LevelLoader loader;
						loader.EliminatePlayerToDefault(1, MenuState::player1Name);
						MenuState::player1Name = "";
						MenuState::eliminate = false;
					}
					else
						Logger::Err("Error, File could not be deleted");
				}
			}
			else if (transform.position.y == 296)
			{
				if (MenuState::player2Name.size() != 0 && !MenuState::eliminate)
				{
					game.SetPlayerNum(2);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}
				else if (MenuState::player2Name.size() != 0 && MenuState::eliminate)
				{
					// Remove file from Saved files
					const std::string save2File = "./Assets/SavedFiles/save2.lua";
					if (std::filesystem::remove(save2File))
					{
						LevelLoader loader;
						loader.EliminatePlayerToDefault(2, MenuState::player2Name);
						MenuState::player2Name = "";
						MenuState::eliminate = false;
					}
					else
						Logger::Err("Error, File could not be deleted");
				}

			}
			else if (transform.position.y == 392)
			{
				if (MenuState::player3Name.size() != 0 && !MenuState::eliminate)
				{
					game.SetPlayerNum(3);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}
				else if (MenuState::player3Name.size() != 0 && MenuState::eliminate)
				{
					// Remove file from Saved files
					const std::string save3File = "./Assets/SavedFiles/save3.lua";
					if (std::filesystem::remove(save3File))
					{
						Logger::Log("Player: " + MenuState::player3Name + " was eliminated");
						LevelLoader loader;
						loader.EliminatePlayerToDefault(3, MenuState::player3Name);
						MenuState::player3Name = "";
						MenuState::eliminate = false;
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
				}
				game.GetStateMachine()->PopState();
				game.GetStateMachine()->PushState(new NameState());
			}
			else if (transform.position.y == 584)
			{
				Logger::Log("Eliminate");
				if (MenuState::player1Name.size() != 0 || MenuState::player2Name.size() != 0 || MenuState::player3Name.size() != 0)
				{
					MenuState::eliminate = !MenuState::eliminate;
					transform.position.y = 200;
				}
			}
			else if (transform.position.y == 680)
			{
				game.GetStateMachine()->PopState();
				game.GetStateMachine()->PushState(new SettingsState());
			}
			else if (transform.position.y == 776)
			{
				game.GetStateMachine()->PopState();
				game.GetStateMachine()->PushState(new EditorState());
			}
		}
		else if (event.button == game.GetBtnBindings().at(Game::Action::CANCEL))
		{
			if (MenuState::eliminate)
				MenuState::eliminate = false;
		}
	}
}

void GamePadSystem::SettingsStateBtns(GamePadButtonPressedEvent& event)
{
	auto selector = Registry::Instance().GetEntityByTag("settings_selector");
	auto& selectTransform = selector.GetComponent<TransformComponent>();


	for (const auto& entity : GetSystemEntities())
	{
		if (entity.HasComponent<SettingsComponent>())
		{
			const auto& settings = entity.GetComponent<SettingsComponent>();

			if (!SettingsState::mEnterKey)
			{
				if (event.button == SDL_CONTROLLER_BUTTON_A)
				{
					SettingsState::mEnterKey = true;
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "bomb_drop", 0, 1);
					break;

				}
				else if (event.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
				{
					selectTransform.position.y -= 75;
					SettingsState::mActionIndex--;

					if (selectTransform.position.y < 225)
					{
						selectTransform.position.y = 600;
						SettingsState::mActionIndex = 5;
					}
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				}
				else if (event.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
				{
					selectTransform.position.y += 75;
					SettingsState::mActionIndex++;

					if (selectTransform.position.y > 600)
					{
						selectTransform.position.y = 225;
						SettingsState::mActionIndex = 0;
					}
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				}
			}
			else
			{
				Game::Instance().ChangeBtnBinding(static_cast<Game::Action>(SettingsState::mActionIndex), 
					static_cast<SDL_GameControllerButton>(event.button));
				
				std::string btnPressed = std::string(SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(event.button)));
				
				if (entity.HasComponent<TextLabelComponent>())
				{
					Logger::Log("set index: " + std::to_string(settings.index));

					if (settings.index == SettingsState::mActionIndex && settings.input == SettingsComponent::Input::BUTTON)
					{
						auto& text = entity.GetComponent<TextLabelComponent>();
						text.text = btnPressed;
						Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "get_item", 0, 1);
						SettingsState::mEnterKey = false;
						break;
					}
				}
			}
		}
		else
			continue;
	}



	auto& selectText = selector.GetComponent<TextLabelComponent>();
	if (SettingsState::mEnterKey)
	{
		switch (SettingsState::mActionIndex)
		{
		case 0: selectText.text = "Changing Move Up Button"; break;
		case 1: selectText.text = "Changing Move Down Button"; break;
		case 2: selectText.text = "Changing Move Left Button"; break;
		case 3: selectText.text = "Changing Move Right Button"; break;
		case 4: selectText.text = "Changing Attack Button"; break;
		case 5: selectText.text = "Changing Use Item Button"; break;
		default: break;
		}
	}
	else
		selectText.text = "Choose an Action to Change!";
}


void GamePadSystem::NameStateBtns(GamePadButtonPressedEvent& event)
{
	if (MenuState::player1Name.size() == 0)
		NameState::slot = 1;
	else if (MenuState::player2Name.size() == 0)
		NameState::slot = 2;
	else if (MenuState::player3Name.size() == 0)
		NameState::slot = 3;
	
	for (auto& entity : GetSystemEntities())
	{
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& text = entity.GetComponent<TextLabelComponent>();

		if (entity.HasTag("box"))
		{
			if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_UP))
			{
				transform.position.y -= sprite.height * transform.scale.y * 2;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				NameState::row--;
				if (transform.position.y < 200)
				{
					transform.position.y = 584;
					NameState::row = 3;
				}
				Logger::Log("row: " + std::to_string(NameState::row));
				break;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
			{
				transform.position.y += sprite.height * transform.scale.y * 2;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				NameState::row++;
				if (transform.position.y > 584)
				{
					transform.position.y = 200;
					NameState::row = 0;
				}
				Logger::Log("row: " + std::to_string(NameState::row));
				break;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_RIGHT))
			{
				transform.position.x += sprite.width * transform.scale.x;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				NameState::col++;
				if (transform.position.x > 708)
				{
					transform.position.x = 260;
					NameState::col = 0;
				}
				Logger::Log("col: " + std::to_string(NameState::col));
				break;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_LEFT))
			{
				transform.position.x -= sprite.width * transform.scale.x;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				NameState::col--;
				if (transform.position.x < 260)
				{
					transform.position.x = 708;
					NameState::col = 7;
				}
				Logger::Log("col: " + std::to_string(NameState::col));
			}

			if (event.button == game.GetBtnBindings().at(Game::Action::SELECT))
			{
				if (text.text.size() < 6)
				{
					bool valid = true;
					// Make sure that the row and col have a valid letter based on the 
					// name-letters sprite
					if (NameState::row == 3 && NameState::col > 1)
					{
						valid = false;
					}

					// Create a new char based on ASCII Upper characters and the letter position
					// on the sprite
					char newChar = (NameState::row * 8) + NameState::col + 65; // 65 is 'A'

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
					NameState::row = 0;
					NameState::col = 0;
					loader.SavePlayerNameToLuaTable(std::to_string(NameState::slot), NameState::name);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new MenuState());
				}
			}
		}
	}
}

void GamePadSystem::PauseStateBtns(GamePadButtonPressedEvent& event)
{
	for (const auto& entity : GetSystemEntities())
	{
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();

		if (entity.HasTag("pauseSelector"))
		{
			if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_UP))
			{
				transform.position.y -= ((sprite.height * transform.scale.y) + 6);
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y < 190) transform.position.y = 260;
				
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
			{
				transform.position.y += ((sprite.height * transform.scale.y) + 6);
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y > 260) transform.position.y = 190;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_RIGHT))
			{
				transform.position.x += 100;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.x > 686) transform.position.x = 386;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_LEFT))
			{
				transform.position.x -= 100;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.x < 386) transform.position.x = 686;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::SELECT))
			{

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


void GamePadSystem::SubscribeToEvents(std::unique_ptr<class EventManager>& eventManager)
{
	const auto& currentState = game.GetStateMachine()->GetCurrentState();
	if ((!game.GetCameraMoving() && !game.GetPlayerItem() && !game.GetPlayerDead()) || currentState == "GAMEOVER")
	{
		eventManager->SubscribeToEvent<GamePadButtonPressedEvent>(this, &GamePadSystem::OnButtonPressed);
		
	}
}


void GamePadSystem::OnButtonPressed(GamePadButtonPressedEvent& event)
{
	if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE")
	{
		GameStateBtns(event);
	}
	else if (game.GetStateMachine()->GetCurrentState() == "MENU")
	{
		MenuStateBtns(event);
	}
	else if (game.GetStateMachine()->GetCurrentState() == "NAME")
	{
		NameStateBtns(event);
	}
	else if (game.GetStateMachine()->GetCurrentState() == "PAUSE")
	{
		PauseStateBtns(event);
	}
	else if (game.GetStateMachine()->GetCurrentState() == "SETTINGS")
	{
		SettingsStateBtns(event);
	}
}
