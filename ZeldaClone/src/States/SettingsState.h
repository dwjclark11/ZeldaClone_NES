#pragma once
#include "State.h"

class SettingsState : public State
{
public:
	SettingsState();
	virtual ~SettingsState() { Logger::Err("Settings State Destroyed!!"); }

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event); 
	virtual void OnKeyUp(SDL_Event* event);
	virtual void OnBtnDown(SDL_Event* event); 
	virtual void OnBtnUp(SDL_Event* event);

	virtual std::string GetStateID() const override { return settingsID; }
private:
	static std::string settingsID;
};