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

	reg.GetSystem<RenderEditorSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());

	if (editor)
		reg.GetSystem<RenderEditorGUISystem>().Update(game.GetAssetManager(), game.GetRenderer());
	else
	{
		reg.GetSystem<MouseControlSystem>().Update(game.GetAssetManager(), game.GetRenderer(),
			game.GetMouseBox(), game.GetEvent(), game.GetCamera());

		reg.GetSystem<RenderCollisionSystem>().Update(game.GetRenderer(), game.GetCamera());
		
		// Create the HUD rect that each tilemap screen will be inside
		SDL_Rect hudRectTop = { 0, 0, 1920, 288 };
		
		// Test to see if this works correctly
		// ****************************************************************** //
		SDL_Rect hudRectBottom = { 0, 960, 1920, 120};
		SDL_Rect hudRectLeft = { 0, 0, 448, 1080};
		SDL_Rect hudRectRight = { 1472, 0, 448, 1080};
		// ****************************************************************** //

		// Render all HUD objects
		SDL_SetRenderDrawColor(game.GetRenderer(), 70, 70, 70, 100);

		// ****************************************************************** //
		SDL_RenderFillRect(game.GetRenderer(), &hudRectTop);
		SDL_RenderDrawRect(game.GetRenderer(), &hudRectTop);
		
		SDL_RenderFillRect(game.GetRenderer(), &hudRectBottom);
		SDL_RenderDrawRect(game.GetRenderer(), &hudRectBottom);
		
		SDL_RenderFillRect(game.GetRenderer(), &hudRectLeft);
		SDL_RenderDrawRect(game.GetRenderer(), &hudRectLeft);
		
		SDL_RenderFillRect(game.GetRenderer(), &hudRectRight);
		SDL_RenderDrawRect(game.GetRenderer(), &hudRectRight);
		// ****************************************************************** //
		
		SDL_SetRenderDrawColor(game.GetRenderer(), 0, 0, 0, 255);

		// --- Render the Labels on top of the surounding rects
		reg.GetSystem<RenderEditorLabelSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	}

}	

bool EditorState::OnEnter()
{
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
	if(!reg.HasSystem<RenderCollisionSystem>()) reg.AddSystem<RenderCollisionSystem>();
	if(!reg.HasSystem<RenderEditorGUISystem>()) reg.AddSystem<RenderEditorGUISystem>();
	if(!reg.HasSystem<RenderEditorSystem>()) 	reg.AddSystem<RenderEditorSystem>();
	if(!reg.HasSystem<MouseControlSystem>()) 	reg.AddSystem<MouseControlSystem>();
	if(!reg.HasSystem<EditorKeyboardControlSystem>()) reg.AddSystem<EditorKeyboardControlSystem>();
	if(!reg.HasSystem<RenderEditorLabelSystem>()) reg.AddSystem<RenderEditorLabelSystem>();
	//reg.AddSystem<AnimationSystem>();
	
	// Assign values to varialbes
	editor = false;
	keyDown = false;
	bombs = false; // ?
	return true;
}

bool EditorState::OnExit()
{
	// Delete the editor entities
	reg.GetSystem<RenderEditorSystem>().OnExit();
	
	game.GetCamera().w = game.windowWidth;
	game.GetCamera().y = game.windowHeight;
	
	// Set the Window Size/Position to the desired Game Window Size and position
	SDL_SetWindowSize(game.GetWindow(), 256 * 4, 240 * 4);
	SDL_SetWindowPosition(game.GetWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	// Remove any sytems that are only used inside the editor
	reg.RemoveSystem<RenderEditorGUISystem>();
	reg.RemoveSystem<RenderEditorSystem>();
	reg.RemoveSystem<MouseControlSystem>();
	reg.RemoveSystem<EditorKeyboardControlSystem>();
	reg.RemoveSystem<RenderEditorLabelSystem>();
	
	// Turn on Debug if it has been turned off --> To allow access to the editor without having to reload
	if (!Game::isDebug) Game::isDebug = true;
	
	return true;
}

void EditorState::ProcessEvents(SDL_Event& event)
{
	//if (SDLK_RETURN && editor && keyDown)
	//{
	//	Logger::Err("Editor UP");
	//	editor = -editor;
	//	keyDown = false;
	//}
}

void EditorState::OnKeyDown(SDL_Event* event)
{
	if (game.GetEvent().type == SDL_KEYDOWN && !keyDown)
	{
		if (event->key.keysym.sym == SDLK_RETURN)
		{
			editor = !editor;
		}
		
		keyDown = true;
	}
}

void EditorState::OnKeyUp(SDL_Event* event)
{
	keyDown = false;
}


