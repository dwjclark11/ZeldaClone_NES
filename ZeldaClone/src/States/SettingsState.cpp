#include "SettingsState.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/SettingsComponent.h"
#include "../Components/GamePadComponent.h"
#include "../Components/GameComponent.h"

#include "../Components/KeyboardControlComponent.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"
#include "../Systems/GameSystems/RenderSystem.h"
#include "../Systems/GameSystems/GamePadSystem.h"

#include "MenuState.h"
#include "../Game/Game.h"
#include "../AssetManager/AssetManager.h"
#include "../Logger/Logger.h"
#include <string>
std::string SettingsState::settingsID = "SETTINGS";
int SettingsState::mActionIndex = 0;
bool SettingsState::mEnterKey = false;

SettingsState::SettingsState()
{
}

void SettingsState::Update(const double& deltaTime)
{
	Game::Instance().GetEventManager()->Reset();

	Registry::Instance().GetSystem<KeyboardControlSystem>().SubscribeToEvents(Game::Instance().GetEventManager());
	Registry::Instance().GetSystem<GamePadSystem>().SubscribeToEvents(Game::Instance().GetEventManager());

	Registry::Instance().Update();
}

void SettingsState::Render()
{
	SDL_Rect KeyRect = { 350, 215, 300, 500 };
	SDL_Rect KeyRectOutline = { 350, 215, 300, 500 };

	SDL_Rect BtnRect = { 650, 215, 300, 500 };
	SDL_Rect BtnRectOutline = { 650, 215, 300, 500 };
	
	SDL_SetRenderDrawColor(Game::Instance().GetRenderer(), 70, 70, 70, 70);
	SDL_RenderFillRect(Game::Instance().GetRenderer(), &KeyRect);
	SDL_RenderDrawRect(Game::Instance().GetRenderer(), &KeyRect);
	SDL_RenderFillRect(Game::Instance().GetRenderer(), &BtnRect);
	SDL_RenderDrawRect(Game::Instance().GetRenderer(), &BtnRect);

	SDL_SetRenderDrawColor(Game::Instance().GetRenderer(), 100, 100, 100, 150);
	SDL_RenderDrawRect(Game::Instance().GetRenderer(), &KeyRectOutline);
	SDL_RenderDrawRect(Game::Instance().GetRenderer(), &BtnRectOutline);

	Registry::Instance().GetSystem<RenderTextSystem>().Update(Game::Instance().GetRenderer(), 
		Game::Instance().GetAssetManager(), Game::Instance().GetCamera());
	Registry::Instance().GetSystem<RenderSystem>().Update(Game::Instance().GetRenderer(), Game::Instance().GetAssetManager(), Game::Instance().GetCamera());
}

