#pragma once
#include "State.h"
#include <sol/sol.hpp>
#include "../Utilities/Timer.h"
#include <glm/glm.hpp>

class GameState : public State
{
public:
	// Statics 
	static bool playerDead;
	static int totalRupees;
	static int totalBombs;
	static int totalKeys;
	static bool firstEntered;
	
	// Constructor
	GameState() {}
	GameState(glm::vec2 cameraOffset);
	
	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return gameID; }
private:
	static const std::string gameID;
	sol::state lua;
	Timer enemyTimer;
	Timer playerTimer;
	glm::vec2 cameraOffset;
};