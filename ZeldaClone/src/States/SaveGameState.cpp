#include "States/SaveGameState.h"
#include "States/GameStateMachine.h"
#include "ECS/ECS.h"
#include "Game/Game.h"
#include "AssetManager/AssetManager.h"
#include "Components/TextLabelComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/SaveComponent.h"
#include "Systems/PauseSystems/RenderSaveStateSystem.h"
#include "Systems/GameSystems/AnimationSystem.h"
#include "Utilities/GameData.h"
#include "inputs/InputManager.h"
#include "inputs/Keyboard.h"
#include "inputs/Gamepad.h"
#include "Game/LevelLoader.h"

const std::string SaveGameState::m_sSaveID = "SAVE";

SaveGameState::SaveGameState()
	: m_Game( Game::Instance() )
	, m_GameData( GameData::GetInstance() )
	, m_InputManager( InputManager::GetInstance() )
	, m_Registry( Registry::Instance() )
{
}

void SaveGameState::Update( const float& deltaTime )
{
	m_Registry.Update();
	m_Registry.GetSystem<AnimationSystem>().Update();
}

void SaveGameState::Render()
{
	m_Registry.GetSystem<RenderSaveStateSystem>().Update( m_Game.GetRenderer(), m_Game.GetAssetManager() );
}

bool SaveGameState::OnEnter()
{
	m_Registry.AddSystem<RenderSaveStateSystem>();
	m_Game.GetAssetManager()->AddTextures( m_Game.GetRenderer(), "save_gui", "./assets/HUDSprites/save_state_GUI.png" );

	Entity saveText = m_Registry.CreateEntity();
	saveText.AddComponent<TransformComponent>( glm::vec2( 200, 200 ), glm::vec2( 4, 4 ), 0.0 );
	saveText.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "save_gui",
															 .width = 144,
															 .height = 16,
															 .layer = 0,
															 .bIsFixed = true,
															 .srcRect = SDL_Rect{ 0, 0, 144, 16 } } );

	saveText.AddComponent<SaveComponent>();

	Entity exitText = m_Registry.CreateEntity();
	exitText.AddComponent<TransformComponent>( glm::vec2( 200, 400 ), glm::vec2( 4, 4 ), 0.0 );
	exitText.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "save_gui",
															 .width = 192,
															 .height = 16,
															 .layer = 0,
															 .bIsFixed = true,
															 .srcRect = SDL_Rect{ 0, 16, 192, 16 } } );

	exitText.AddComponent<SaveComponent>();

	Entity quitText = m_Registry.CreateEntity();
	quitText.AddComponent<TransformComponent>( glm::vec2( 200, 600 ), glm::vec2( 4, 4 ), 0.0 );
	quitText.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "save_gui",
															 .width = 144,
															 .height = 16,
															 .layer = 0,
															 .bIsFixed = true,
															 .srcRect = SDL_Rect{ 0, 32, 144, 16 } } );

	quitText.AddComponent<SaveComponent>();

	Entity selector = m_Registry.CreateEntity();
	selector.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "hud_hearts",
															 .width = 16,
															 .height = 16,
															 .layer = 5,
															 .bIsFixed = true,
															 .srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

	selector.AddComponent<TransformComponent>( glm::vec2( 100, 185 ), glm::vec2( 6, 6 ), 0.0 );
	selector.AddComponent<KeyboardControlComponent>();
	selector.AddComponent<SaveComponent>();
	selector.Tag( "save_selector" );
	return true;
}

bool SaveGameState::OnExit()
{
	return true;
}

void SaveGameState::ProcessEvents( SDL_Event& event )
{
	const auto& selector = m_Registry.GetEntityByTag( "save_selector" );
	auto& transform = selector.GetComponent<TransformComponent>();
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& m_Gamepad = m_InputManager.GetGamepad();

	if ( keyboard.IsKeyJustPressed( KEY_W ) || m_Gamepad.IsButtonJustPressed( GP_BTN_DPAD_UP ) )
	{
		transform.position.y -= 200;
		if ( transform.position.y < 185 )
			transform.position.y = 585;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_S ) || m_Gamepad.IsButtonJustPressed( GP_BTN_DPAD_DOWN ) )
	{
		transform.position.y += 200;
		if ( transform.position.y > 585 )
			transform.position.y = 185;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_SPACE ) || m_Gamepad.IsButtonJustPressed( GP_BTN_A ) )
	{
		if ( transform.position.y == 185 )
		{
			LevelLoader loader;

			/*if (m_Game.GetPlayerNum() == 1)
			{
				loader.SavePlayerDataToLuaTable("1");
			}
			else if (m_Game.GetPlayerNum() == 2)
			{
				loader.SavePlayerDataToLuaTable("2");
			}
			else if (m_Game.GetPlayerNum() == 3)
			{
				loader.SavePlayerDataToLuaTable("3");
			}*/

			loader.SavePlayerDataToLuaTable( std::to_string( m_GameData.PlayerNum() ) );
			loader.SaveSecrets();
			m_Game.GetStateMachine()->PopState();
		}
		else if ( transform.position.y == 385 )
		{
			// State::exitToMain = true;
			// m_Game.GetStateMachine()->ClearStates();
			// m_Game.GetStateMachine()->PushState(std::make_unique<MenuState>());
		}
		else if ( transform.position.y == 585 )
		{
			m_Game.SetGameRunning( false );
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_BACKSPACE ) || m_Gamepad.IsButtonJustPressed( GP_BTN_B ) )
	{
		m_Game.GetStateMachine()->PopState();
	}
}
