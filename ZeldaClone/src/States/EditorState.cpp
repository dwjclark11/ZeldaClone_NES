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

void EditorState::Update(const double& deltaTime)
{

	Game::Instance()->GetEventManager()->Reset();
	Registry::Instance()->GetSystem<EditorKeyboardControlSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	
	Registry::Instance()->Update();
	Registry::Instance()->GetSystem<CameraMovementSystem>().Update(Game::Instance()->GetCamera());
	//Registry::Instance()->GetSystem<AnimationSystem>().Update();
}

void EditorState::Render()
{

	Registry::Instance()->GetSystem<RenderEditorSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());

	if (editor)
		Registry::Instance()->GetSystem<RenderEditorGUISystem>().Update(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer());
	else
	{
		Registry::Instance()->GetSystem<MouseControlSystem>().Update(Game::Instance()->GetAssetManager(), Game::Instance()->GetRenderer(),
			Game::Instance()->GetMouseBox(), Game::Instance()->GetEvent(), Game::Instance()->GetCamera());

		Registry::Instance()->GetSystem<RenderCollisionSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetCamera());
		
		// Create the HUD rect --> black rectangle that all the HUD items are on
		SDL_Rect hudRect = { 0, 0, 1920, 1080 / 4 };

		// Render all HUD objects
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 70, 70, 70, 255);
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &hudRect);
		SDL_RenderDrawRect(Game::Instance()->GetRenderer(), &hudRect);
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);

		// ---
		Registry::Instance()->GetSystem<RenderEditorLabelSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	}




}	

bool EditorState::OnEnter()
{
	// Stop any music that my be playing
	Mix_HaltMusic();
	// Set the desired window size/position for the editor
	SDL_SetWindowSize(Game::Instance()->GetWindow(), 1920, 1080);
	SDL_SetWindowPosition(Game::Instance()->GetWindow(), 0, 0);
	
	// Set the Camera Position
	Game::Instance()->GetCamera().x = 0;
	Game::Instance()->GetCamera().y = 0;
	Game::Instance()->GetCamera().w = 1920;
	Game::Instance()->GetCamera().h = 1080;

	// Initialize IMGUI context
	ImGui::CreateContext();
	ImGuiSDL::Initialize(Game::Instance()->GetRenderer(), 1920, 1080);
	
	// If the needed systems are not already in the regstry, Add them
	if(!Registry::Instance()->HasSystem<RenderCollisionSystem>()) Registry::Instance()->AddSystem<RenderCollisionSystem>();
	if(!Registry::Instance()->HasSystem<RenderEditorGUISystem>()) Registry::Instance()->AddSystem<RenderEditorGUISystem>();
	if(!Registry::Instance()->HasSystem<RenderEditorSystem>()) 	Registry::Instance()->AddSystem<RenderEditorSystem>();
	if(!Registry::Instance()->HasSystem<MouseControlSystem>()) 	Registry::Instance()->AddSystem<MouseControlSystem>();
	if(!Registry::Instance()->HasSystem<EditorKeyboardControlSystem>()) Registry::Instance()->AddSystem<EditorKeyboardControlSystem>();
	if(!Registry::Instance()->HasSystem<RenderEditorLabelSystem>()) Registry::Instance()->AddSystem<RenderEditorLabelSystem>();
	//Registry::Instance()->AddSystem<AnimationSystem>();
	
	// Assign values to varialbes
	editor = false;
	keyDown = false;
	bombs = false; // ?
	return true;
}

bool EditorState::OnExit()
{
	// Delete the editor entities
	Registry::Instance()->GetSystem<RenderEditorSystem>().OnExit();
	Game::Instance()->GetCamera().w = Game::Instance()->windowWidth;
	Game::Instance()->GetCamera().y = Game::Instance()->windowHeight;
	// Set the Window Size/Position to the desired Game Window Size and position
	SDL_SetWindowSize(Game::Instance()->GetWindow(), 256 * 4, 240 * 4);
	SDL_SetWindowPosition(Game::Instance()->GetWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	// Remove any sytems that are only used inside the editor
	Registry::Instance()->RemoveSystem<RenderEditorGUISystem>();
	Registry::Instance()->RemoveSystem<RenderEditorSystem>();
	Registry::Instance()->RemoveSystem<MouseControlSystem>();
	Registry::Instance()->RemoveSystem<EditorKeyboardControlSystem>();
	Registry::Instance()->RemoveSystem<RenderEditorLabelSystem>();

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
	if (Game::Instance()->GetEvent().type == SDL_KEYDOWN && !keyDown)
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


