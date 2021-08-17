#include "SaveGameState.h"
#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Components/TextLabelComponent.h"
#include "../Systems/PauseSystems/RenderSaveStateSystem.h"
#include "../Systems/PauseSystems/SaveSelectKeyboardSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"

const std::string SaveGameState::saveID = "SAVE";

void SaveGameState::Update(const double& deltaTime)
{
	Game::Instance()->GetEventManager()->Reset();
	Registry::Instance()->GetSystem<SaveSelectKeyboardSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->Update();
	Registry::Instance()->GetSystem<AnimationSystem>().Update();

	if (State::exitToMain) Game::Instance()->GetStateMachine()->PopState();
}

void SaveGameState::Render()
{
	Game::Instance()->GetSystem<RenderSaveStateSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager());
}

bool SaveGameState::OnEnter()
{	

	/*
		Create some assets/Textures to replace the SDL_TTF components --> There are glitches with the SDL_TTF and some times 
		when in release mode the words do not show up on the screen.
		
		Also TODO:
			- Create a SaveComponent
			- Create a SaveKeyboardControlSystem
				- Also have it allow the game controller to work with it.
			
	*/
	
	Registry::Instance()->AddSystem<RenderSaveStateSystem>();
	Registry::Instance()->AddSystem<SaveSelectKeyboardSystem>();
	Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "save_gui", "./Assets/Backgrounds/save_state_GUI.png");


	Entity saveText = Registry::Instance()->CreateEntity();
	saveText.AddComponent<TransformComponent>(glm::vec2(200, 200), glm::vec2(4, 4), 0.0);
	saveText.AddComponent<SpriteComponent>("save_gui", 144, 16, 0, true, 0,0);
	saveText.AddComponent<SaveComponent>();
	
	Entity exitText = Registry::Instance()->CreateEntity();
	exitText.AddComponent<TransformComponent>(glm::vec2(200, 400), glm::vec2(4, 4), 0.0);
	exitText.AddComponent<SpriteComponent>("save_gui", 192, 16, 0, true, 0, 16);
	exitText.AddComponent<SaveComponent>();

	Entity quitText = Registry::Instance()->CreateEntity();
	quitText.AddComponent<TransformComponent>(glm::vec2(200, 600), glm::vec2(4, 4), 0.0);
	quitText.AddComponent<SpriteComponent>("save_gui", 144, 16, 0, true, 0, 32);
	quitText.AddComponent<SaveComponent>();

	Entity selector = Registry::Instance()->CreateEntity();
	selector.AddComponent<SpriteComponent>("hud_hearts", 16, 16, 5, true, 0, 0);
	selector.AddComponent<TransformComponent>(glm::vec2(100, 185), glm::vec2(6, 6), 0.0);
	selector.AddComponent<SaveComponent>();
	selector.Tag("save_selector");
	return true;
}

bool SaveGameState::OnExit()
{
	Logger::Err("Exiting Save State");
	
	return true;
}

void SaveGameState::ProcessEvents(SDL_Event& event)
{
	
}

void SaveGameState::OnKeyDown(SDL_Event* event)
{
	if (event->key.keysym.sym == SDLK_x)
	{
		Game::Instance()->GetStateMachine()->PopState();
	}
}
void SaveGameState::OnKeyUp(SDL_Event* event)
{
	
}