bool SettingsState::OnEnter()
{

	Game::Instance().GetAssetManager()->AddSoundFX("bomb_drop", "./Assets/sounds/Bomb_Drop.wav");
	Game::Instance().GetAssetManager()->AddSoundFX("get_item", "./Assets/sounds/Get_Item.wav");

	std::string attackKey = std::string(SDL_GetKeyName(Game::Instance().GetKeyBindings().at(Game::Action::ATTACK)));
	std::string moveUpKey = std::string(SDL_GetKeyName(Game::Instance().GetKeyBindings().at(Game::Action::MOVE_UP)));
	std::string moveDownKey = std::string(SDL_GetKeyName(Game::Instance().GetKeyBindings().at(Game::Action::MOVE_DOWN)));
	std::string moveLeftKey = std::string(SDL_GetKeyName(Game::Instance().GetKeyBindings().at(Game::Action::MOVE_LEFT)));
	std::string moveRightKey = std::string(SDL_GetKeyName(Game::Instance().GetKeyBindings().at(Game::Action::MOVE_RIGHT)));
	std::string useItemKey = std::string(SDL_GetKeyName(Game::Instance().GetKeyBindings().at(Game::Action::USE_ITEM)));

	std::string attackBtn = std::string(SDL_GameControllerGetStringForButton(Game::Instance().GetBtnBindings().at(Game::Action::ATTACK)));
	std::string moveUpBtn = std::string(SDL_GameControllerGetStringForButton(Game::Instance().GetBtnBindings().at(Game::Action::MOVE_UP)));
	std::string moveDownBtn = std::string(SDL_GameControllerGetStringForButton(Game::Instance().GetBtnBindings().at(Game::Action::MOVE_DOWN)));
	std::string moveLeftBtn = std::string(SDL_GameControllerGetStringForButton(Game::Instance().GetBtnBindings().at(Game::Action::MOVE_LEFT)));
	std::string moveRightBtn = std::string(SDL_GameControllerGetStringForButton(Game::Instance().GetBtnBindings().at(Game::Action::MOVE_RIGHT)));
	std::string useItemBtn = std::string(SDL_GameControllerGetStringForButton(Game::Instance().GetBtnBindings().at(Game::Action::USE_ITEM)));


	auto action = Registry::Instance().CreateEntity();
	action.AddComponent<TextLabelComponent>(glm::vec2(100, 150), "ACTION", "charriot-font-60", SDL_Color{255, 255, 255, 255});
	action.AddComponent<SettingsComponent>();
	auto key = Registry::Instance().CreateEntity();
	key.AddComponent<TextLabelComponent>(glm::vec2(400, 150), "KEY", "charriot-font-60", SDL_Color{ 255, 255, 255, 255 });
	key.AddComponent<SettingsComponent>();
	auto btn = Registry::Instance().CreateEntity();
	btn.AddComponent<TextLabelComponent>(glm::vec2(700, 150), "BUTTON", "charriot-font-60", SDL_Color{ 255, 255, 255, 255 });
	btn.AddComponent<SettingsComponent>();
	
	auto upACT = Registry::Instance().CreateEntity();
	upACT.AddComponent<TextLabelComponent>(glm::vec2(400, 225), moveUpKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	upACT.AddComponent<KeyboardControlComponent>();
	upACT.AddComponent<SettingsComponent>(0, SettingsComponent::Input::KEY);

	auto downACT = Registry::Instance().CreateEntity();
	downACT.AddComponent<TextLabelComponent>(glm::vec2(400, 300), moveDownKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	downACT.AddComponent<KeyboardControlComponent>();
	downACT.AddComponent<SettingsComponent>(1, SettingsComponent::Input::KEY);

	auto leftACT = Registry::Instance().CreateEntity();
	leftACT.AddComponent<TextLabelComponent>(glm::vec2(400, 375), moveLeftKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	leftACT.AddComponent<KeyboardControlComponent>();
	leftACT.AddComponent<SettingsComponent>(2, SettingsComponent::Input::KEY);

	auto rightACT = Registry::Instance().CreateEntity();
	rightACT.AddComponent<TextLabelComponent>(glm::vec2(400, 450), moveRightKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	rightACT.AddComponent<KeyboardControlComponent>();
	rightACT.AddComponent<SettingsComponent>(3, SettingsComponent::Input::KEY);

	auto attackACT = Registry::Instance().CreateEntity();
	attackACT.AddComponent<TextLabelComponent>(glm::vec2(400, 525), attackKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	attackACT.AddComponent<KeyboardControlComponent>();
	attackACT.AddComponent<SettingsComponent>(4, SettingsComponent::Input::KEY);

	auto useItemACT = Registry::Instance().CreateEntity();
	useItemACT.AddComponent<TextLabelComponent>(glm::vec2(400, 600), useItemKey, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	useItemACT.AddComponent<KeyboardControlComponent>();
	useItemACT.AddComponent<SettingsComponent>(5, SettingsComponent::Input::KEY);


	auto upACTBtn = Registry::Instance().CreateEntity();
	upACTBtn.AddComponent<TextLabelComponent>(glm::vec2(700, 225), moveUpBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	upACTBtn.AddComponent<GamePadComponent>();
	upACTBtn.AddComponent<SettingsComponent>(0, SettingsComponent::Input::BUTTON);

	auto downACTBtn = Registry::Instance().CreateEntity();
	downACTBtn.AddComponent<TextLabelComponent>(glm::vec2(700, 300), moveDownBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	downACTBtn.AddComponent<GamePadComponent>();
	downACTBtn.AddComponent<SettingsComponent>(1, SettingsComponent::Input::BUTTON);

	auto leftACTBtn = Registry::Instance().CreateEntity();
	leftACTBtn.AddComponent<TextLabelComponent>(glm::vec2(700, 375), moveLeftBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	leftACTBtn.AddComponent<GamePadComponent>();
	leftACTBtn.AddComponent<SettingsComponent>(2, SettingsComponent::Input::BUTTON);

	auto rightACTBtn = Registry::Instance().CreateEntity();
	rightACTBtn.AddComponent<TextLabelComponent>(glm::vec2(700, 450), moveRightBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	rightACTBtn.AddComponent<GamePadComponent>();
	rightACTBtn.AddComponent<SettingsComponent>(3, SettingsComponent::Input::BUTTON);

	auto attackACTBtn = Registry::Instance().CreateEntity();
	attackACTBtn.AddComponent<TextLabelComponent>(glm::vec2(700, 525), attackBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	attackACTBtn.AddComponent<GamePadComponent>();
	attackACTBtn.AddComponent<SettingsComponent>(4, SettingsComponent::Input::BUTTON);

	auto useItemACTBtn = Registry::Instance().CreateEntity();
	useItemACTBtn.AddComponent<TextLabelComponent>(glm::vec2(700, 600), useItemBtn, "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	useItemACTBtn.AddComponent<GamePadComponent>();
	useItemACTBtn.AddComponent<SettingsComponent>(5, SettingsComponent::Input::BUTTON);

	auto moveUpWord = Registry::Instance().CreateEntity();
	moveUpWord.AddComponent<TextLabelComponent>(glm::vec2(100, 225), "MOVE UP", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	moveUpWord.AddComponent<SettingsComponent>();
	
	auto moveDownWord = Registry::Instance().CreateEntity();
	moveDownWord.AddComponent<TextLabelComponent>(glm::vec2(100, 300), "MOVE DOWN", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	moveDownWord.AddComponent<SettingsComponent>();
	
	auto moveLeftWord = Registry::Instance().CreateEntity();
	moveLeftWord.AddComponent<TextLabelComponent>(glm::vec2(100, 375), "MOVE LEFT", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	moveLeftWord.AddComponent<SettingsComponent>();
	
	auto moveRightWord = Registry::Instance().CreateEntity();
	moveRightWord.AddComponent<TextLabelComponent>(glm::vec2(100, 450), "MOVE RIGHT", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	moveRightWord.AddComponent<SettingsComponent>();
	
	auto attackWord = Registry::Instance().CreateEntity();
	attackWord.AddComponent<TextLabelComponent>(glm::vec2(100, 525), "ATTACK", "charriot-font-40", SDL_Color{255, 255, 255, 255});
	attackWord.AddComponent<SettingsComponent>();
	
	auto useItemWord = Registry::Instance().CreateEntity();
	useItemWord.AddComponent<TextLabelComponent>(glm::vec2(100, 600), "USE ITEM", "charriot-font-40", SDL_Color{ 255, 255, 255, 255 });
	useItemWord.AddComponent<SettingsComponent>();

	auto selector = Registry::Instance().CreateEntity();
	selector.AddComponent<SpriteComponent>("hearts", 16, 16, 0, true);
	selector.AddComponent<TransformComponent>(glm::vec2(50, 225), glm::vec2(4, 4), 0);
	selector.AddComponent<SettingsComponent>();
	selector.AddComponent<GameComponent>();
	selector.AddComponent<GamePadComponent>();
	selector.AddComponent<KeyboardControlComponent>();
	selector.AddComponent<TextLabelComponent>(glm::vec2(200, 800), "Choose an Action to Change!", "charriot-font-40", SDL_Color{ 255,255,255,255 }, true);
	selector.Tag("settings_selector");

	return true;
}

bool SettingsState::OnExit()
{
	Registry::Instance().GetSystem<RenderTextSystem>().OnExitSettings();
	Game::Instance().GetAssetManager()->RemoveSoundFX("bomb_drop");
	Game::Instance().GetAssetManager()->RemoveSoundFX("get_item" );

	return true;
}

void SettingsState::ProcessEvents(SDL_Event& event)
{

}

void SettingsState::OnKeyDown(SDL_Event* event)
{

}

void SettingsState::OnKeyUp(SDL_Event* event)
{
	if (event->key.keysym.sym == Game::Instance().GetKeyBindings().at(Game::Action::CANCEL) && !mEnterKey)
	{
		Game::Instance().GetStateMachine()->PopState();
		Game::Instance().GetStateMachine()->PushState(new MenuState());
	}
}

void SettingsState::OnBtnDown(SDL_Event* event)
{
	if (event->cbutton.button == Game::Instance().GetBtnBindings().at(Game::Action::CANCEL) && !mEnterKey)
	{
		Game::Instance().GetStateMachine()->PopState();
		Game::Instance().GetStateMachine()->PushState(new MenuState());
	}
}

void SettingsState::OnBtnUp(SDL_Event* event)
{

}
