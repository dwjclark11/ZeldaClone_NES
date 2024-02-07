#include "SaveGameState.h"
#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/SaveComponent.h"
#include "../Systems/PauseSystems/RenderSaveStateSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"
#include "../Utilities/GameData.h"
#include "../inputs/InputManager.h"
#include "../inputs/Keyboard.h"
#include "../inputs/Gamepad.h"
#include "../Game/LevelLoader.h"

const std::string SaveGameState::saveID = "SAVE";

SaveGameState::SaveGameState()
	: game(Game::Instance()), m_GameData(GameData::GetInstance())
	, m_InputManager(InputManager::GetInstance()), reg(Registry::Instance())
{

}

void SaveGameState::Update(const float& deltaTime)
{
	reg.Update();
	reg.GetSystem<AnimationSystem>().Update();
}

void SaveGameState::Render()
{
	reg.GetSystem<RenderSaveStateSystem>().Update(game.GetRenderer(), game.GetAssetManager());
}

bool SaveGameState::OnEnter()
{	
	reg.AddSystem<RenderSaveStateSystem>();
	game.GetAssetManager()->AddTextures(game.GetRenderer(), "save_gui", "./Assets/HUDSprites/save_state_GUI.png");


	Entity saveText = reg.CreateEntity();
	saveText.AddComponent<TransformComponent>(glm::vec2(200, 200), glm::vec2(4, 4), 0.0);
	saveText.AddComponent<SpriteComponent>(
		SpriteComponent{
			.assetID = "save_gui",
			.width = 144,
			.height = 16,
			.layer = 0,
			.isFixed = true,
			.srcRect = SDL_Rect{0, 0, 144, 16} 
		}
	);

	saveText.AddComponent<SaveComponent>();
	
	Entity exitText = reg.CreateEntity();
	exitText.AddComponent<TransformComponent>(glm::vec2(200, 400), glm::vec2(4, 4), 0.0);
	exitText.AddComponent<SpriteComponent>(
		SpriteComponent{
			.assetID = "save_gui",
			.width = 192,
			.height = 16,
			.layer = 0,
			.isFixed = true,
			.srcRect = SDL_Rect{0, 16, 192, 16} 
		}
	);

	exitText.AddComponent<SaveComponent>();

	Entity quitText = reg.CreateEntity();
	quitText.AddComponent<TransformComponent>(glm::vec2(200, 600), glm::vec2(4, 4), 0.0);
	quitText.AddComponent<SpriteComponent>(
		SpriteComponent{
			.assetID = "save_gui",
			.width = 144,
			.height = 16,
			.layer = 0,
			.isFixed = true,
			.srcRect = SDL_Rect{0, 32, 144, 16} 
		}
	);

	quitText.AddComponent<SaveComponent>();

	Entity selector = reg.CreateEntity();
	selector.AddComponent<SpriteComponent>(
		SpriteComponent{
			.assetID = "hud_hearts",
			.width = 16,
			.height = 16,
			.layer = 5,
			.isFixed = true,
			.srcRect = SDL_Rect {0, 0, 16, 16} 
		}
	);

	selector.AddComponent<TransformComponent>(glm::vec2(100, 185), glm::vec2(6, 6), 0.0);
	selector.AddComponent<KeyboardControlComponent>();
	selector.AddComponent<SaveComponent>();
	selector.Tag("save_selector");
	return true;
}

bool SaveGameState::OnExit()
{
	return true;
}

void SaveGameState::ProcessEvents(SDL_Event& event)
{
	const auto& selector = reg.GetEntityByTag("save_selector");
	auto& transform = selector.GetComponent<TransformComponent>();
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();

	if (keyboard.IsKeyJustPressed(KEY_W) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_UP))
	{
		transform.position.y -= 200;
		if (transform.position.y < 185) 
			transform.position.y = 585;
	}
	else if (keyboard.IsKeyJustPressed(KEY_S) || gamepad.IsButtonJustPressed(GP_BTN_DPAD_DOWN))
	{
		transform.position.y += 200;
		if (transform.position.y > 585) 
			transform.position.y = 185;
	}
	else if (keyboard.IsKeyJustPressed(KEY_SPACE) || gamepad.IsButtonJustPressed(GP_BTN_A))
	{
		if (transform.position.y == 185)
		{
			LevelLoader loader;

			/*if (game.GetPlayerNum() == 1)
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
			}*/

			loader.SavePlayerDataToLuaTable(std::to_string(m_GameData.PlayerNum()));
			loader.SaveSecrets();
			game.GetStateMachine()->PopState();
		}
		else if (transform.position.y == 385)
		{
			//State::exitToMain = true;
			//game.GetStateMachine()->ClearStates();
			//game.GetStateMachine()->PushState(std::make_unique<MenuState>());
		}
		else if (transform.position.y == 585)
		{
			game.SetGameRunning(false);
		}
	}
	else if (keyboard.IsKeyJustPressed(KEY_BACKSPACE) || gamepad.IsButtonJustPressed(GP_BTN_B))
	{
		game.GetStateMachine()->PopState();
	}
}
