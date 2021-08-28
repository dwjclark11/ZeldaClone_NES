#pragma once
#include "State.h"
#include <sol/sol.hpp>

class DungeonState : public State
{
public:
	
	DungeonState() : paused(false) {}
	DungeonState(std::string level);
	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return dungeonID; }
private:
	static const std::string dungeonID;
	std::string level;
	bool paused;
	sol::state lua;
};