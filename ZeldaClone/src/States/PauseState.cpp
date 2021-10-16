#include "PauseState.h"
#include "../Game/Game.h"
#include <SDL_ttf.h>
#include "../States/GameState.h"
#include "../States/SaveGameState.h"
#include "../Systems/PauseSystems/RenderPauseSystem.h"
#include "../Systems/GameSystems/RenderHUDSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/PauseSystems/ItemSelectKeyboardSystem.h"
#include "../Game/LevelLoader.h"
#include "../Systems/GameSystems/GamePadSystem.h"

const std::string PauseState::pauseID = "PAUSE";
bool PauseState::firstEnter = false;

void PauseState::Update(const double& deltaTime)
{
	Game::Instance()->GetEventManager()->Reset();
	Registry::Instance()->GetSystem<ItemSelectKeyboardControlSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->Update();
	Registry::Instance()->GetSystem<ItemSelectKeyboardControlSystem>().Update();

	if (GameState::totalBombs > 0 && !Game::Instance()->GetGameItems().bombs)
	{
		Entity bombItem = Registry::Instance()->CreateEntity();
		bombItem.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 112);
		bombItem.AddComponent<TransformComponent>(glm::vec2(486, 190), glm::vec2(4, 4), 0.0);
		bombItem.AddComponent<PauseComponent>();
		bombItem.Tag("bombItem");
		bombItem.Group("pause");

		Game::Instance()->GetGameItems().bombs = true;
	}

	if (GameState::totalBombs == 0 && Game::Instance()->GetGameItems().bombs) Game::Instance()->GetGameItems().bombs = false;
	if (State::exitToMain) Game::Instance()->GetStateMachine()->PopState();
}

void PauseState::Render()
{
	Game::Instance()->GetSystem<RenderPauseSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager());
	Game::Instance()->GetSystem<RenderHUDSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager());
	Game::Instance()->GetSystem<RenderTextSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	Registry::Instance()->GetSystem<AnimationSystem>().Update();
}

