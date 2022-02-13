#pragma once
#include "../../ECS/ECS.h"
#include <SDL.h>
#include "../../Events/GamePadButtonPressedEvent.h"

class GamePadSystem : public System
{
private:
	class Game& game;
	void GameStateBtns(GamePadButtonPressedEvent& event);
	void MenuStateBtns(GamePadButtonPressedEvent& event);
	void SettingsStateBtns(GamePadButtonPressedEvent& event);
	void NameStateBtns(GamePadButtonPressedEvent& event);
	void PauseStateBtns(GamePadButtonPressedEvent& event);
	void OnButtonPressed(GamePadButtonPressedEvent& event);
public:
	SDL_GameController* gameController;
	const int JOYSTICK_DEAD_ZONE = 25000;
	
	static bool paused;

	GamePadSystem();

	void Init();
	void SubscribeToEvents(std::unique_ptr<class EventManager>& eventManager);

private:

};
