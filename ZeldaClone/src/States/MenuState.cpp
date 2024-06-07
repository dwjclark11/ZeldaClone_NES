#include "States/MenuState.h"
#include "States/NameState.h"
#include "States/SettingsState.h"
#include "States/GameState.h"
#include "States/EditorState.h"
#include "States/GameStateMachine.h"
#include "Game/Game.h"
#include "Utilities/GameData.h"
#include "inputs/InputManager.h"
#include "inputs/Keyboard.h"
#include "inputs/Gamepad.h"

#include "Game/LevelLoader.h"
#include "Components/TextLabelComponent.h"
#include "Components/MenuComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/MenuSystems/RenderMainMenuSystem.h"
#include "Systems/EditorSystems/MouseControlSystem.h"
#include "Systems/EditorSystems/RenderEditorGUISystem.h"
#include "Systems/EditorSystems/RenderEditorSystem.h"

#include "Systems/GameSystems/RenderSystem.h"
#include "Systems/GameSystems/DamageSystem.h"
#include "Systems/GameSystems/RenderHUDSystem.h"
#include "Systems/GameSystems/MovementSystem.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/GameSystems/CaptionSystem.h"
#include "Systems/GameSystems/RenderHealthSystem.h"
#include "Systems/GameSystems/RenderTileSystem.h"
#include "Systems/GameSystems/RenderGameOverTextSystem.h"
#include "Systems/GameSystems/RenderGameOverSystem.h"
#include "Systems/GameSystems/HealthSystem.h"
#include "Systems/GameSystems/TriggerSystem.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/GameSystems/CollectItemSystem.h"
#include "Systems/GameSystems/ProjectileEmitterSystem.h"
#include "Systems/GameSystems/ProjectileLifeCycleSystem.h"
#include "Systems/GameSystems/AISystem.h"
#include "Systems/GameSystems/CollisionSystem.h"
#include "Systems/MenuSystems/RenderMainMenuSystem.h"

#include <filesystem>
namespace fs = std::filesystem;

// Define all static variables
const std::string MenuState::menuID = "MENU";