bool PauseState::OnEnter()
{
	// Turn music volume down while paused
	Mix_VolumeMusic(3);

	// =============================================================================================================================
	// Add all necessary systems to the registry if they are not yet registered
	// =============================================================================================================================
	if (!Registry::Instance()->HasSystem<ItemSelectKeyboardControlSystem>()) 
		Registry::Instance()->AddSystem<ItemSelectKeyboardControlSystem>();
	if (!Registry::Instance()->HasSystem<RenderPauseSystem>()) 
		Registry::Instance()->AddSystem<RenderPauseSystem>();
	// =============================================================================================================================

	if (!firstEnter)
	{
		Logger::Log("Entered!");
		Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "pause_hud", "./Assets/HUDSprites/pauseHud.png");
		firstEnter = true;

		Entity pauseSelector = Registry::Instance()->CreateEntity();
		pauseSelector.AddComponent<SpriteComponent>("box", 16, 16, 0, false, 16, 0);
		pauseSelector.AddComponent<TransformComponent>(glm::vec2(386, 190), glm::vec2(4, 4), 0.0);
		pauseSelector.AddComponent<PauseComponent>();
		pauseSelector.Tag("pauseSelector");
		pauseSelector.Group("pause");



		Entity selectedItem = Registry::Instance()->CreateEntity();
		selectedItem.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 48, 16);
		selectedItem.AddComponent<TransformComponent>(glm::vec2(200, 185), glm::vec2(6, 6), 0.0);
		selectedItem.AddComponent<PauseComponent>();
		selectedItem.Tag("selectedItem");
		selectedItem.Group("pause");

		Entity hudHolder = Registry::Instance()->CreateEntity();
		hudHolder.AddComponent<TransformComponent>(glm::vec2(325, 100), glm::vec2(5, 5), 0.0);
		hudHolder.AddComponent<SpriteComponent>("pause_hud", 96, 64, 5, true, 0, 0);
		hudHolder.AddComponent<PauseComponent>();
		hudHolder.Group("pause");

		Entity hudHolder2 = Registry::Instance()->CreateEntity();
		hudHolder2.AddComponent<TransformComponent>(glm::vec2(144, 135), glm::vec2(6, 6), 0.0);
		hudHolder2.AddComponent<SpriteComponent>("hud_holder", 32, 32, 5, true, 0, 0);
		hudHolder2.AddComponent<PauseComponent>();
		hudHolder2.Group("pause");

		Entity triforce = Registry::Instance()->CreateEntity();
		triforce.AddComponent<SpriteComponent>("triforce", 96, 64, 0, false, 0, 0);
		triforce.AddComponent<TransformComponent>(glm::vec2(325, 400), glm::vec2(4, 4), 0.0);
		triforce.AddComponent<AnimationComponent>(9, 5, false);
		triforce.AddComponent<PauseComponent>();
		triforce.Group("pause");

		// Top Row Items
		if (Game::Instance()->GetGameItems().woodBoomerang)
		{
			Entity boomerang = Registry::Instance()->CreateEntity();
			boomerang.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 0, 112);
			boomerang.AddComponent<TransformComponent>(glm::vec2(386, 190), glm::vec2(4, 4), 0.0);
			boomerang.AddComponent<PauseComponent>();
			boomerang.Group("pause");
		}

		if (GameState::totalBombs > 0 && Game::Instance()->GetGameItems().bombs)
		{
			Entity bombs = Registry::Instance()->CreateEntity();
			bombs.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 112);
			bombs.AddComponent<TransformComponent>(glm::vec2(486, 190), glm::vec2(4, 4), 0.0);
			bombs.AddComponent<PauseComponent>();
			bombs.Tag("bombItem");
			bombs.Group("pause");
		}


		if (Game::Instance()->GetGameItems().bow)
		{
			Entity bow = Registry::Instance()->CreateEntity();
			bow.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 0);
			bow.AddComponent<TransformComponent>(glm::vec2(586, 190), glm::vec2(4, 4), 0.0);
			bow.AddComponent<PauseComponent>();
			bow.Group("pause");
		}

		if (Game::Instance()->GetGameItems().candle)
		{
			Entity candle = Registry::Instance()->CreateEntity();
			candle.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 0, 48);
			candle.AddComponent<TransformComponent>(glm::vec2(686, 190), glm::vec2(4, 4), 0.0);
			candle.AddComponent<PauseComponent>();
			candle.Group("pause");
		}

		// Bottom Row Items
		if (Game::Instance()->GetGameItems().flute)
		{
			Entity flute = Registry::Instance()->CreateEntity();
			flute.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 80, 16);
			flute.AddComponent<TransformComponent>(glm::vec2(386, 260), glm::vec2(4, 4), 0.0);
			flute.AddComponent<PauseComponent>();
			flute.Group("pause");
		}

		if (Game::Instance()->GetGameItems().food)
		{
			Entity meat = Registry::Instance()->CreateEntity();
			meat.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 112, 16);
			meat.AddComponent<TransformComponent>(glm::vec2(486, 260), glm::vec2(4, 4), 0.0);
			meat.AddComponent<PauseComponent>();
			meat.Group("pause");
		}

		if (Game::Instance()->GetGameItems().redPotion)
		{
			Entity potion = Registry::Instance()->CreateEntity();
			potion.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 32, 32);
			potion.AddComponent<TransformComponent>(glm::vec2(586, 260), glm::vec2(4, 4), 0.0);
			potion.AddComponent<PauseComponent>();
			potion.Group("pause");
		}

		if (Game::Instance()->GetGameItems().magicRod)
		{
			Entity magicalRod = Registry::Instance()->CreateEntity();
			magicalRod.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 80);
			magicalRod.AddComponent<TransformComponent>(glm::vec2(686, 260), glm::vec2(4, 4), 0.0);
			magicalRod.AddComponent<PauseComponent>();
			magicalRod.Group("pause");
		}
	}
	
	return false;
}

bool PauseState::OnExit()
{
	//Registry::Instance()->GetSystem<RenderPauseSystem>().OnExit();
	//Logger::Err("Exiting Pause State");
	return true;
}

void PauseState::ProcessEvents(SDL_Event& event)
{
	Registry::Instance()->GetSystem<GamePadSystem>().Update(event);
}

void PauseState::OnKeyDown(SDL_Event* event)
{
	if (event->key.keysym.sym == SDLK_x)
	{
		Game::Instance()->GetStateMachine()->PopState();
	}

	if (event->key.keysym.sym == SDLK_m)
	{
		//Game::Instance()->GetStateMachine()->PopState();
		Game::Instance()->GetStateMachine()->PushState(new SaveGameState());
	}
}

void PauseState::OnKeyUp(SDL_Event* event)
{

}


