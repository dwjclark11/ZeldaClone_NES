#include "NameState.h"
#include "../ECS/ECS.h"
#include "../Systems/GameSystems/RenderSystem.h"
#include "../Systems/NameSystems/RenderNameStateTextSystem.h"
#include "../Systems/NameSystems/RenderNameSystem.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"
#include "../Systems/GameSystems/GamePadSystem.h"
#include "../Components/RegisterNameComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../States/MenuState.h"
#include "../AssetManager/AssetManager.h"
#include "../Game/Game.h"

const std::string NameState::nameID = "NAME";
std::string NameState::name = "";

int NameState::slot = 0;
int NameState::row = 0;
int NameState::col = 0;

NameState::NameState()
	: editor(false), keyDown(false), game(Game::Instance()), reg(Registry::Instance())
{
}

void NameState::Update(const float& deltaTime)
{
	game.GetEventManager()->Reset();
	
	Registry::Instance().GetSystem<KeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	Registry::Instance().GetSystem<GamePadSystem>().SubscribeToEvents(game.GetEventManager());
	reg.Update();
}

void NameState::Render()
{
	Registry::Instance().GetSystem<RenderNameSystem>().Update(game.GetRenderer(), game.GetAssetManager());
	Registry::Instance().GetSystem<RenderNameStateTextSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
}

bool NameState::OnEnter()
{
	reg.AddSystem<RenderNameStateTextSystem>();
	reg.AddSystem<RenderNameSystem>();

	game.GetAssetManager()->AddTextures(game.GetRenderer(), "name-letters", "./Assets/HUDSprites/Name_Letters.png");
	game.GetAssetManager()->AddTextures(game.GetRenderer(), "box", "./Assets/HUDSprites/box.png");
	game.GetAssetManager()->AddFonts("charriot-font-120", "./Assets/Fonts/charriot.ttf", 120);

	Entity letters = reg.CreateEntity();
	letters.AddComponent<SpriteComponent>("name-letters", 128, 112, 0, true);
	letters.AddComponent<TransformComponent>(glm::vec2(260, 200), glm::vec2(4,4), 0);
	letters.AddComponent<RegisterNameComponent>();

	Entity box = reg.CreateEntity();
	box.AddComponent<SpriteComponent>("box", 16, 16, 0, true);
	box.AddComponent<TransformComponent>(glm::vec2(260, 200), glm::vec2(4, 4), 0);
	box.AddComponent<RegisterNameComponent>();
	box.AddComponent<KeyboardControlComponent>();
	box.AddComponent<TextLabelComponent>(glm::vec2(300, 800), "", "charriot-font-120", SDL_Color{ 255,255,255,255 }, true);
	box.Tag("box");

	return true;
}

bool NameState::OnExit()
{
	Registry::Instance().GetSystem<RenderNameStateTextSystem>().OnExit();

	return true;
}

void NameState::ProcessEvents(SDL_Event& event)
{

}

void NameState::OnKeyDown(SDL_Event* event)
{

}

void NameState::OnKeyUp(SDL_Event* event)
{

}

void NameState::OnBtnDown(SDL_Event* event)
{

}

void NameState::OnBtnUp(SDL_Event* event)
{
}


