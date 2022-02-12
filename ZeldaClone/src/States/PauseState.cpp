#include "PauseState.h"
#include "../Game/Game.h"
#include <SDL_ttf.h>
#include "../States/GameState.h"
#include "../States/SaveGameState.h"
#include "../Systems/PauseSystems/RenderPauseSystem.h"
#include "../Systems/GameSystems/RenderHUDSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"
//#include "../Systems/PauseSystems/ItemSelectKeyboardSystem.h"
#include "../Game/LevelLoader.h"
#include "../Systems/GameSystems/GamePadSystem.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PauseComponent.h"

const std::string PauseState::pauseID = "PAUSE";
bool PauseState::firstEnter = false;

PauseState::PauseState()
	: bombs(false), game(Game::Instance()), reg(Registry::Instance())
{
}

void PauseState::Update(const double& deltaTime)
{
	game.GetEventManager()->Reset();
	//Registry::Instance().GetSystem<ItemSelectKeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	Registry::Instance().GetSystem<KeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	Registry::Instance().GetSystem<GamePadSystem>().SubscribeToEvents(game.GetEventManager());
	reg.Update();
	//Registry::Instance().GetSystem<ItemSelectKeyboardControlSystem>().Update();

	if (GameState::totalBombs > 0 && !game.GetGameItems().bombs)
	{
		Entity bombItem = reg.CreateEntity();
		bombItem.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 112);
		bombItem.AddComponent<TransformComponent>(glm::vec2(486, 190), glm::vec2(4, 4), 0.0);
		bombItem.AddComponent<PauseComponent>();
		bombItem.Tag("bombItem");
		bombItem.Group("pause");

		game.GetGameItems().bombs = true;
	}

	if (game.GetFadeAlpha() == 0 && !game.FadeFinished())
	{
		GameState::unpause = true;
		GamePadSystem::paused = false;
		game.FadeFinished() = true;
		game.StartFadeOut() = false;
		game.GetStateMachine()->PopState();
	}

	if (GameState::totalBombs == 0 && game.GetGameItems().bombs) game.GetGameItems().bombs = false;
	//if (State::exitToMain) game.GetStateMachine()->PopState();
}

void PauseState::Render()
{
	Registry::Instance().GetSystem<RenderPauseSystem>().Update(game.GetRenderer(), game.GetAssetManager());
	Registry::Instance().GetSystem<RenderHUDSystem>().Update(game.GetRenderer(), game.GetAssetManager());
	Registry::Instance().GetSystem<RenderTextSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	//Registry::Instance().GetSystem<AnimationSystem>().Update();
}

