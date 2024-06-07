#include "States/EditorState.h"
#include "ECS/ECS.h"
#include "Systems/EditorSystems/RenderEditorGUISystem.h"
#include "Systems/EditorSystems/RenderEditorSystem.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/GameSystems/AnimationSystem.h"
#include "Systems/RenderCollisionSystem.h"
#include "Game/Game.h"
#include "Game/LevelLoader.h"
#include "Systems/EditorSystems/MouseControlSystem.h"
#include "Utilities/Camera.h"
#include "Utilities/GameData.h"
#include "inputs/InputManager.h"
#include "inputs/Keyboard.h"
#include "inputs/Gamepad.h"

EditorState::EditorState()
	: m_Game{ Game::Instance() }
	, m_GameData{ GameData::GetInstance() }
	, m_pMouseControl{ std::make_unique<MouseControlSystem>() }
	, m_InputManager{ InputManager::GetInstance() }
	, m_Registry{ Registry::Instance() }
	, m_Loader{ *m_pMouseControl }
	, m_bEditor{ false }
	, m_bKeyDown{ false }
{
}

void EditorState::Update( const float& deltaTime )
{
	m_Registry.Update();
	Registry::Instance().GetSystem<CameraMovementSystem>().UpdateEditorCam( m_Game.GetCamera(), deltaTime );
}

void EditorState::Render()
{
	const auto& camera = m_Game.GetCamera();
	// Create the HUD rect that each tilemap screen will be inside
	SDL_Rect hudRectTop = { 0, -1080 - camera.GetCameraPos().y, camera.GetCameraWidth(), 1080 };
	SDL_Rect hudRectLeft = { -camera.GetCameraWidth() - camera.GetCameraPos().x, 0, camera.GetCameraWidth(), 1080 };
	SDL_Rect panelRect = { 448, 348, PANEL_WIDTH, PANEL_HEIGHT };
	SDL_Rect hudRect = { 448, 60, PANEL_WIDTH, HUD_HEIGHT };
	SDL_SetRenderDrawColor( m_Game.GetRenderer(), 70, 70, 70, 70 );

	int gridSize{ m_pMouseControl->GridSize() == 0 ? 16 : m_pMouseControl->GridSize() };
	int xNum{ m_pMouseControl->GetCanvasWidth() / gridSize };
	int yNum{ m_pMouseControl->GetCanvasHeight() / gridSize };

	for ( int i = 0; i < yNum; i++ )
	{
		for ( int j = 0; j < xNum; j++ )
		{
			SDL_Rect newRect{
				( j * gridSize ) - camera.GetCameraPos().x, ( i * gridSize ) - camera.GetCameraPos().y, 64, 64 };

			SDL_RenderDrawRect( m_Game.GetRenderer(), &newRect );
		}
	}

	Registry::Instance().GetSystem<RenderEditorSystem>().Update(
		m_Game.GetRenderer(), m_Game.GetAssetManager(), m_Game.GetCamera().GetCameraRect() );

	m_pMouseControl->Update( m_Game.GetAssetManager(),
							 m_Game.GetRenderer(),
							 m_Game.GetMouseBox(),
							 m_Game.GetEvent(),
							 m_Game.GetCamera().GetCameraRect() );

	Registry::Instance().GetSystem<RenderCollisionSystem>().Update( m_Game.GetRenderer(),
																	m_Game.GetCamera().GetCameraRect() );

	Registry::Instance().GetSystem<RenderEditorGUISystem>().Update( m_Game.GetAssetManager(), m_Game.GetRenderer() );

	// Draw The Editor Boundries -- HACK NEEDED FOR GHOSTING
	SDL_SetRenderDrawColor( m_Game.GetRenderer(), 70, 70, 70, 0 );
	SDL_RenderFillRect( m_Game.GetRenderer(), &hudRectLeft );
	SDL_RenderDrawRect( m_Game.GetRenderer(), &hudRectLeft );
	SDL_RenderFillRect( m_Game.GetRenderer(), &hudRectTop );
	SDL_RenderDrawRect( m_Game.GetRenderer(), &hudRectTop );

	SDL_SetRenderDrawColor( m_Game.GetRenderer(), 225, 225, 0, 0 );
	SDL_RenderFillRect( m_Game.GetRenderer(), &panelRect );
	SDL_RenderDrawRect( m_Game.GetRenderer(), &panelRect );
	SDL_SetRenderDrawColor( m_Game.GetRenderer(), 255, 255, 255, 0 );
	SDL_RenderFillRect( m_Game.GetRenderer(), &hudRect );
	SDL_RenderDrawRect( m_Game.GetRenderer(), &hudRect );
}

