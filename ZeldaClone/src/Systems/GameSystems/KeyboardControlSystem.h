#pragma once
#include "../../ECS/ECS.h"

enum directionE {NONE = 0, UP, RIGHT, DOWN, LEFT, ATTACK};

class KeyboardControlSystem : public System
{
private:
	class Game& game;

	void MenuStateKeys(class KeyPressedEvent& event);
	void EditorStateKeys(class KeyPressedEvent& event);
	void PauseStateKeys(class KeyPressedEvent& event);
	void NameStateKeys(class KeyPressedEvent& event);
	void GameStateKeys(class KeyPressedEvent& event);
	void SaveStateKeys(class KeyPressedEvent& event);

public:
	static directionE dir;
	static bool keyDown;
	bool attack;
	
	KeyboardControlSystem();

	void SubscribeToEvents(std::unique_ptr<class EventManager>& eventManager);

	// Callback function
	void OnKeyPressed(class KeyPressedEvent& event);
	void UpdatePlayer();
	void Update();
};

