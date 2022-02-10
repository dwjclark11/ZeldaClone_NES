#pragma once
#include "State.h"


class GameOverState : public State
{
private:
	class Game& game;
public:

	GameOverState();
	~GameOverState() {}

	virtual void Update(const double& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;

	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event) override;
	virtual void OnBtnUp(SDL_Event* event) override;

	virtual std::string GetStateID() const override { return gameOverID; }
private:
	static const std::string gameOverID;
};