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
	loader.LoadMenuUIFromLuaTable(lua, "menu_state_load");

	//Game::Instance()->GetSystem<MusicPlayerSystem>().PlayMusic(Game::Instance()->GetAssetManager(), "Main_Menu", -1);

	loader.LoadMenuScreenFromLuaTable(lua, "save1");
	loader.LoadMenuScreenFromLuaTable(lua, "save2");
	loader.LoadMenuScreenFromLuaTable(lua, "save3");
	
	return true;
}

bool MenuState::OnExit()
{
	Registry::Instance()->GetSystem<RenderMainMenuSystem>().OnExit();
	Registry::Instance()->GetSystem<RenderTextSystem>().OnExit();

	Registry::Instance()->RemoveSystem<RenderMainMenuSystem>();
	Registry::Instance()->RemoveSystem<MenuKeyboardControlSystem>();
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