void MenuState::EliminateKeys()
{
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();
	const auto& entity = Registry::Instance().GetEntityByTag( "selector" );
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();

	if ( keyboard.IsKeyJustPressed( KEY_W ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_UP ) )
	{
		transform.position.y -= sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );

		if ( transform.position.y < 200 )
			transform.position.y = 392;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_S ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_DOWN ) )
	{
		transform.position.y += sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );

		if ( transform.position.y > 392 )
			transform.position.y = 200;
	}
	else if ( keyboard.IsKeyJustPressed( KEY_SPACE ) || gamepad.IsButtonJustPressed( GP_BTN_A ) )
	{
		if ( transform.position.y == 200 )
		{

			if ( m_GameData.GetPlayer1Name().size() != 0 )
			{
				// Remove file from Saved files
				const std::string save1File = "./assets/SavedFiles/slot_1/save1.lua";
				if ( std::filesystem::remove( save1File ) )
				{
					LevelLoader loader;
					loader.EliminatePlayerToDefault( 1, m_GameData.GetPlayer1Name() );
					m_GameData.SetPlayer1Name( "" );
					m_bEliminate = false;
					std::filesystem::remove( "./assets/SavedFiles/slot_1/GameSecrets_1.lua" );
				}
				else
					Logger::Err( "Error, File could not be deleted" );
			}
		}
		else if ( transform.position.y == 296 )
		{

			if ( m_GameData.GetPlayer2Name().size() != 0 )
			{
				// Remove file from Saved files
				const std::string save2File = "./assets/SavedFiles/slot_2/save2.lua";
				if ( std::filesystem::remove( save2File ) )
				{
					LevelLoader loader;
					loader.EliminatePlayerToDefault( 2, m_GameData.GetPlayer2Name() );
					m_GameData.SetPlayer2Name( "" );
					m_bEliminate = false;
					std::filesystem::remove( "./assets/SavedFiles/slot_2/GameSecrets_2.lua" );
				}
				else
					Logger::Err( "Error, File could not be deleted" );
			}
		}
		else if ( transform.position.y == 392 )
		{
			if ( m_GameData.GetPlayer3Name().size() != 0 )
			{
				// Remove file from Saved files
				const std::string save3File = "./assets/SavedFiles/slot_3/save3.lua";
				if ( std::filesystem::remove( save3File ) )
				{
					LevelLoader loader;
					loader.EliminatePlayerToDefault( 3, m_GameData.GetPlayer3Name() );
					m_GameData.SetPlayer3Name( "" );
					m_bEliminate = false;
					std::filesystem::remove( "./assets/SavedFiles/slot_3/GameSecrets_3.lua" );
				}
				else
					Logger::Err( "Error, File could not be deleted" );
			}
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_BACKSPACE ) || gamepad.IsButtonJustPressed( GP_BTN_B ) )
	{
		m_bEliminate = false;
	}
}
void MenuState::SelectorKeys()
{
	auto& keyboard = m_InputManager.GetKeyboard();
	auto& gamepad = m_InputManager.GetGamepad();
	const auto& entity = Registry::Instance().GetEntityByTag( "selector" );
	auto& sprite = entity.GetComponent<SpriteComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();

	if ( keyboard.IsKeyJustPressed( KEY_W ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_UP ) )
	{
		transform.position.y -= sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		if ( transform.position.y < 200 )
		{
			transform.position.y = game.IsDebugging() ? 776 : 584;
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_S ) || gamepad.IsButtonJustPressed( GP_BTN_DPAD_DOWN ) )
	{
		transform.position.y += sprite.height * 6;
		game.GetSoundPlayer().PlaySoundFX( "text_slow", 0, SoundChannel::TEXT );
		if ( transform.position.y > ( game.IsDebugging() ? 776 : 584 ) )
		{
			transform.position.y = 200;
		}
	}
	else if ( keyboard.IsKeyJustPressed( KEY_SPACE ) || gamepad.IsButtonJustPressed( GP_BTN_A ) )
	{
		if ( transform.position.y == 200 )
		{
			if ( m_GameData.GetPlayer1Name().size() != 0 )
			{
				m_GameData.SetPlayerNum( 1 );
				game.GetStateMachine()->PushState( std::make_unique<GameState>( glm::vec2( 7168, 4416 ) ) );
			}
		}
		else if ( transform.position.y == 296 )
		{
			if ( m_GameData.GetPlayer2Name().size() != 0 )
			{
				m_GameData.SetPlayerNum( 2 );
				game.GetStateMachine()->PushState( std::make_unique<GameState>() );
			}
		}
		else if ( transform.position.y == 392 )
		{
			if ( m_GameData.GetPlayer3Name().size() != 0 )
			{
				m_GameData.SetPlayerNum( 3 );
				game.GetStateMachine()->PushState( std::make_unique<GameState>() );
			}
		}
		else if ( transform.position.y == 488 )
		{
			if ( m_GameData.GetPlayer1Name().size() != 0 && m_GameData.GetPlayer2Name().size() != 0 &&
				 m_GameData.GetPlayer3Name().size() != 0 )
			{
				m_bSlotsFull = true;
				return;
			}

			game.GetStateMachine()->PushState( std::make_unique<NameState>() );
		}
		else if ( transform.position.y == 584 )
		{
			if ( m_GameData.GetPlayer1Name().size() != 0 || m_GameData.GetPlayer2Name().size() != 0 ||
				 m_GameData.GetPlayer3Name().size() != 0 )
			{
				m_bEliminate = !m_bEliminate;
				transform.position.y = 200;
			}
		}
		else if ( transform.position.y == 680 )
		{
			game.GetStateMachine()->PushState( std::make_unique<SettingsState>() );
		}
		else if ( transform.position.y == 776 )
		{
			game.GetStateMachine()->PushState( std::make_unique<EditorState>() );
		}
	}
}

MenuState::MenuState()
	: slot1( 1 )
	, slot2( 2 )
	, slot3( 3 )
	, m_bFull( false )
	, m_bEliminate{ false }
	, m_bSlotsFull{ false }
	, game( Game::Instance() )
	, m_GameData( GameData::GetInstance() )
	, m_InputManager( InputManager::GetInstance() )
	, reg( Registry::Instance() )
{
}

