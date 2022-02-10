#include "GamePadSystem.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TextLabelComponent.h"
#include "../../Systems/PauseSystems/ItemSelectKeyboardSystem.h"
#include "../SoundFXSystem.h"
#include "KeyboardControlSystem.h"

#include "../../States/PauseState.h"
#include "../../States/MenuState.h"
#include "../../States/GameState.h"
#include "../../States/NameState.h"
#include "../../States/EditorState.h"
#include "../../Game/Game.h"
#include "../../Game/LevelLoader.h"

bool GamePadSystem::paused = false;

GamePadSystem::GamePadSystem()
	: gameController(nullptr)
	, game(Game::Instance())
	, row(0)
	, col(0)
	, slot(0)
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
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
	if (game.GetFadeAlpha() == 255)
	{
		auto player = Registry::Instance().GetEntityByTag("player");
		auto& playerTransform = player.GetComponent<TransformComponent>();
		auto& playerCollider = player.GetComponent<BoxColliderComponent>();
		auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
		auto& playerSprite = player.GetComponent<SpriteComponent>();
		auto& playerControl = player.GetComponent<KeyboardControlComponent>();

		auto shield = Registry::Instance().GetEntityByTag("the_shield");
		auto& shieldTransform = shield.GetComponent<TransformComponent>();
		auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
		auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

		auto sword = Registry::Instance().GetEntityByTag("the_sword");
		auto& swordTransform = sword.GetComponent<TransformComponent>();
		auto& swordCollider = sword.GetComponent<BoxColliderComponent>();
		auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

		if (!game.PlayerHold() || event.button != game.GetKeyBindings().at(Game::Action::ATTACK))
		{
			if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_UP))
			{
				playerRigidbody.velocity = playerControl.upVelocity;
				playerSprite.srcRect.x = playerSprite.width * 2;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 2;
				shieldCollider.width = 24;
				shieldCollider.offset = glm::vec2(48, 32);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				KeyboardControlSystem::dir = UP;
			}

			if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_RIGHT))
			{
				playerSprite.srcRect.x = playerSprite.width * 3;
				playerRigidbody.velocity = playerControl.rightVelocity;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 30;
				shieldCollider.width = 2;
				shieldCollider.offset = glm::vec2(90, 56);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				KeyboardControlSystem::dir = RIGHT;
			}
			if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
			{
				playerRigidbody.velocity = playerControl.downVelocity;
				playerSprite.srcRect.x = playerSprite.width * 0;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 2;
				shieldCollider.width = 24;
				shieldCollider.offset = glm::vec2(40, 84);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				KeyboardControlSystem::dir = DOWN;
			}
			if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_LEFT))
			{
				playerRigidbody.velocity = playerControl.leftVelocity;
				playerSprite.srcRect.x = playerSprite.width * 1;

				shieldTransform.position = playerTransform.position;
				shieldCollider.height = 30;
				shieldCollider.width = 2;
				shieldCollider.offset = glm::vec2(30, 50);
				shieldRigidbody = playerRigidbody;

				swordTransform.position = playerTransform.position;
				swordCollider.height = 2;
				swordCollider.width = 2;
				swordCollider.offset = glm::vec2(64, 60);
				swordRigidbody = playerRigidbody;

				KeyboardControlSystem::dir = LEFT;
			}
		}
	}
}

void GamePadSystem::MenuStateBtns(GamePadButtonPressedEvent& event)
{
	for (auto& entity : GetSystemEntities())
	{
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();

		if (entity.HasTag("selector"))
		{
			if (!Game::isDebug)
			{
				if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_UP))
				{
					transform.position.y -= sprite.height * 6;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 200) transform.position.y = 584;
					
				}

				if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
				{
					transform.position.y += sprite.height * 6;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 584) transform.position.y = 200;
					//GamePadSystem::buttonDirDown = true;
				}

				if (event.button == game.GetBtnBindings().at(Game::Action::SELECT))
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
				}
			}
			else
			{
				if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_UP))
				{
					transform.position.y -= sprite.height * 6;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 200) transform.position.y = 680;
					//GamePadSystem::buttonDirDown = true;
				}

				if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
				{
					transform.position.y += sprite.height * 6;
					game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 680) transform.position.y = 200;
					//GamePadSystem::buttonDirDown = true;
				}

				if (event.button == game.GetBtnBindings().at(Game::Action::SELECT))
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
					//GamePadSystem::buttonDown = true;
				}
			}
		}
	}
}

void GamePadSystem::NameStateBtns(GamePadButtonPressedEvent& event)
{
	if (MenuState::player1Name.size() == 0)
		slot = 1;
	else if (MenuState::player2Name.size() == 0)
		slot = 2;
	else if (MenuState::player3Name.size() == 0)
		slot = 3;
	

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
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				row--;
				if (transform.position.y < 200)
				{
					transform.position.y = 584;
					row = 3;
				}
				Logger::Log("row: " + std::to_string(row));
				break;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
			{
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
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_RIGHT))
			{
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
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_LEFT))
			{
				transform.position.x -= sprite.width * transform.scale.x;
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				col--;
				if (transform.position.x < 260)
				{
					transform.position.x = 708;
					col = 7;
				}
				Logger::Log("col: " + std::to_string(col));
			}

			if (event.button == game.GetBtnBindings().at(Game::Action::SELECT))
			{
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
					char newChar = (row * 8) + col + 65; // 65 is 'A'

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
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y < 190) transform.position.y = 260;
				
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_DOWN))
			{
				transform.position.y += ((sprite.height * transform.scale.y) + 6);
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y > 260) transform.position.y = 190;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_RIGHT))
			{
				transform.position.x += 100;
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.x > 686) transform.position.x = 386;
			}
			else if (event.button == game.GetBtnBindings().at(Game::Action::MOVE_LEFT))
			{
				transform.position.x -= 100;
				game.GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
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
	eventManager->SubscribeToEvent<GamePadButtonPressedEvent>(this, &GamePadSystem::OnButtonPressed);
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
}
