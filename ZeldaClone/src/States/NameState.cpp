#include "NameState.h"

#include "../ECS/ECS.h"
#include "../Systems/GameSystems/RenderSystem.h"
#include "../Systems/NameSystems/RenderNameStateTextSystem.h"
#include "../Systems/NameSystems/RenderNameSystem.h"
#include "../Systems/NameSystems/NameSelectKeyboardControlSystem.h"
#include "../Components/RegisterNameComponent.h"
#include "../States/MenuState.h"
#include "../Game/Game.h"

const std::string NameState::nameID = "NAME";
std::string NameState::name = "";

NameState::NameState()
	: editor(false), keyDown(false), game(*Game::Instance()), reg(*Registry::Instance())
{
}

void NameState::Update(const double& deltaTime)
{
	game.GetEventManager()->Reset();
	reg.GetSystem<NameSelectKeyboardControlSystem>().SubscribeToEvents(game.GetEventManager());
	reg.Update();
	reg.GetSystem<NameSelectKeyboardControlSystem>().Update();
}

void NameState::Render()
{
	reg.GetSystem<RenderNameSystem>().Update(game.GetRenderer(), game.GetAssetManager());
	reg.GetSystem<RenderNameStateTextSystem>().Update(game.GetRenderer(), game.GetAssetManager(), game.GetCamera());
}

bool NameState::OnEnter()
{
	reg.AddSystem<RenderNameStateTextSystem>();
	reg.AddSystem<RenderNameSystem>();

	game.GetAssetManager()->AddTextures(game.GetRenderer(), "name-letters", "./Assets/HUDSprites/Name_Letters.png");
	game.GetAssetManager()->AddTextures(game.GetRenderer(), "box", "./Assets/HUDSprites/box.png");

	Entity letters = reg.CreateEntity();
	letters.AddComponent<SpriteComponent>("name-letters", 128, 112, 0, true);
	letters.AddComponent<TransformComponent>(glm::vec2(260, 200), glm::vec2(4,4), 0);
	letters.AddComponent<RegisterNameComponent>();

	Entity box = reg.CreateEntity();
	box.AddComponent<SpriteComponent>("box", 16, 16, 0, true);
	box.AddComponent<TransformComponent>(glm::vec2(260, 200), glm::vec2(4, 4), 0);
	box.AddComponent<RegisterNameComponent>();
	box.Tag("box");

	//Entity menuLabel1 = reg.CreateEntity();
	//menuLabel1.AddComponent<TextLabelComponent>(glm::vec2(260, 50), " - ENTER NAME - ", "charriot-font-60", SDL_Color{ 255,255,255,255 }, true);

	Entity player1Name = reg.CreateEntity();
	player1Name.AddComponent<TextLabelComponent>(glm::vec2(320, 800), "", "charriot-font-40", SDL_Color{ 255,255,255,255 }, true);
	player1Name.Tag("slot1");

	return true;
}

bool NameState::OnExit()
{
	//game.GetAssetManager()->ClearAssets();
	reg.GetSystem<RenderNameStateTextSystem>().OnExit();
	//reg.GetSystem<RenderSystem>().OnExit();
	//Logger::Err("Exiting Name state");
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


