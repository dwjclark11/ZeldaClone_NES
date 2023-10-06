#include "EditorState.h"
#include "../ECS/ECS.h"
#include "../Systems/EditorSystems/RenderEditorGUISystem.h"
#include "../Systems/EditorSystems/RenderEditorSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Game/Game.h"
#include "../Game/LevelLoader.h"
#include "../Systems/EditorSystems/MouseControlSystem.h"
#include "../Utilities/Camera.h"
#include "../Utilities/GameData.h"
#include "../inputs/InputManager.h"
#include "../inputs/Keyboard.h"
#include "../inputs/Gamepad.h"

const std::string EditorState::editorID = "EDITOR";

EditorState::EditorState()
	: editor(false), keyDown(false), game(Game::Instance()), gameData(GameData::GetInstance())
	, inputManager(InputManager::GetInstance()), reg(Registry::Instance())
{

}

void EditorState::Update(const float& deltaTime)
{
	reg.Update();
	Registry::Instance().GetSystem<CameraMovementSystem>().UpdateEditorCam(game.GetCamera(), deltaTime);
}

void EditorState::Render()
{
	const auto& camera = game.GetCamera();
	//// Create the HUD rect that each tilemap screen will be inside
	SDL_Rect hudRectTop = { 0, -1080 - camera.GetCameraPos().y, camera.GetCameraWidth(), 1080};
	SDL_Rect hudRectLeft = { -camera.GetCameraWidth() - camera.GetCameraPos().x, 0, camera.GetCameraWidth(), 1080};
	SDL_Rect panelRect = {448, 348, PANEL_WIDTH, PANEL_HEIGHT};
	SDL_Rect hudRect = { 448, 60, PANEL_WIDTH, HUD_HEIGHT };
	SDL_SetRenderDrawColor(game.GetRenderer(), 70, 70, 70, 70);
	int xNum = MouseControlSystem::CanvasWidth / MouseControlSystem::gridSize;
	int yNum = MouseControlSystem::CanvasHeight / MouseControlSystem::gridSize;

	for (int i = 0; i < yNum; i++)
		for (int j = 0; j < xNum; j++)
		{
			SDL_Rect newRect = { (j * MouseControlSystem::gridSize) - camera.GetCameraPos().x, i * MouseControlSystem::gridSize - camera.GetCameraPos().y, 64, 64};
			SDL_RenderDrawRect(game.GetRenderer(), &newRect);
		}

	Registry::Instance().GetSystem<RenderEditorSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera().GetCameraRect());
	Registry::Instance().GetSystem<MouseControlSystem>().Update(game.GetAssetManager(), game.GetRenderer(),
		game.GetMouseBox(), game.GetEvent(), game.GetCamera().GetCameraRect());
	Registry::Instance().GetSystem<RenderCollisionSystem>().Update(game.GetRenderer(), game.GetCamera().GetCameraRect());
	Registry::Instance().GetSystem<RenderEditorGUISystem>().Update(game.GetAssetManager(), game.GetRenderer());

	// Draw The Editor Boundries
	SDL_SetRenderDrawColor(game.GetRenderer(), 70, 70, 70, 70);
	SDL_RenderFillRect(game.GetRenderer(), &hudRectLeft);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRectLeft);
	SDL_RenderFillRect(game.GetRenderer(), &hudRectTop);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRectTop);

	SDL_SetRenderDrawColor(game.GetRenderer(), 225, 225, 0, 100);
	SDL_RenderFillRect(game.GetRenderer(), &panelRect);
	SDL_RenderDrawRect(game.GetRenderer(), &panelRect);
	SDL_SetRenderDrawColor(game.GetRenderer(), 255, 255, 255, 100);
	SDL_RenderFillRect(game.GetRenderer(), &hudRect);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRect);
}

bool EditorState::OnEnter()
{
	LevelLoader loader;
	// Stop any music that my be playing
	Mix_HaltMusic();
	
	// Set the desired window size/position for the editor
	SDL_SetWindowSize(game.GetWindow(), 1920, 1080);
	SDL_SetWindowPosition(game.GetWindow(), 0, 0);
	
	// Change the title to the Editor Title
	SDL_SetWindowTitle(game.GetWindow(), "Zelda Quest Editor");
	// We want the editor to be resizable!
	SDL_SetWindowResizable(game.GetWindow(), SDL_TRUE);
	
	// Set the Camera Position
	game.GetCamera().SetCameraPosition(0, 0);
	game.GetCamera().SetCameraWidth(1920);
	game.GetCamera().SetCameraHeight(1080);

	// Initialize IMGUI context
	ImGui::CreateContext();
	ImGuiSDL::Initialize(game.GetRenderer(), 1920, 1080);

	// If the needed systems are not already in the regstry, Add them
	if (!reg.HasSystem<RenderCollisionSystem>()) reg.AddSystem<RenderCollisionSystem>();
	if (!reg.HasSystem<RenderEditorGUISystem>()) reg.AddSystem<RenderEditorGUISystem>();
	if (!reg.HasSystem<RenderEditorSystem>()) 	reg.AddSystem<RenderEditorSystem>();
	if (!reg.HasSystem<MouseControlSystem>()) 	reg.AddSystem<MouseControlSystem>();
	
	loader.LoadAssetsFromLuaTable(game.GetLuaState(), "editor_assets");
	// Assign values to varialbes
	editor = false;
	keyDown = false;
	return true;
}

bool EditorState::OnExit()
{
	// Delete the editor entities
	Registry::Instance().GetSystem<RenderEditorSystem>().OnExit();

	// Set the Camera Position
	game.GetCamera().SetCameraWidth(game.GetWindowWidth());
	game.GetCamera().SetCameraHeight(game.GetWindowHeight());

	// Set the Window Size/Position to the desired Game Window Size and position
	SDL_SetWindowSize(game.GetWindow(), 256 * 4, 240 * 4);
	SDL_SetWindowPosition(game.GetWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	// Change the title to the Game Title
	SDL_SetWindowTitle(game.GetWindow(), "Zelda Clone");
	// We do not want the Game to be resizable!
	SDL_SetWindowResizable(game.GetWindow(), SDL_FALSE);
	
	if (!game.IsDebugging()) 
		game.SetDebug(true);

	return true;
}

void EditorState::ProcessEvents(SDL_Event& event)
{
	auto& camera = game.GetCamera();
	const auto& camera_pos = camera.GetCameraPos();
	auto& keyboard = inputManager.GetKeyboard();

		// Move the camera UP
	if (keyboard.IsKeyHeld(KEY_W))
	{
		camera.SetCameraPosition(camera_pos.x, camera_pos.y - 32);
	}
	else if (keyboard.IsKeyHeld(KEY_D))
	{
		camera.SetCameraPosition(camera_pos.x + 64, camera_pos.y);
	}
	else if (keyboard.IsKeyHeld(KEY_S))
	{
		camera.SetCameraPosition(camera_pos.x, camera_pos.y + 32);
	}
	else if (keyboard.IsKeyHeld(KEY_A))
	{
		camera.SetCameraPosition(camera_pos.x - 64, camera_pos.y);
	}
		
	// Clamps for Game Camera and Image Src Rec!! --> This needs to be investigated further
	if (camera.GetCameraPos().x < -1000)
		camera.SetCameraPosition(-1000, camera_pos.y);
	if (camera.GetCameraPos().y < -1000)
		camera.SetCameraPosition(camera_pos.x, -1000);;
}