bool EditorState::OnEnter()
{
	LevelLoader loader;
	// Stop any music that my be playing
	Mix_HaltMusic();

	// Set the desired window size/position for the m_bEditor
	SDL_SetWindowSize( m_Game.GetWindow(), 1920, 1080 );
	SDL_SetWindowPosition( m_Game.GetWindow(), 0, 0 );

	// Change the title to the Editor Title
	SDL_SetWindowTitle( m_Game.GetWindow(), "Zelda Quest Editor" );
	// We want the m_bEditor to be resizable!
	SDL_SetWindowResizable( m_Game.GetWindow(), SDL_TRUE );

	// Set the Camera Position
	m_Game.GetCamera().SetCameraPosition( 0, 0 );
	m_Game.GetCamera().SetCameraWidth( 1920 );
	m_Game.GetCamera().SetCameraHeight( 1080 );

	// Initialize IMGUI context
	ImGui::CreateContext();
	ImGuiSDL::Initialize( m_Game.GetRenderer(), 1920, 1080 );

	// If the needed systems are not already in the regstry, Add them
	if ( !m_Registry.HasSystem<RenderCollisionSystem>() )
		m_Registry.AddSystem<RenderCollisionSystem>();

	m_Registry.AddSystem<RenderEditorGUISystem>( *m_pMouseControl );

	if ( !m_Registry.HasSystem<RenderEditorSystem>() )
		m_Registry.AddSystem<RenderEditorSystem>();

	loader.LoadAssetsFromLuaTable( m_Game.GetLuaState(), "editor_assets" );
	// Assign values to varialbes
	m_bEditor = false;
	m_bKeyDown = false;
	return true;
}

bool EditorState::OnExit()
{
	// Delete the m_bEditor entities
	Registry::Instance().GetSystem<RenderEditorSystem>().OnExit();

	// Set the Camera Position
	m_Game.GetCamera().SetCameraWidth( m_Game.GetWindowWidth() );
	m_Game.GetCamera().SetCameraHeight( m_Game.GetWindowHeight() );

	// Set the Window Size/Position to the desired Game Window Size and position
	SDL_SetWindowSize( m_Game.GetWindow(), 256 * 4, 240 * 4 );
	SDL_SetWindowPosition( m_Game.GetWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
	// Change the title to the Game Title
	SDL_SetWindowTitle( m_Game.GetWindow(), "Zelda Clone" );
	// We do not want the Game to be resizable!
	SDL_SetWindowResizable( m_Game.GetWindow(), SDL_FALSE );

	if ( !m_Game.IsDebugging() )
		m_Game.SetDebug( true );

	return true;
}

void EditorState::ProcessEvents( SDL_Event& event )
{
	auto& camera = m_Game.GetCamera();
	const auto& camera_pos = camera.GetCameraPos();
	auto& keyboard = m_InputManager.GetKeyboard();

	// Move the camera UP
	if ( keyboard.IsKeyHeld( KEY_W ) )
	{
		camera.SetCameraPosition( camera_pos.x, camera_pos.y - 32 );
	}
	else if ( keyboard.IsKeyHeld( KEY_D ) )
	{
		camera.SetCameraPosition( camera_pos.x + 64, camera_pos.y );
	}
	else if ( keyboard.IsKeyHeld( KEY_S ) )
	{
		camera.SetCameraPosition( camera_pos.x, camera_pos.y + 32 );
	}
	else if ( keyboard.IsKeyHeld( KEY_A ) )
	{
		camera.SetCameraPosition( camera_pos.x - 64, camera_pos.y );
	}

	// Clamps for Game Camera and Image Src Rec!! --> This needs to be investigated further
	if ( camera.GetCameraPos().x < -1000 )
		camera.SetCameraPosition( -1000, camera_pos.y );
	if ( camera.GetCameraPos().y < -1000 )
		camera.SetCameraPosition( camera_pos.x, -1000 );
	;
}
