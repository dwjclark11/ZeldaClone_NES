#include "MenuState.h"
#include "GameState.h"
#include "EditorState.h"
#include "../Game/Game.h"
#include "GameStateMachine.h"
#include "../Game/LevelLoader.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/MenuComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/GameSystems/GamePadSystem.h"
#include "../Systems/MenuSystems/RenderMainMenuSystem.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"


// Define all static variables
const std::string MenuState::menuID = "MENU";
std::string MenuState::player1Name = "";
std::string MenuState::player2Name = "";
std::string MenuState::player3Name = "";

bool MenuState::slotsFull = false;
bool MenuState::eliminate = false;

MenuState::MenuState()
	: full(false), slot1(1), slot2(2), slot3(3), game(Game::Instance()), reg(Registry::Instance())
{
}

void MenuState::Update(const double& deltaTime)
{
	game.GetEventManager()->Reset();
	reg.GetSystem<KeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	reg.GetSystem<GamePadSystem>().SubscribeToEvents(game.GetEventManager());
	

	reg.Update();

	if (slotsFull && !full)
	{
		Entity slotsFull = reg.CreateEntity();
		//slotsFull.AddComponent<TextLabelComponent>(glm::vec2(300, 900), " REGISTERS ARE FULL! ", "charriot-font-40", SDL_Color{ 255,0,0,0 }, true);
		full = true;
	}
}

void MenuState::Render()
{
	//reg.GetSystem<RenderTextSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	reg.GetSystem<RenderMainMenuSystem>().Update(game.GetRenderer(), game.GetAssetManager());
}

bool MenuState::OnEnter()
{
	LevelLoader loader;
	full = false;

	//if (!reg.HasSystem<MenuKeyboardControlSystem>()) reg.AddSystem<MenuKeyboardControlSystem>();
	//if (!reg.HasSystem<RenderTextSystem>()) reg.AddSystem<RenderTextSystem>();
	if (!reg.HasSystem<RenderMainMenuSystem>()) reg.AddSystem<RenderMainMenuSystem>();

	loader.LoadAssetsFromLuaTable(lua, "menu_state_assets");
	loader.LoadMenuUIFromLuaTable(lua, "menu_state_load");

	game.GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), "Main_Menu", -1);

	loader.LoadMenuScreenFromLuaTable(lua, "save1");
	loader.LoadMenuScreenFromLuaTable(lua, "save2");
	loader.LoadMenuScreenFromLuaTable(lua, "save3");
	
	auto selector = reg.GetEntityByTag("selector");
	
	if (!selector.HasComponent<KeyboardControlComponent>())
		selector.AddComponent<KeyboardControlComponent>();

	return true;
}

bool MenuState::OnExit()
{
	reg.GetSystem<RenderMainMenuSystem>().OnExit();
	//reg.GetSystem<RenderTextSystem>().OnExit();

	//reg.RemoveSystem<RenderMainMenuSystem>();
	//reg.RemoveSystem<MenuKeyboardControlSystem>();
	return true;
}

void MenuState::ProcessEvents(SDL_Event& event)
{
	//reg.GetSystem<GamePadSystem>().UpdateOtherStates(event);
}

void MenuState::OnKeyDown(SDL_Event* event)
{

}

void MenuState::OnKeyUp(SDL_Event* event)
{
}

void MenuState::OnBtnDown(SDL_Event* event)
{

}

void MenuState::OnBtnUp(SDL_Event* event)
{

}


