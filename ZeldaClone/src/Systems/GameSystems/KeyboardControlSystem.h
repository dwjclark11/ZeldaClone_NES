#pragma once
#include "../../ECS/ECS.h"

enum directionE {NONE = 0, UP, RIGHT, DOWN, LEFT, ATTACK};

class KeyboardControlSystem : public System
{
private:
	class Game& game;
public:
	static directionE dir;
	static bool keyDown;
	bool attack;
	
	KeyboardControlSystem();

	void SubscribeToEvents(std::unique_ptr<class EventManager>& eventManager);

	// Callback function
	void OnKeyPressed(class KeyPressedEvent& event);
	void UpdatePlayer();

};

