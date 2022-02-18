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
#include "../Systems/EditorSystems/MouseControlSystem.h"
#include "../Systems/EditorSystems/RenderEditorGUISystem.h"
#include "../Systems/EditorSystems/RenderEditorSystem.h"
#include <filesystem>

namespace fs = std::filesystem;

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

void MenuState::Update(const float& deltaTime)
{
	game.GetEventManager()->Reset();
	
	Registry::Instance().GetSystem<KeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	Registry::Instance().GetSystem<GamePadSystem>().SubscribeToEvents(game.GetEventManager());
	
	reg.Update();
}

void MenuState::Render()
{
	// Registry::Instance().GetSystem<RenderTextSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
	Registry::Instance().GetSystem<RenderMainMenuSystem>().Update(game.GetRenderer(), game.GetAssetManager());
}

bool MenuState::OnEnter()
{
	reg.Update();
	LevelLoader loader;
	full = false;

	if (!reg.HasSystem<RenderTextSystem>()) reg.AddSystem<RenderTextSystem>();
	if (!reg.HasSystem<RenderSystem>()) reg.AddSystem<RenderSystem>();
	if (!reg.HasSystem<RenderMainMenuSystem>()) reg.AddSystem<RenderMainMenuSystem>();

	loader.LoadAssetsFromLuaTable(lua, "menu_state_assets");
	loader.LoadMenuUIFromLuaTable(lua, "menu_state_load");
	game.GetAssetManager()->AddFonts("charriot-font-40", "./Assets/Fonts/charriot.ttf", 40);
	game.GetAssetManager()->AddFonts("charriot-font-60", "./Assets/Fonts/charriot.ttf", 60);

	Registry::Instance().GetSystem<MusicPlayerSystem>().PlayMusic(game.GetAssetManager(), "Main_Menu", -1);

	// Check to see if the save file exists, if it does call the loader function
	fs::path saves("./Assets/SavedFiles/save1.lua");
	if (fs::status_known(fs::file_status{}) ? fs::exists(fs::file_status{}) : fs::exists(saves))
		loader.LoadMenuScreenFromLuaTable(lua, "save1");
	saves = "./Assets/SavedFiles/save2.lua";
	if (fs::status_known(fs::file_status{}) ? fs::exists(fs::file_status{}) : fs::exists(saves))
		loader.LoadMenuScreenFromLuaTable(lua, "save2");
	saves = "./Assets/SavedFiles/save3.lua";
	if (fs::status_known(fs::file_status{}) ? fs::exists(fs::file_status{}) : fs::exists(saves))
		loader.LoadMenuScreenFromLuaTable(lua, "save3");
	
	auto selector = reg.GetEntityByTag("selector");
	
	if (!selector.HasComponent<KeyboardControlComponent>())
		selector.AddComponent<KeyboardControlComponent>();

	// If we were in the editor system remove these for they are only needed there!
	if (reg.HasSystem<RenderEditorGUISystem>()) reg.RemoveSystem<RenderEditorGUISystem>();
	if (reg.HasSystem<RenderEditorGUISystem>()) reg.RemoveSystem<RenderEditorSystem>();
	if (reg.HasSystem<RenderEditorGUISystem>()) reg.RemoveSystem<MouseControlSystem>();

	// Remove the Render Title System since we are not using it!
	if (reg.HasSystem<RenderTitleSystem>()) reg.RemoveSystem<RenderTitleSystem>();

	return true;
}

bool MenuState::OnExit()
{
	Registry::Instance().GetSystem<RenderMainMenuSystem>().OnExit();

	// Remove Unsued Textures
	game.GetAssetManager()->RemoveTexture("menu_box");
	game.GetAssetManager()->RemoveTexture("main_menu_gui");
	// Remove Unused Music
	game.GetAssetManager()->RemoveMusic("Main_Menu");
	// Remove Unused SoundFXs
	game.GetAssetManager()->RemoveSoundFX("Eliminate");
	return true;
}

void MenuState::ProcessEvents(SDL_Event& event)
{
	
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


