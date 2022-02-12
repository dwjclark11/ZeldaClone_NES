#include "SettingsState.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextLabelComponent.h"
#include "MenuState.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"

std::string SettingsState::settingsID = "SETTINGS";

SettingsState::SettingsState()
{
}

void SettingsState::Update(const double& deltaTime)
{
	Game::Instance().GetEventManager()->Reset();
	Registry::Instance().Update();
}

void SettingsState::Render()
{
}

bool SettingsState::OnEnter()
{
	Logger::Err("Entering Settings State!");
	return true;
}

bool SettingsState::OnExit()
{
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
	if (event->key.keysym.sym == Game::Instance().GetKeyBindings().at(Game::Action::MOVE_LEFT))
	{
		Game::Instance().GetStateMachine()->PopState();
		Game::Instance().GetStateMachine()->PushState(new MenuState());
	}
}

void SettingsState::OnBtnDown(SDL_Event* event)
{
}

void SettingsState::OnBtnUp(SDL_Event* event)
{
}
