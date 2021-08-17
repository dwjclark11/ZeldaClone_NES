#pragma once
#include "State.h"

class SaveGameState : public State
{
	public:
	SaveGameState(){}

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return saveID; }
private:

	static const std::string saveID;
	
};
