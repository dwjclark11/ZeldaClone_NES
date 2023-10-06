#include "MenuState.h"
#include "NameState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "EditorState.h"
#include "../Game/Game.h"
#include "../Utilities/GameData.h"
#include "../inputs/InputManager.h"
#include "../inputs/Keyboard.h"
#include "../inputs/Gamepad.h"
#include "GameStateMachine.h"
#include "../Game/LevelLoader.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/MenuComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/MenuSystems/RenderMainMenuSystem.h"
#include "../Systems/EditorSystems/MouseControlSystem.h"
#include "../Systems/EditorSystems/RenderEditorGUISystem.h"
#include "../Systems/EditorSystems/RenderEditorSystem.h"
#include <filesystem>

namespace fs = std::filesystem;

// Define all static variables
const std::string MenuState::menuID = "MENU";

void MenuState::EliminateKeys()
{
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();
	const auto& entity = Registry::Instance().GetEntityByTag("selector");
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();

	if (keyboard.IsKeyJustPressed(KEY_W) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_UP))
	{
		transform.position.y -= sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);

		if (transform.position.y < 200)
			transform.position.y = 392;
	}
	else if (keyboard.IsKeyJustPressed(KEY_S) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_DOWN))
	{
		transform.position.y += sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);

		if (transform.position.y > 392)
			transform.position.y = 200;
	}
	else if (keyboard.IsKeyJustPressed(KEY_SPACE) || gamepad.IsButtonJustPressed(GP_BTN_A))
	{
		if (transform.position.y == 200)
		{

			if (m_GameData.GetPlayer1Name().size() != 0)
			{
				// Remove file from Saved files
				const std::string save1File = "./Assets/SavedFiles/slot_1/save1.lua";
				if (std::filesystem::remove(save1File))
				{
					Logger::Log("Player: " + m_GameData.GetPlayer1Name() + " was eliminated");
					LevelLoader loader;
					loader.EliminatePlayerToDefault(1, m_GameData.GetPlayer1Name());
					m_GameData.SetPlayer1Name("");
					m_bEliminate = false;
					std::filesystem::remove("./Assets/SavedFiles/slot_1/GameSecrets_1.lua");
				}
				else
					Logger::Err("Error, File could not be deleted");
			}
		}
		else if (transform.position.y == 296)
		{

			if (m_GameData.GetPlayer2Name().size() != 0)
			{
				// Remove file from Saved files
				const std::string save2File = "./Assets/SavedFiles/slot_2/save2.lua";
				if (std::filesystem::remove(save2File))
				{
					Logger::Log("Player: " + m_GameData.GetPlayer2Name() + " was eliminated");
					LevelLoader loader;
					loader.EliminatePlayerToDefault(2, m_GameData.GetPlayer2Name());
					m_GameData.SetPlayer2Name("");
					m_bEliminate = false;
					std::filesystem::remove("./Assets/SavedFiles/slot_2/GameSecrets_2.lua");
				}
				else
					Logger::Err("Error, File could not be deleted");
			}

		}
		else if (transform.position.y == 392)
		{
			if (m_GameData.GetPlayer3Name().size() != 0)
			{
				// Remove file from Saved files
				const std::string save3File = "./Assets/SavedFiles/slot_3/save3.lua";
				if (std::filesystem::remove(save3File))
				{
					Logger::Log("Player: " + m_GameData.GetPlayer3Name() + " was eliminated");
					LevelLoader loader;
					loader.EliminatePlayerToDefault(3, m_GameData.GetPlayer3Name());
					m_GameData.SetPlayer3Name("");
					m_bEliminate = false;
					std::filesystem::remove("./Assets/SavedFiles/slot_3/GameSecrets_3.lua");
				}
				else
					Logger::Err("Error, File could not be deleted");
			}
		}
	}
	else if (keyboard.IsKeyJustPressed(KEY_BACKSPACE) || gamepad.IsButtonJustPressed(GP_BTN_B))
	{
		m_bEliminate = false;
	}
}
void MenuState::SelectorKeys()
{
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();
	const auto& entity = Registry::Instance().GetEntityByTag("selector");
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();

	if (keyboard.IsKeyJustPressed(KEY_W) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_UP))
	{
		transform.position.y -= sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
		if (transform.position.y < 200)
		{
			transform.position.y = game.IsDebugging() ? 776 : 584;
		}

	}
	else if (keyboard.IsKeyJustPressed(KEY_S) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_DOWN))
	{
		transform.position.y += sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX("text_slow", 0, SoundChannel::TEXT);
		if (transform.position.y > (game.IsDebugging() ? 776 : 584))
		{
			transform.position.y = 200;
		}
	}
	else if (keyboard.IsKeyJustPressed(KEY_D) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_RIGHT))
	{
	}
	else if (keyboard.IsKeyJustPressed(KEY_A) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_LEFT))
	{
	}
	else if (keyboard.IsKeyJustPressed(KEY_SPACE) || gamepad.IsButtonJustPressed(GP_BTN_A))
	{
		if (transform.position.y == 200)
		{
			if (m_GameData.GetPlayer1Name().size() != 0)
			{
				m_GameData.SetPlayerNum(1);
				game.GetStateMachine()->PushState(std::make_unique<GameState>(glm::vec2(7168, 4416)));
			}
		}
		else if (transform.position.y == 296)
		{
			if (m_GameData.GetPlayer2Name().size() != 0)
			{
				m_GameData.SetPlayerNum(2);
				game.GetStateMachine()->PushState(std::make_unique<GameState>());
			}
		}
		else if (transform.position.y == 392)
		{
			if (m_GameData.GetPlayer3Name().size() != 0)
			{
				m_GameData.SetPlayerNum(3);
				game.GetStateMachine()->PushState(std::make_unique<GameState>());
			}
		}
		else if (transform.position.y == 488)
		{
			if (m_GameData.GetPlayer1Name().size() != 0 && m_GameData.GetPlayer2Name().size() != 0 && m_GameData.GetPlayer3Name().size() != 0)
			{
				m_bSlotsFull = true;
				return;
			}

			game.GetStateMachine()->PushState(std::make_unique <NameState>());
		}
		else if (transform.position.y == 584)
		{
			if (m_GameData.GetPlayer1Name().size() != 0 || m_GameData.GetPlayer2Name().size() != 0 || m_GameData.GetPlayer3Name().size() != 0)
			{
				m_bEliminate = !m_bEliminate;
				transform.position.y = 200;
			}
		}
		else if (transform.position.y == 680)
		{
			game.GetStateMachine()->PushState(std::make_unique <SettingsState>());
		}
		else if (transform.position.y == 776)
		{
			game.GetStateMachine()->PushState(std::make_unique<EditorState>());
		}
	}
}