bool PauseState::OnEnter()
{
	// Turn music volume down while paused
	Mix_VolumeMusic(3);

	game.StartFadeIn() = true;
	// =============================================================================================================================
	// Add all necessary systems to the registry if they are not yet registered
	// =============================================================================================================================
	//if (!reg.HasSystem<ItemSelectKeyboardControlSystem>()) 
	//	reg.AddSystem<ItemSelectKeyboardControlSystem>();
	if (!reg.HasSystem<RenderPauseSystem>()) 
		reg.AddSystem<RenderPauseSystem>();
	// =============================================================================================================================


	game.GetAssetManager()->AddTextures(game.GetRenderer(), "pause_hud", "./Assets/HUDSprites/pauseHud.png");
	firstEnter = true;

	Entity pauseSelector = reg.CreateEntity();
	pauseSelector.AddComponent<SpriteComponent>("box", 16, 16, 0, false, 16, 0);
	pauseSelector.AddComponent<TransformComponent>(glm::vec2(386, 190), glm::vec2(4, 4), 0.0);
	pauseSelector.AddComponent<PauseComponent>();
	pauseSelector.AddComponent<KeyboardControlComponent>();
	pauseSelector.Tag("pauseSelector");
	pauseSelector.Group("pause");

	Entity selectedItem = reg.CreateEntity();
	selectedItem.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 48, 16);
	selectedItem.AddComponent<TransformComponent>(glm::vec2(200, 185), glm::vec2(6, 6), 0.0);
	selectedItem.AddComponent<PauseComponent>();
	selectedItem.AddComponent<KeyboardControlComponent>();
	selectedItem.Tag("selectedItem");
	selectedItem.Group("pause");

	Entity hudHolder = reg.CreateEntity();
	hudHolder.AddComponent<TransformComponent>(glm::vec2(325, 100), glm::vec2(5, 5), 0.0);
	hudHolder.AddComponent<SpriteComponent>("pause_hud", 96, 64, 5, true, 0, 0);
	hudHolder.AddComponent<PauseComponent>();
	hudHolder.Group("pause");

	Entity hudHolder2 = reg.CreateEntity();
	hudHolder2.AddComponent<TransformComponent>(glm::vec2(144, 135), glm::vec2(6, 6), 0.0);
	hudHolder2.AddComponent<SpriteComponent>("hud_holder", 32, 32, 5, true, 0, 0);
	hudHolder2.AddComponent<PauseComponent>();
	hudHolder2.Group("pause");

	Entity triforce = reg.CreateEntity();
	triforce.AddComponent<SpriteComponent>("triforce", 96, 64, 0, false, 0, 0);
	triforce.AddComponent<TransformComponent>(glm::vec2(325, 400), glm::vec2(4, 4), 0.0);
	triforce.AddComponent<PauseComponent>();
	triforce.Group("pause");

	// Top Row Items
	if (game.GetGameItems().woodBoomerang)
	{
		Entity boomerang = reg.CreateEntity();
		boomerang.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 0, 112);
		boomerang.AddComponent<TransformComponent>(glm::vec2(386, 190), glm::vec2(4, 4), 0.0);
		boomerang.AddComponent<PauseComponent>();
		boomerang.Group("pause");
	}

	if (GameState::totalBombs > 0 && game.GetGameItems().bombs)
	{
		Entity bombs = reg.CreateEntity();
		bombs.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 112);
		bombs.AddComponent<TransformComponent>(glm::vec2(486, 190), glm::vec2(4, 4), 0.0);
		bombs.AddComponent<PauseComponent>();
		bombs.Tag("bombItem");
		bombs.Group("pause");
	}

	if (game.GetGameItems().bow)
	{
		Entity bow = reg.CreateEntity();
		bow.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 0);
		bow.AddComponent<TransformComponent>(glm::vec2(586, 190), glm::vec2(4, 4), 0.0);
		bow.AddComponent<PauseComponent>();
		bow.Group("pause");
	}

	if (game.GetGameItems().candle)
	{
		Entity candle = reg.CreateEntity();
		candle.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 0, 48);
		candle.AddComponent<TransformComponent>(glm::vec2(686, 190), glm::vec2(4, 4), 0.0);
		candle.AddComponent<PauseComponent>();
		candle.Group("pause");
	}

	// Bottom Row Items
	if (game.GetGameItems().flute)
	{
		Entity flute = reg.CreateEntity();
		flute.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 80, 16);
		flute.AddComponent<TransformComponent>(glm::vec2(386, 260), glm::vec2(4, 4), 0.0);
		flute.AddComponent<PauseComponent>();
		flute.Group("pause");
	}

	if (game.GetGameItems().food)
	{
		Entity meat = reg.CreateEntity();
		meat.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 112, 16);
		meat.AddComponent<TransformComponent>(glm::vec2(486, 260), glm::vec2(4, 4), 0.0);
		meat.AddComponent<PauseComponent>();
		meat.Group("pause");
	}

	if (game.GetGameItems().redPotion)
	{
		Entity potion = reg.CreateEntity();
		potion.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 32, 32);
		potion.AddComponent<TransformComponent>(glm::vec2(586, 260), glm::vec2(4, 4), 0.0);
		potion.AddComponent<PauseComponent>();
		potion.Group("pause");
	}

	if (game.GetGameItems().magicRod)
	{
		Entity magicalRod = reg.CreateEntity();
		magicalRod.AddComponent<SpriteComponent>("items", 16, 16, 0, false, 64, 80);
		magicalRod.AddComponent<TransformComponent>(glm::vec2(686, 260), glm::vec2(4, 4), 0.0);
		magicalRod.AddComponent<PauseComponent>();
		magicalRod.Group("pause");
	}
	
	return false;
}

bool PauseState::OnExit()
{
	Registry::Instance().GetSystem<RenderPauseSystem>().OnExit();
	return true;
}

void PauseState::ProcessEvents(SDL_Event& event)
{
	
}

void PauseState::OnKeyDown(SDL_Event* event)
{


	if (event->key.keysym.sym == SDLK_m)
	{
		game.GetStateMachine()->PushState(new SaveGameState());
	}
}

void PauseState::OnKeyUp(SDL_Event* event)
{
	if (event->key.keysym.sym == SDLK_q)
	{
		game.FadeFinished() = false;
		game.StartFadeOut() = true;
	}
}

void PauseState::OnBtnDown(SDL_Event* event)
{

}

void PauseState::OnBtnUp(SDL_Event* event)
{
	if (event->cbutton.button == game.GetBtnBindings().at(Game::Action::PAUSE))
	{
		game.FadeFinished() = false;
		game.StartFadeOut() = true;
	}
}


