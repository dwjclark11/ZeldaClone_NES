#include "EditorState.h"
#include "../ECS/ECS.h"
#include "../Systems/EditorSystems/RenderEditorGUISystem.h"
#include "../Systems/EditorSystems/RenderEditorSystem.h"
#include "../Systems/EditorSystems/EditorKeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Systems/EditorSystems/RenderEditorLabelSystem.h"
#include "../Game/Game.h"
#include "../Game/LevelLoader.h"
#include "../Systems/EditorSystems/MouseControlSystem.h"

const std::string EditorState::editorID = "EDITOR";

EditorState::EditorState()
	: editor(false), keyDown(false), game(*Game::Instance()), reg(*Registry::Instance())
{

}

void EditorState::Update(const double& deltaTime)
{
	game.GetEventManager()->Reset();
	reg.GetSystem<EditorKeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());

	reg.Update();
	reg.GetSystem<CameraMovementSystem>().Update(game.GetCamera());
	//reg.GetSystem<AnimationSystem>().Update();
}

void EditorState::Render()
{
	//// Create the HUD rect that each tilemap screen will be inside
	SDL_Rect hudRectTop = { 0, -1080 - game.GetCamera().y, game.GetCamera().w, 1080 };
	SDL_Rect hudRectLeft = { -game.GetCamera().w - game.GetCamera().x, 0, game.GetCamera().w, 1080 };
	
	SDL_SetRenderDrawColor(game.GetRenderer(), 70, 70, 70, 70);
	int xNum = MouseControlSystem::CanvasWidth / MouseControlSystem::gridSize;
	int yNum = MouseControlSystem::CanvasHeight / MouseControlSystem::gridSize;

	for (int i = 0; i < yNum; i++)
		for (int j = 0; j < xNum; j++)
		{
			SDL_Rect newRect = { (j * MouseControlSystem::gridSize) - game.GetCamera().x, i * MouseControlSystem::gridSize - game.GetCamera().y, 64, 64 };
			SDL_RenderDrawRect(game.GetRenderer(), &newRect);
		}

	reg.GetSystem<RenderEditorSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	reg.GetSystem<MouseControlSystem>().Update(game.GetAssetManager(), game.GetRenderer(),
		game.GetMouseBox(), game.GetEvent(), game.GetCamera());
	reg.GetSystem<RenderCollisionSystem>().Update(game.GetRenderer(), game.GetCamera());
	reg.GetSystem<RenderEditorGUISystem>().Update(game.GetAssetManager(), game.GetRenderer());

	// Draw The Editor Boundries
	SDL_SetRenderDrawColor(game.GetRenderer(), 70, 70, 70, 70);
	SDL_RenderFillRect(game.GetRenderer(), &hudRectLeft);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRectLeft);
	SDL_RenderFillRect(game.GetRenderer(), &hudRectTop);
	SDL_RenderDrawRect(game.GetRenderer(), &hudRectTop);
}

bool EditorState::OnEnter()
{
	LevelLoader loader;
	// Stop any music that my be playing
	Mix_HaltMusic();
	// Set the desired window size/position for the editor
	SDL_SetWindowSize(game.GetWindow(), 1920, 1080);
	SDL_SetWindowPosition(game.GetWindow(), 0, 0);

	// Set the Camera Position
	game.GetCamera().x = 0;
	game.GetCamera().y = 0;
	game.GetCamera().w = 1920;
	game.GetCamera().h = 1080;

	// Initialize IMGUI context
	ImGui::CreateContext();
	ImGuiSDL::Initialize(game.GetRenderer(), 1920, 1080);

	// If the needed systems are not already in the regstry, Add them
	if (!reg.HasSystem<RenderCollisionSystem>()) reg.AddSystem<RenderCollisionSystem>();
	if (!reg.HasSystem<RenderEditorGUISystem>()) reg.AddSystem<RenderEditorGUISystem>();
	if (!reg.HasSystem<RenderEditorSystem>()) 	reg.AddSystem<RenderEditorSystem>();
	if (!reg.HasSystem<MouseControlSystem>()) 	reg.AddSystem<MouseControlSystem>();
	if (!reg.HasSystem<EditorKeyboardControlSystem>()) reg.AddSystem<EditorKeyboardControlSystem>();

	loader.LoadAssetsFromLuaTable(game.GetLuaState(), "editor_assets");
	// Assign values to varialbes
	editor = false;
	keyDown = false;
	return true;
}

bool EditorState::OnExit()
{
	// Delete the editor entities
	reg.GetSystem<RenderEditorSystem>().OnExit();

	game.GetCamera().w = game.windowWidth;
	game.GetCamera().h = game.windowHeight;

	// Set the Window Size/Position to the desired Game Window Size and position
	SDL_SetWindowSize(game.GetWindow(), 256 * 4, 240 * 4);
	SDL_SetWindowPosition(game.GetWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	// Remove any sytems that are only used inside the editor
	reg.RemoveSystem<RenderEditorGUISystem>();
	reg.RemoveSystem<RenderEditorSystem>();
	reg.RemoveSystem<MouseControlSystem>();
	reg.RemoveSystem<EditorKeyboardControlSystem>();

	if (!Game::isDebug) Game::isDebug = true;
	return true;
}

void EditorState::ProcessEvents(SDL_Event& event)
{

}

void EditorState::OnKeyDown(SDL_Event* event)
{
	EditorFileLoader loader;

	if (game.GetEvent().type == SDL_KEYDOWN && !keyDown)
	{
		const Uint8* state = SDL_GetKeyboardState(NULL);
		FileDialogs dialog;

		if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_O])
		{
			if (!RenderEditorGUISystem::imageName.empty())
			{
				RenderEditorGUISystem::fileName = dialog.OpenFile();

				loader.SetFileName(RenderEditorGUISystem::fileName);

				// Check to see if the String is Empty!!
				if (!RenderEditorGUISystem::fileName.empty())
				{
					loader.LoadTilemap(game.GetAssetManager(), game.GetRenderer());
					RenderEditorGUISystem::fileLoaded = true;
				}
			}
		}

		if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_S])
		{
			if (MouseControlSystem::createTile)
			{
				if (!RenderEditorGUISystem::fileName.empty())
				{
					loader.SaveTilemap(RenderEditorGUISystem::fileName, game.GetAssetManager(), game.GetRenderer());
					RenderEditorGUISystem::fileLoaded = true;

				}
			}

			if (MouseControlSystem::createObstacles)
			{
				if (!RenderEditorGUISystem::fileName.empty())
				{
					loader.SaveObjectMap(RenderEditorGUISystem::fileName, game.GetAssetManager(), game.GetRenderer());
					RenderEditorGUISystem::fileLoaded = true;
				}
			}
		}

		if (event->key.keysym.sym == SDLK_SPACE)
		{
			game.GetCamera().x = 0;
			game.GetCamera().y = 0;
		}
		keyDown = true;
	}
}

void EditorState::OnKeyUp(SDL_Event* event)
{
	keyDown = false;
}
