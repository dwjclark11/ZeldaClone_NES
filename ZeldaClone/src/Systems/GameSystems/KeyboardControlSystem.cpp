#include "KeyboardControlSystem.h"
#include "../../Game/Game.h"
#include "../../Game/LevelLoader.h"
#include "../../Events/EventManager.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/SettingsComponent.h"
#include "../../Components/TextLabelComponent.h"
#include "../../Components/SaveComponent.h"
#include "../../Components/MenuComponent.h"
#include "../../Components/PauseComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Logger/Logger.h"
#include "../../AssetManager/AssetManager.h"
#include "../../States/GameState.h"
#include "../../States/MenuState.h"
#include "../../States/NameState.h"
#include "../../States/EditorState.h"
#include "../../States/SettingsState.h"
#include "../../States/GameOverState.h"
#include "../SoundFXSystem.h"
#include <string>
#include <filesystem>
#include <SDL.h>
#include "../../Utilities/Timer.h"


directionE KeyboardControlSystem::dir = directionE::NONE;
bool KeyboardControlSystem::keyDown = false;

void KeyboardControlSystem::UpdatePlayer()
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

	// reset the sword
	swordRigidbody = glm::vec2(0);
	swordCollider.offset = glm::vec2(64, 60);
	swordCollider.height = 4;
	swordCollider.width = 4;

	// Reset Player velocity and box collider
	playerRigidbody = glm::vec2(0);
	playerCollider.offset = glm::vec2(45, 45);

	if (dir == UP)
	{
		playerSprite.srcRect.x = playerSprite.width * 2;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == RIGHT)
	{
		playerSprite.srcRect.x = playerSprite.width * 3;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == DOWN)
	{
		playerSprite.srcRect.x = playerSprite.width * 0;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}
	else if (dir == LEFT)
	{
		playerSprite.srcRect.x = playerSprite.width * 1;
		playerSprite.srcRect.y = playerSprite.height * 0;
	}

	// Reset the Shield based on player direction
	shieldRigidbody = glm::vec2(0);
	if (dir == UP)
	{
		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(48, 32);
	}
	else if (dir == RIGHT)
	{
		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(90, 56);
	}
	else if (dir == DOWN)
	{
		swordCollider.height = 2;
		swordCollider.width = 24;
		swordCollider.offset = glm::vec2(40, 84);
	}
	else if (dir == LEFT)
	{
		swordCollider.height = 30;
		swordCollider.width = 2;
		swordCollider.offset = glm::vec2(30, 50);
	}
}

void KeyboardControlSystem::Update()
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

		/*	if (!game.PlayerHold() && event.symbol != SDLK_RSHIFT)
			{*/
			/*switch (event.symbol)
			{
			case SDLK_w:*/
		const Uint8* state = SDL_GetKeyboardState(NULL);

		
		if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_RSHIFT])
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

			dir = UP;
		}
		//break;

	/*case SDLK_d:*/
		if (state[SDL_SCANCODE_D] && !state[SDL_SCANCODE_RSHIFT])
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

			dir = RIGHT;
		}
		//break;

	/*case SDLK_s:*/
		if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_RSHIFT])
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

			dir = DOWN;
		}
		//break;

	/*case SDLK_a:*/
		if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_RSHIFT])
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

			dir = LEFT;
		}
	}
}

void KeyboardControlSystem::MenuStateKeys(KeyPressedEvent& event)
{
	for (const auto& entity : GetSystemEntities())
	{
		if (entity.HasTag("selector") && entity.HasComponent<MenuComponent>())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			if (!Game::isDebug)
			{
				switch (event.symbol)
				{
				case SDLK_UP:
					transform.position.y -= sprite.height * 6;
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 200) transform.position.y = 584;
					break;

				case SDLK_DOWN:
					transform.position.y += sprite.height * 6;
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 584) transform.position.y = 200;
					break;

				case SDLK_SPACE:
				{
					if (transform.position.y == 200)
					{
						if (MenuState::player1Name.size() != 0)
						{
							game.SetPlayerNum(1);
							game.GetStateMachine()->PopState();
							game.GetStateMachine()->PushState(new GameState(glm::vec2(7168, 4416)));
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
						if (MenuState::player3Name.size() != 0)
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
							break;
						}
						//game.GetStateMachine()->PopState();
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

					break;

				case SDLK_DOWN:
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

					break;

				case SDLK_SPACE:
				{
					if (transform.position.y == 200)
					{
						if (MenuState::player1Name.size() != 0 && !MenuState::eliminate)
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
								Logger::Log("Player: " + MenuState::player2Name + " was eliminated");
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
					break;
				}
				case SDLK_BACKSPACE:
				{
					if (MenuState::eliminate)
						MenuState::eliminate = false;
					break;
				}

				default:
					break;
				}
			}
		}
		else 
			continue;
	}
}

