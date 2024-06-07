#pragma once
#include "State.h"

class GameOverState : public State
{
  private:
	class Game& m_Game;
	class GameData& m_GameData;
	class InputManager& m_InputManager;

  public:
	GameOverState();
	~GameOverState() {}

	virtual void Update( const float& deltaTime ) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents( SDL_Event& event ) override;

	virtual std::string GetStateID() const override { return m_sGameOverID; }

  private:
	static const std::string m_sGameOverID;
};