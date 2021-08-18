#include "EditorState.h"
#include "../ECS/ECS.h"
#include "../Systems/EditorSystems/RenderEditorGUISystem.h"
#include "../Systems/EditorSystems/RenderEditorSystem.h"
#include "../Systems/EditorSystems/EditorKeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
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
		Registry::Instance()->GetSystem<RenderEditorLabelSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	}
}	

bool EditorState::OnEnter()
{
	Mix_HaltMusic();
	SDL_SetWindowSize(Game::Instance()->GetWindow(), 1920, 1080);
	SDL_SetWindowPosition(Game::Instance()->GetWindow(), 0, 0);
	Game::Instance()->GetCamera().x = 0;
	Game::Instance()->GetCamera().y = 0;

	// Initialize IMGUI context
	ImGui::CreateContext();
	ImGuiSDL::Initialize(Game::Instance()->GetRenderer(), 1920, 1080);
	
	Registry::Instance()->AddSystem<RenderEditorGUISystem>();
	Registry::Instance()->AddSystem<RenderEditorSystem>();
	Registry::Instance()->AddSystem<MouseControlSystem>();
	Registry::Instance()->AddSystem<EditorKeyboardControlSystem>();
	Registry::Instance()->AddSystem<RenderEditorLabelSystem>();
	
	editor = false;
	keyDown = false;
	bombs = false;
	return true;
}

bool EditorState::OnExit()
{
	Registry::Instance()->GetSystem<RenderEditorSystem>().OnExit();
	SDL_SetWindowSize(Game::Instance()->GetWindow(), 256 * 4, 240 * 4);
	SDL_SetWindowPosition(Game::Instance()->GetWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
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