void KeyboardControlSystem::SettingsStateKeys(KeyPressedEvent& event)
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
				if (event.symbol == SDLK_RETURN)
				{
					SettingsState::mEnterKey = true;
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "bomb_drop", 0, 1);
					break;
					
				}
				else if (event.symbol == SDLK_UP)
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
				else if (event.symbol == SDLK_DOWN)
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
				Game::Instance().ChangeKeyBinding(static_cast<Game::Action>(SettingsState::mActionIndex), event.symbol);
				std::string keyPressed = std::string(SDL_GetKeyName(event.symbol));
				if (entity.HasComponent<TextLabelComponent>())
				{
					
					if (settings.index == SettingsState::mActionIndex && settings.input == SettingsComponent::Input::KEY)
					{
						Logger::Log(keyPressed);
						auto& text = entity.GetComponent<TextLabelComponent>();
						text.text = keyPressed;
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
		case 0: selectText.text = "Changing Move Up Key"; break;
		case 1: selectText.text = "Changing Move Down Key"; break;
		case 2: selectText.text = "Changing Move Left Key"; break;
		case 3: selectText.text = "Changing Move Right Key"; break;
		case 4: selectText.text = "Changing Attack Key"; break;
		case 5: selectText.text = "Changing Use Item Key"; break;
		default: break;
		}
	}
	else
		selectText.text = "Choose an Action to Change!";
}

void KeyboardControlSystem::EditorStateKeys(KeyPressedEvent& event)
{
	switch (event.symbol)
	{
		// Move the camera UP
	case SDLK_w:
		//game.SetCameraY(-16); // Why 16? Should be based on the sprite_size*scale
		game.GetCamera().y -= 32;
		break;
		// Move the camera RIGHT
	case SDLK_d:
		//game.SetCameraX(16);
		game.GetCamera().x += 64;
		break;
		// Move the camera DOWN
	case SDLK_s:
		//game.SetCameraY(16);
		game.GetCamera().y += 32;
		break;
		// Move the camera LEFT
	case SDLK_a:
		//game.SetCameraX(-16);
		game.GetCamera().x -= 64;
		break;
		// Toggle the Collision
	case SDLK_c:
		//MouseControlSystem::isCollision = !MouseControlSystem::isCollision; // Also changed with the editor ImGui!!
		break;

		// I don't think this works yet!
	case SDLK_F1:
		//FileDialogs::SaveFile();
		break;
		// Change the type of trigger --> This will be removed when drop-down list is funcitonal
	case SDLK_m:
		/*	if(MouseControlSystem::triggerType == SECRET_AREA || MouseControlSystem::triggerType == ENTER_DUNGEON)
				MouseControlSystem::triggerLevelNum++;*/
		break;
		// Same as above
	case SDLK_n:
		/*if (MouseControlSystem::triggerType == SECRET_AREA || MouseControlSystem::triggerType == ENTER_DUNGEON)
			MouseControlSystem::triggerLevelNum--;
		if (MouseControlSystem::triggerLevelNum-- < 0)
			MouseControlSystem::triggerLevelNum = 0;*/
		break;
		// This will also be removed with the implementation of the drop-list stated above
	}



	// Clamps for Game Camera and Image Src Rec!! --> This needs to be investigated further
	if (game.GetCamera().x < -1000) game.GetCamera().x = -1000;
	if (game.GetCamera().y < -1000) game.GetCamera().y = -1000;
	//if (game.GetCamera().x > (levelWidth - game.GetCamera().w) + 1000) game.GetCamera().x = (levelWidth - game.GetCamera().w) + 1000;
	//if (game.GetCamera().y > (levelHeight - game.GetCamera().h) + 1000) game.GetCamera().y = (levelHeight - game.GetCamera().h) + 1000;
	//
	//if (MouseControlSystem::spriteComponent.layer < 0) MouseControlSystem::spriteComponent.layer = 0;
	//if (MouseControlSystem::spriteComponent.layer > 10) MouseControlSystem::spriteComponent.layer = 10;
}

