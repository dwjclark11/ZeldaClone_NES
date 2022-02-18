#pragma once
#include "State.h"

class PauseState : public State
{
public:

	PauseState();
	~PauseState() {}

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;
	virtual void ProcessEvents(SDL_Event& event) override;

	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event) override;
	virtual void OnBtnUp(SDL_Event* event) override;


	virtual std::string GetStateID() const { return pauseID; }
private:
	bool bombs;

	bool static firstEnter;
	static const std::string pauseID;

	class Game& game;
	class Registry& reg;
};