MenuState::MenuState()
	: slot1(1), slot2(2), slot3(3), m_bFull(false), m_bEliminate{ false }, m_bSlotsFull{false}
	, game(Game::Instance()), m_GameData(GameData::GetInstance())
	, m_InputManager(InputManager::GetInstance()), reg(Registry::Instance())
{
}

void MenuState::Update(const float& deltaTime)
{
	reg.Update();
}

void MenuState::Render()
{
	Registry::Instance().GetSystem<RenderMainMenuSystem>().Update(game.GetRenderer(), game.GetAssetManager());
}

bool MenuState::OnEnter()
{
	reg.Update();
	LevelLoader loader;
	m_bFull = false;

	if (!reg.HasSystem<RenderTextSystem>()) reg.AddSystem<RenderTextSystem>();
	if (!reg.HasSystem<RenderSystem>()) reg.AddSystem<RenderSystem>();
	if (!reg.HasSystem<RenderMainMenuSystem>()) reg.AddSystem<RenderMainMenuSystem>();

	loader.LoadAssetsFromLuaTable(lua, "menu_state_assets");
	loader.LoadMenuUIFromLuaTable(lua, "menu_state_load");
	
	if (!game.GetAssetManager()->HasFont("charriot-font-40"))
		game.GetAssetManager()->AddFonts("charriot-font-40", "./Assets/Fonts/charriot.ttf", 40);
	if (!game.GetAssetManager()->HasFont("charriot-font-60"))
		game.GetAssetManager()->AddFonts("charriot-font-60", "./Assets/Fonts/charriot.ttf", 60);

	game.GetMusicPlayer().PlayMusic("Main_Menu", -1);

	// Check to see if the save file exists, if it does call the loader function
	fs::path saves("./Assets/SavedFiles/slot_1/save1.lua");
	if (fs::status_known(fs::file_status{}) ? fs::exists(fs::file_status{}) : fs::exists(saves))
		loader.LoadMenuScreenFromLuaTable(lua, "save1", 1);
	saves = "./Assets/SavedFiles/slot_2/save2.lua";
	if (fs::status_known(fs::file_status{}) ? fs::exists(fs::file_status{}) : fs::exists(saves))
		loader.LoadMenuScreenFromLuaTable(lua, "save2", 2);
	saves = "./Assets/SavedFiles/slot_3/save3.lua";
	if (fs::status_known(fs::file_status{}) ? fs::exists(fs::file_status{}) : fs::exists(saves))
		loader.LoadMenuScreenFromLuaTable(lua, "save3", 3);
	
	auto selector = reg.GetEntityByTag("selector");
	
	if (!selector.HasComponent<KeyboardControlComponent>())
		selector.AddComponent<KeyboardControlComponent>();

	return true;
}

bool MenuState::OnExit()
{
	Registry::Instance().GetSystem<RenderMainMenuSystem>().OnExit();

	// Remove Unsued Textures
	game.GetAssetManager()->RemoveTexture("menu_box");
	game.GetAssetManager()->RemoveTexture("main_menu_gui");
	
	// Remove Unused SoundFXs
	game.GetAssetManager()->RemoveSoundFX("Eliminate");
	// Remove Fonts
	game.GetAssetManager()->RemoveFont("charriot-font-40");
	game.GetAssetManager()->RemoveFont("charriot-font-60");
	return true;
}

void MenuState::ProcessEvents(SDL_Event& event)
{
	if (!m_bEliminate)
	{
		SelectorKeys();
	}
	else
	{
		EliminateKeys();
	}
}