void KeyboardControlSystem::PauseStateKeys(KeyPressedEvent& event)
{
	for (const auto& entity : GetSystemEntities())
	{
		if (entity.HasComponent<PauseComponent>())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			if (entity.HasTag("pauseSelector"))
			{
				switch (event.symbol)
				{
				case SDLK_UP:
					transform.position.y -= ((sprite.height * transform.scale.y) + 6);
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y < 190) transform.position.y = 260;
					break;

				case SDLK_DOWN:
					transform.position.y += ((sprite.height * transform.scale.y) + 6);
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.y > 260) transform.position.y = 190;
					break;

				case SDLK_RIGHT:
					transform.position.x += 100;
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
					if (transform.position.x > 686) transform.position.x = 386;
					break;

				case SDLK_LEFT:
					transform.position.x -= 100;
					Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
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


			if (entity.HasTag("selectedItem"))
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
		else 
			continue;
	}
}

void KeyboardControlSystem::NameStateKeys(KeyPressedEvent& event)
{
	if (MenuState::player1Name.size() == 0)
		NameState::slot = 1;
	else if (MenuState::player2Name.size() == 0)
		NameState::slot = 2;
	else if (MenuState::player3Name.size() == 0)
		NameState::slot = 3;

	auto entity = Registry::Instance().GetEntityByTag("box");

	const auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();
	auto& text = entity.GetComponent<TextLabelComponent>();

	switch (event.symbol)
	{
	case SDLK_UP:
		transform.position.y -= sprite.height * transform.scale.y * 2;
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
		NameState::row--;
		if (transform.position.y < 200)
		{
			transform.position.y = 584;
			NameState::row = 3;
		}

		break;

	case SDLK_DOWN:
		transform.position.y += sprite.height * transform.scale.y * 2;
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
		NameState::row++;
		if (transform.position.y > 584)
		{
			transform.position.y = 200;
			NameState::row = 0;
		}

		break;

	case SDLK_RIGHT:
		transform.position.x += sprite.width * transform.scale.x;
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
		NameState::col++;
		if (transform.position.x > 708)
		{
			transform.position.x = 260;
			NameState::col = 0;
		}

		break;

	case SDLK_LEFT:
		transform.position.x -= sprite.width * transform.scale.x;
		Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
		NameState::col--;
		if (transform.position.x < 260)
		{
			transform.position.x = 708;
			NameState::col = 7;
		}

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

		break;
	default:
		break;
	}
}

void KeyboardControlSystem::GameStateKeys(KeyPressedEvent& event)
{
	if (game.GetFadeAlpha() == 255 && !game.PlayerHold() && event.symbol != game.GetKeyBindings().at(Game::Action::ATTACK))
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

		if (event.symbol == game.GetKeyBindings().at(Game::Action::MOVE_UP))
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

			dir = UP;
		}

		if (event.symbol == game.GetKeyBindings().at(Game::Action::MOVE_RIGHT))
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

			dir = RIGHT;
		}
		if (event.symbol == game.GetKeyBindings().at(Game::Action::MOVE_DOWN))
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

			dir = DOWN;
		}
		if (event.symbol == game.GetKeyBindings().at(Game::Action::MOVE_LEFT))
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

			dir = LEFT;
		}
	}
}

