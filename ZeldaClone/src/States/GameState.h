#pragma once
#include "State.h"
#include <sol/sol.hpp>
#include "../Utilities/Timer.h"
#include <glm/glm.hpp>
#include "PauseState.h"
#include "../StateMachines/PlayerStateMachine.h"


class GameState : public State
{
public:
	// Statics 
	static int totalRupees;
	static int totalPrevRupees;
	static int totalBombs;
	static int totalPrevBombs;
	static int totalKeys;
	static int totalPrevKeys;
	static bool firstEntered;
	static bool unpause;
	// Constructor
	GameState();
	~GameState();
	GameState(glm::vec2 cameraOffset);
	
	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return gameID; }
	
	void ConvertHUDNumbers();
	
private:
	static const std::string gameID;
	glm::vec2 cameraOffset;
	class Game& game;
	class Registry& reg;
	
};