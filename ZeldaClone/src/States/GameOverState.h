#pragma once
#include "State.h"


class GameOverState : public State
{
private:
	class Game& game;
public:

	GameOverState();
	~GameOverState() {}

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return gameOverID; }
private:
	static const std::string gameOverID;
};