void KeyboardControlSystem::SaveStateKeys(KeyPressedEvent& event)
{
	for (const auto& entity : GetSystemEntities())
	{
		if (entity.HasComponent<SaveComponent>())
		{
			if (entity.HasTag("save_selector"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				switch (event.symbol)
				{
				case SDLK_UP:
					transform.position.y -= 200;
					if (transform.position.y < 185) transform.position.y = 585;
					break;

				case SDLK_DOWN:
					transform.position.y += 200;
					if (transform.position.y > 585) transform.position.y = 185;
					break;

				case SDLK_RETURN:
				{
					if (transform.position.y == 185)
					{
						LevelLoader loader;

						if (game.GetPlayerNum() == 1)
						{
							loader.SavePlayerDataToLuaTable("1");
						}
						else if (game.GetPlayerNum() == 2)
						{
							loader.SavePlayerDataToLuaTable("2");
						}
						else if (game.GetPlayerNum() == 3)
						{
							loader.SavePlayerDataToLuaTable("3");
						}

						//Entity saving = Registry::Instance().CreateEntity();
						//saving.AddComponent<SpriteComponent>("save_gui", 96, 16, 1, true, 0, 0);
						//saving.AddComponent<TransformComponent>(glm::vec2(400, 100), glm::vec2(2, 2), 0.0);
						//saving.AddComponent<AnimationComponent>(2, 5);
						//saving.AddComponent<SaveComponent>();
						//Sleep(1000);
						game.GetStateMachine()->PopState();
					}
					else if (transform.position.y == 385)
					{
						//State::exitToMain = true;
						//game.GetStateMachine()->ClearStates();
						//game.GetStateMachine()->PushState(new MenuState());
					}
					else if (transform.position.y == 585)
					{
						game.SetGameRunning(false);
					}
					break;
				}
				default:
					break;
				}
			}
		}
		else
			continue;
	}
}

void KeyboardControlSystem::GameOverStateKeys(KeyPressedEvent& event)
{
	for (auto& entity : GetSystemEntities())
	{
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();

		if (entity.HasTag("gameOverSelector"))
		{
			switch (event.symbol)
			{
			case SDLK_UP:
				transform.position.y -= 100;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y < 475) transform.position.y = 675;
				break;

			case SDLK_DOWN:
				transform.position.y += 100;
				Registry::Instance().GetSystem<SoundFXSystem>().PlaySoundFX(game.GetAssetManager(), "text_slow", 0, 1);
				if (transform.position.y > 675) transform.position.y = 475;
				break;

			case SDLK_SPACE:
			{
				if (transform.position.y == 475)
				{
					game.SetPlayerCreated(false);
					game.SetPlayerDead(false);
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}
				else if (transform.position.y == 575)
				{
					game.GetPlayerNum();
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new GameState());
				}
				else if (transform.position.y == 675)
				{

					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new MenuState());
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

KeyboardControlSystem::KeyboardControlSystem()
	: game(Game::Instance())
{
	//RequiredComponent<RigidBodyComponent>();
	RequiredComponent<KeyboardControlComponent>();
	attack = false;
}

void KeyboardControlSystem::SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
{
	const auto& currentState = game.GetStateMachine()->GetCurrentState();
	if ((!game.GetCameraMoving() && !game.GetPlayerItem() && !game.GetPlayerDead()) || currentState == "GAMEOVER")
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed); // Callback Function
	}
}

void KeyboardControlSystem::OnKeyPressed(KeyPressedEvent& event)
{
	const auto& currentState = game.GetStateMachine()->GetCurrentState();
	
	if (currentState == "GAMESTATE")
	{
		GameStateKeys(event);
	}
	else if (currentState == "MENU")
	{
		MenuStateKeys(event); 
	}
	else if (currentState == "PAUSE")
	{
		PauseStateKeys(event);
	}
	else if (currentState == "NAME")
	{
		NameStateKeys(event);
	}
	else if (currentState == "EDITOR")
	{
		EditorStateKeys(event);
	}
	else if (currentState == "SAVE")
	{
		SaveStateKeys(event);
	}
	else if (currentState == "SETTINGS")
	{
		SettingsStateKeys(event);
	}
	else if (currentState == "GAMEOVER")
	{
		GameOverStateKeys(event);
	}
}

