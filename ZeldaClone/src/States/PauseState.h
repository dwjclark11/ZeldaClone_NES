#pragma once
#include "State.h"

class PauseState : public State
{
public:

	PauseState() {}

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();
	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return pauseID; }
private:
	bool bombs;

	bool static firstEnter;
	static const std::string pauseID;
};