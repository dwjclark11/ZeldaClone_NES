#include "MenuState.h"
#include "GameState.h"
#include "EditorState.h"
#include "../Game/Game.h"
#include "GameStateMachine.h"
#include "../Game/LevelLoader.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/MenuComponent.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/GameSystems/GamePadSystem.h"
#include "../Systems/MenuSystems/RenderMainMenuSystem.h"
#include "../Systems/MenuSystems/MenuKeyboardControlSystem.h"

// Define all static variables
const std::string MenuState::menuID = "MENU";
std::string MenuState::player1Name = "";
std::string MenuState::player2Name = "";
std::string MenuState::player3Name = "";

bool MenuState::slotsFull = false;

void MenuState::Update(const double& deltaTime)
{
	Game::Instance()->GetEventManager()->Reset();
	Registry::Instance()->GetSystem<MenuKeyboardControlSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	

	Registry::Instance()->Update();
	Registry::Instance()->GetSystem<MenuKeyboardControlSystem>().Update();

	if (slotsFull && !full)
	{
		Entity slotsFull = Registry::Instance()->CreateEntity();
		slotsFull.AddComponent<TextLabelComponent>(glm::vec2(300, 900), " REGISTERS ARE FULL! ", "charriot-font-40", SDL_Color{ 255,0,0,0 }, true);
		full = true;
	}
}

void MenuState::Render()
{
	//Registry::Instance()->GetSystem<RenderSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	Registry::Instance()->GetSystem<RenderTextSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
	Registry::Instance()->GetSystem<RenderMainMenuSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager());
}

bool MenuState::OnEnter()
{
	LevelLoader loader;
	full = false;

	if (!Registry::Instance()->HasSystem<MenuKeyboardControlSystem>()) Registry::Instance()->AddSystem<MenuKeyboardControlSystem>();
	if (!Registry::Instance()->HasSystem<RenderTextSystem>()) Registry::Instance()->AddSystem<RenderTextSystem>();
	if (!Registry::Instance()->HasSystem<RenderMainMenuSystem>()) Registry::Instance()->AddSystem<RenderMainMenuSystem>();

	loader.LoadAssetsFromLuaTable(lua, "menu_state_assets");

	Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), "Main_Menu", -1);

	Entity playerGameBTN1 = Registry::Instance()->CreateEntity();
	playerGameBTN1.AddComponent<SpriteComponent>("Link", 32, 32, 0, true);
	playerGameBTN1.AddComponent<TransformComponent>(glm::vec2(320, 200), glm::vec2(4, 4), 0.0);
	playerGameBTN1.AddComponent<MenuComponent>();
	playerGameBTN1.Tag("first_menu_slot");

	Entity playerGameBTN2 = Registry::Instance()->CreateEntity();
	playerGameBTN2.AddComponent<SpriteComponent>("Link", 32, 32, 0, true);
	playerGameBTN2.AddComponent<TransformComponent>(glm::vec2(320, 300), glm::vec2(4, 4), 0.0);
	playerGameBTN2.AddComponent<MenuComponent>();
	playerGameBTN2.Tag("second_menu_slot");

	Entity playerGameBTN3 = Registry::Instance()->CreateEntity();
	playerGameBTN3.AddComponent<SpriteComponent>("Link", 32, 32, 0, true);
	playerGameBTN3.AddComponent<TransformComponent>(glm::vec2(320, 400), glm::vec2(4, 4), 0.0);
	playerGameBTN3.AddComponent<MenuComponent>();
	playerGameBTN3.Tag("third_menu_slot");

	loader.LoadMenuScreenFromLuaTable(lua, "save1");
	loader.LoadMenuScreenFromLuaTable(lua, "save2");
	loader.LoadMenuScreenFromLuaTable(lua, "save3");

	if (Game::isDebug)
	{	
		// Tilemap Editor
		Entity editorBTN = Registry::Instance()->CreateEntity();
		editorBTN.AddComponent<SpriteComponent>("main_menu_gui", 224, 16, 0, true, 0, 80);
		editorBTN.AddComponent<TransformComponent>(glm::vec2(320, 700), glm::vec2(1.5, 1.5), 0.0);
		editorBTN.AddComponent<MenuComponent>();
	}

	// Register your name
	Entity registerNameBTN = Registry::Instance()->CreateEntity();
	registerNameBTN.AddComponent<SpriteComponent>("main_menu_gui", 288, 16, 0, true, 0, 16);
	registerNameBTN.AddComponent<TransformComponent>(glm::vec2(320, 500), glm::vec2(1.5, 1.5), 0.0);
	registerNameBTN.AddComponent<MenuComponent>();

	// Elimination Mode
	Entity deleteNameBTN = Registry::Instance()->CreateEntity();
	deleteNameBTN.AddComponent<SpriteComponent>("main_menu_gui", 256, 16, 0, true, 0, 64);
	deleteNameBTN.AddComponent<TransformComponent>(glm::vec2(320, 600), glm::vec2(1.5, 1.5), 0.0);
	deleteNameBTN.AddComponent<MenuComponent>();

	// Select
	Entity menuLabel1 = Registry::Instance()->CreateEntity();
	menuLabel1.AddComponent<SpriteComponent>("main_menu_gui", 160, 16, 0, true, 0, 0);
	menuLabel1.AddComponent<TransformComponent>(glm::vec2(340, 100), glm::vec2(2.5, 2.5), 0.0);
	menuLabel1.AddComponent<MenuComponent>();

	// Name
	Entity menuLabel2 = Registry::Instance()->CreateEntity();
	menuLabel2.AddComponent<SpriteComponent>("main_menu_gui", 64, 16, 0, true, 0, 32);
	menuLabel2.AddComponent<TransformComponent>(glm::vec2(355, 150), glm::vec2(2, 2), 0.0);
	menuLabel2.AddComponent<MenuComponent>();

	// Life
	Entity menuLabel3 = Registry::Instance()->CreateEntity();
	menuLabel3.AddComponent<SpriteComponent>("main_menu_gui", 64, 16, 0, true, 0, 48);
	menuLabel3.AddComponent<TransformComponent>(glm::vec2(608, 150), glm::vec2(2, 2), 0.0);
	menuLabel3.AddComponent<MenuComponent>();

	Entity menuBox = Registry::Instance()->CreateEntity();
	menuBox.AddComponent<SpriteComponent>("menu_box", 160, 160, 0, false);
	menuBox.AddComponent<TransformComponent>(glm::vec2(140, 70), glm::vec2(5, 5), 0.0);
	menuBox.AddComponent<MenuComponent>();

	Entity selector = Registry::Instance()->CreateEntity();
	selector.AddComponent<SpriteComponent>("hearts", 16, 16, 0, false);
	selector.AddComponent<TransformComponent>(glm::vec2(260, 200), glm::vec2(4, 4), 0.0);
	selector.AddComponent<MenuComponent>();
	selector.Tag("selector");

	return true;
}

bool MenuState::OnExit()
{
	Registry::Instance()->GetSystem<RenderMainMenuSystem>().OnExit();
	Registry::Instance()->GetSystem<RenderTextSystem>().OnExit();
	return true;
}

void MenuState::ProcessEvents(SDL_Event& event)
{
	Registry::Instance()->GetSystem<GamePadSystem>().UpdateOtherStates(event);
}

void MenuState::OnKeyDown(SDL_Event* event)
{

}

void MenuState::OnKeyUp(SDL_Event* event)
{
}


