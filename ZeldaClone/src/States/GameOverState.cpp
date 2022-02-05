#include "GameOverState.h"
#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/GameSystems/GameOverKeyboardControlSystem.h"
#include "../Systems/GameSystems/RenderGameOverSystem.h"
#include "../Systems/GameSystems/RenderSystem.h"
#include "../AssetManager/AssetManager.h"

const std::string GameOverState::gameOverID = "GAMEOVER";

GameOverState::GameOverState()
	: game(Game::Instance())
{
}

void GameOverState::Update(const double& deltaTime)
{
	game.GetEventManager()->Reset();
	Registry::Instance().GetSystem<GameOverKeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	Registry::Instance().Update();
	Registry::Instance().GetSystem<GameOverKeyboardControlSystem>().Update();
}

void GameOverState::Render()
{
	game.GetSystem<RenderGameOverTextSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	game.GetSystem<RenderGameOverSystem>().Update(game.GetRenderer(), game.GetAssetManager());
}

bool GameOverState::OnEnter()
{
	if (!Registry::Instance().HasSystem<RenderGameOverTextSystem>()) 
		Registry::Instance().AddSystem<RenderGameOverTextSystem>();
	if (!Registry::Instance().HasSystem<RenderGameOverSystem>()) 
		Registry::Instance().AddSystem<RenderGameOverSystem>();
	if (!Registry::Instance().HasSystem<GameOverKeyboardControlSystem>()) 
		Registry::Instance().AddSystem<GameOverKeyboardControlSystem>();
	
	game.GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), "Main_Menu", -1);
	
	game.GetAssetManager()->AddTextures(game.GetRenderer(), "game_over_words", "./Assets/HUDSprites/game_over_words.png");

	Entity game_over_text = Registry::Instance().CreateEntity();
	game_over_text.AddComponent<TransformComponent>(glm::vec2(185, 50), glm::vec2(4, 4), 0.0);
	game_over_text.AddComponent<SpriteComponent>("game_over_words", 176, 16, 1, true, 0, 0);
	game_over_text.Group("game_over");

	Entity continue_text = Registry::Instance().CreateEntity();
	continue_text.AddComponent<TransformComponent>(glm::vec2(320, 500), glm::vec2(2, 2), 0.0);
	continue_text.AddComponent<SpriteComponent>("game_over_words", 128, 16, 1, true, 0, 16);
	continue_text.Group("game_over");

	Entity retry_text = Registry::Instance().CreateEntity();
	retry_text.AddComponent<TransformComponent>(glm::vec2(320, 600), glm::vec2(2, 2), 0.0);
	retry_text.AddComponent<SpriteComponent>("game_over_words", 80, 16, 1, true, 0, 32);
	retry_text.Group("game_over");

	Entity quit_text = Registry::Instance().CreateEntity();
	quit_text.AddComponent<TransformComponent>(glm::vec2(320, 700), glm::vec2(2, 2), 0.0);
	quit_text.AddComponent<SpriteComponent>("game_over_words", 64, 16, 1, true, 0, 48);
	quit_text.Group("game_over");

	Entity selector = Registry::Instance().CreateEntity();
	selector.AddComponent<SpriteComponent>("hearts", 16, 16, 0, false);
	selector.AddComponent<TransformComponent>(glm::vec2(260, 475), glm::vec2(4, 4), 0.0);
	selector.Tag("gameOverSelector");
	selector.Group("game_over");

	//Logger::Log("Entering Game Over State");
	return true;
}

bool GameOverState::OnExit()
{
	game.GetSystem<RenderGameOverSystem>().OnExit();
	game.GetSystem<RenderGameOverTextSystem>().OnExit();
	game.GetSystem<RenderTextSystem>().OnExit();
	Logger::Log("Exiting Game Over State");
	game.GetPlayerDead() = false;
	return true;
}

void GameOverState::ProcessEvents(SDL_Event& event)
{
	game.GetSystem<GamePadSystem>().UpdateOtherStates(event);
}

void GameOverState::OnKeyDown(SDL_Event* event)
{

}

void GameOverState::OnKeyUp(SDL_Event* event)
{

}
