#pragma once
#include "State.h"

class SettingsState : public State
{
private:
	static std::string settingsID;
	class Game& game;
	class GameData& gameData;
	class InputManager& inputManager;

public:
	static int mActionIndex;
	static bool mEnterKey;
	SettingsState();
	~SettingsState() = default;

	virtual void Update(const float& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event); 
	virtual void OnKeyUp(SDL_Event* event);
	virtual void OnBtnDown(SDL_Event* event); 
	virtual void OnBtnUp(SDL_Event* event);

	virtual std::string GetStateID() const override { return settingsID; }

};