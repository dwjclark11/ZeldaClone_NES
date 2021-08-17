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
	//SDL_Rect hudRect = { 0, 0, 1024, 300 };
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	//SDL_RenderFillRect(Game::Instance()->GetRenderer(), &hudRect);
	//SDL_RenderDrawRect(Game::Instance()->GetRenderer(), &hudRect);
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
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
	editor = false;
	keyDown = false;
	bombs = false;

	Registry::Instance()->AddSystem<RenderEditorGUISystem>();
	Registry::Instance()->AddSystem<RenderEditorSystem>();
	Registry::Instance()->AddSystem<MouseControlSystem>();
	Registry::Instance()->AddSystem<EditorKeyboardControlSystem>();
	Registry::Instance()->AddSystem<RenderEditorLabelSystem>();

	//Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "secret_dungeon", "./Assets/Backgrounds/secret_dungeon.png");
	//Entity map = Registry::Instance()->CreateEntity();
	//map.AddComponent<TransformComponent>(glm::vec2(0, 256), glm::vec2(4, 4), 0.0);
	//map.AddComponent<SpriteComponent>("secret_dungeon", 256, 176, 0, false, 0, 0);
	//map.AddComponent<EditorComponent>();
	//map.Group("map");

	Entity map = Registry::Instance()->CreateEntity();
	map.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(4, 4), 0.0);
	map.AddComponent<SpriteComponent>("map", 4096, 1344, 0, false, 0, 0);
	map.AddComponent<EditorComponent>();
	map.Group("map");

	Logger::Log("Entering Editor State");
	return true;
}

bool EditorState::OnExit()
{
	//Registry::Instance()->GetSystem<RenderSystem>().OnExit();
	//Game::Instance()->GetAssetManager()->ClearAssets();
	Logger::Log("Exiting Editor State");
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


