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

void NameState::Update(const double& deltaTime)
{
	Game::Instance()->GetEventManager()->Reset();
	Registry::Instance()->GetSystem<NameSelectKeyboardControlSystem>().SubscribeToEvents(Game::Instance()->GetEventManager());
	Registry::Instance()->Update();
	Registry::Instance()->GetSystem<NameSelectKeyboardControlSystem>().Update();
}

void NameState::Render()
{
	Registry::Instance()->GetSystem<RenderNameSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager());
	Registry::Instance()->GetSystem<RenderNameStateTextSystem>().Update(Game::Instance()->GetRenderer(), Game::Instance()->GetAssetManager(), Game::Instance()->GetCamera());
}

bool NameState::OnEnter()
{
	Registry::Instance()->AddSystem<RenderNameStateTextSystem>();
	Registry::Instance()->AddSystem<RenderNameSystem>();

	Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "name-letters", "./Assets/HUDSprites/Name_Letters.png");
	Game::Instance()->GetAssetManager()->AddTextures(Game::Instance()->GetRenderer(), "box", "./Assets/HUDSprites/box.png");

	Entity letters = Registry::Instance()->CreateEntity();
	letters.AddComponent<SpriteComponent>("name-letters", 128, 112, 0, true);
	letters.AddComponent<TransformComponent>(glm::vec2(260, 200), glm::vec2(4,4), 0);
	letters.AddComponent<RegisterNameComponent>();

	Entity box = Registry::Instance()->CreateEntity();
	box.AddComponent<SpriteComponent>("box", 16, 16, 0, true);
	box.AddComponent<TransformComponent>(glm::vec2(260, 200), glm::vec2(4, 4), 0);
	box.AddComponent<RegisterNameComponent>();
	box.Tag("box");

	Entity menuLabel1 = Registry::Instance()->CreateEntity();
	menuLabel1.AddComponent<TextLabelComponent>(glm::vec2(260, 50), " - ENTER NAME - ", "charriot-font-60", SDL_Color{ 255,255,255,255 }, true);

	Entity player1Name = Registry::Instance()->CreateEntity();
	player1Name.AddComponent<TextLabelComponent>(glm::vec2(320, 800), "", "charriot-font-40", SDL_Color{ 255,255,255,255 }, true);
	player1Name.Tag("slot1");

	return true;
}

bool NameState::OnExit()
{
	//Game::Instance()->GetAssetManager()->ClearAssets();
	Registry::Instance()->GetSystem<RenderNameStateTextSystem>().OnExit();
	Registry::Instance()->GetSystem<RenderSystem>().OnExit();
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


