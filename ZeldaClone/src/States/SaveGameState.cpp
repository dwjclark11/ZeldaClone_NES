#include "SaveGameState.h"
#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/SaveComponent.h"
#include "../Systems/PauseSystems/RenderSaveStateSystem.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"
#include "../Systems/GameSystems/AnimationSystem.h"

const std::string SaveGameState::saveID = "SAVE";

SaveGameState::SaveGameState()
	: game(Game::Instance()), reg(Registry::Instance())
{
}

void SaveGameState::Update(const float& deltaTime)
{
	game.GetEventManager()->Reset();
	Registry::Instance().GetSystem<KeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	reg.Update();
	Registry::Instance().GetSystem<AnimationSystem>().Update();
}

void SaveGameState::Render()
{
	Registry::Instance().GetSystem<RenderSaveStateSystem>().Update(game.GetRenderer(), game.GetAssetManager());
}

bool SaveGameState::OnEnter()
{	
	
	reg.AddSystem<RenderSaveStateSystem>();

	game.GetAssetManager()->AddTextures(game.GetRenderer(), "save_gui", "./Assets/HUDSprites/save_state_GUI.png");


	Entity saveText = reg.CreateEntity();
	saveText.AddComponent<TransformComponent>(glm::vec2(200, 200), glm::vec2(4, 4), 0.0);
	saveText.AddComponent<SpriteComponent>("save_gui", 144, 16, 0, true, 0,0);
	saveText.AddComponent<SaveComponent>();
	
	Entity exitText = reg.CreateEntity();
	exitText.AddComponent<TransformComponent>(glm::vec2(200, 400), glm::vec2(4, 4), 0.0);
	exitText.AddComponent<SpriteComponent>("save_gui", 192, 16, 0, true, 0, 16);
	exitText.AddComponent<SaveComponent>();

	Entity quitText = reg.CreateEntity();
	quitText.AddComponent<TransformComponent>(glm::vec2(200, 600), glm::vec2(4, 4), 0.0);
	quitText.AddComponent<SpriteComponent>("save_gui", 144, 16, 0, true, 0, 32);
	quitText.AddComponent<SaveComponent>();

	Entity selector = reg.CreateEntity();
	selector.AddComponent<SpriteComponent>("hud_hearts", 16, 16, 5, true, 0, 0);
	selector.AddComponent<TransformComponent>(glm::vec2(100, 185), glm::vec2(6, 6), 0.0);
	selector.AddComponent<KeyboardControlComponent>();
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
		game.GetStateMachine()->PopState();
	}
}
void SaveGameState::OnKeyUp(SDL_Event* event)
{
	
}

void SaveGameState::OnBtnDown(SDL_Event* event)
{

}

void SaveGameState::OnBtnUp(SDL_Event* event)
{
}