void MenuState::Update( const float& deltaTime )
{
	reg.Update();
}

void MenuState::Render()
{
	Registry::Instance().GetSystem<RenderMainMenuSystem>().Update( game.GetRenderer(), game.GetAssetManager() );
}

bool MenuState::OnEnter()
{
	reg.Update();
	LevelLoader loader;
	m_bFull = false;

	// ===============================================================================================
	// Add all necessary systems to the registry if they are not yet registered
	// ===============================================================================================
	reg.AddSystem<RenderTextSystem>();
	reg.AddSystem<RenderSystem>();
	reg.AddSystem<RenderMainMenuSystem>();

	reg.AddSystem<RenderHUDSystem>();
	reg.AddSystem<ProjectileEmitterSystem>();
	reg.AddSystem<ProjectileLifeCycleSystem>();
	reg.AddSystem<DamageSystem>();
	reg.AddSystem<RenderHealthSystem>();
	reg.AddSystem<RenderTileSystem>();
	reg.AddSystem<HealthSystem>();
	reg.AddSystem<CollisionSystem>( game.GetEventManager() );
	reg.AddSystem<MovementSystem>();
	reg.AddSystem<TriggerSystem>();
	reg.AddSystem<CollectItemSystem>();

	reg.AddSystem<RenderTextSystem>();
	reg.AddSystem<AISystem>();
	reg.AddSystem<CaptionSystem>();
	// =================================================================================================

	loader.LoadAssetsFromLuaTable( lua, "menu_state_assets" );
	loader.LoadMenuUIFromLuaTable( lua, "menu_state_load" );

	if ( !game.GetAssetManager()->HasFont( "charriot-font-40" ) )
		game.GetAssetManager()->AddFonts( "charriot-font-40", "./assets/Fonts/charriot.ttf", 40 );
	if ( !game.GetAssetManager()->HasFont( "charriot-font-60" ) )
		game.GetAssetManager()->AddFonts( "charriot-font-60", "./assets/Fonts/charriot.ttf", 60 );

	game.GetMusicPlayer().PlayMusic( "Main_Menu", -1 );

	// Check to see if the save file exists, if it does call the loader function
	fs::path saves( "./assets/SavedFiles/slot_1/save1.lua" );
	if ( fs::status_known( fs::file_status{} ) ? fs::exists( fs::file_status{} ) : fs::exists( saves ) )
		loader.LoadMenuScreenFromLuaTable( lua, "save1", 1 );
	saves = "./assets/SavedFiles/slot_2/save2.lua";
	if ( fs::status_known( fs::file_status{} ) ? fs::exists( fs::file_status{} ) : fs::exists( saves ) )
		loader.LoadMenuScreenFromLuaTable( lua, "save2", 2 );
	saves = "./assets/SavedFiles/slot_3/save3.lua";
	if ( fs::status_known( fs::file_status{} ) ? fs::exists( fs::file_status{} ) : fs::exists( saves ) )
		loader.LoadMenuScreenFromLuaTable( lua, "save3", 3 );

	auto selector = reg.GetEntityByTag( "selector" );

	if ( !selector.HasComponent<KeyboardControlComponent>() )
		selector.AddComponent<KeyboardControlComponent>();

	return true;
}

bool MenuState::OnExit()
{
	Registry::Instance().GetSystem<RenderMainMenuSystem>().OnExit();

	// Remove Unsued Textures
	game.GetAssetManager()->RemoveTexture( "menu_box" );
	game.GetAssetManager()->RemoveTexture( "main_menu_gui" );

	// Remove Unused SoundFXs
	game.GetAssetManager()->RemoveSoundFX( "Eliminate" );
	// Remove Fonts
	game.GetAssetManager()->RemoveFont( "charriot-font-40" );
	game.GetAssetManager()->RemoveFont( "charriot-font-60" );

	return true;
}

void MenuState::ProcessEvents( SDL_Event& event )
{
	if ( !m_bEliminate )
	{
		SelectorKeys();
	}
	else
	{
		EliminateKeys();
	}
}
