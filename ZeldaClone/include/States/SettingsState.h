#pragma once
#include "States/State.h"

class SettingsState : public State
{
  private:
	class Game& m_Game;
	class GameData& m_GameData;
	class InputManager& m_InputManager;
	const std::string m_sSettingsID{ "SETTINGS" };

  public:
	SettingsState();
	~SettingsState() = default;

	virtual void Update( const float& deltaTime );
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents( SDL_Event& event );

	virtual void OnKeyDown( SDL_Event* event );
	virtual void OnKeyUp( SDL_Event* event );
	virtual void OnBtnDown( SDL_Event* event );
	virtual void OnBtnUp( SDL_Event* event );

	virtual std::string GetStateID() const override { return m_sSettingsID; }
};