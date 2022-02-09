#pragma once
#include "../../ECS/ECS.h"
#include <SDL.h>

class GamePadSystem : public System
{
private:
	class Game& game;
public:
	SDL_GameController* gameController;
	const int JOYSTICK_DEAD_ZONE = 25000;
	
	/*
		Should this be a struct?
	*/
	static bool xPressed;
	static bool yPressed;
	static bool aPressed;
	static bool bPressed;
	static bool upPressed;
	static bool downPressed;
	static bool leftPressed;
	static bool rightPressed;
	static bool buttonDown;
	static bool buttonDirDown;
	static bool paused;

	GamePadSystem();

	void Init();
	void Update(SDL_Event& event);
	void UpdateOtherStates(SDL_Event& event);

private:

};
