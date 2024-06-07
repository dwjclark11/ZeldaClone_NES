#pragma once
#include <vector>
#include <SDL.h>
#include <memory>
#include <string>

class State;

class GameStateMachine
{
  private:
	std::vector<std::unique_ptr<State>> m_States;

  public:
	GameStateMachine() {}
	~GameStateMachine() {}

	void Update( const float deltaTime );
	void Render();

	void PushState( std::unique_ptr<State> state, bool bReplace = true );
	void PopState();
	void ClearStates();

	std::string GetCurrentState();
	void ProcessEvents( SDL_Event& event );
};