#pragma once
#include "State.h"
#include <sol/sol.hpp>
#include "../Utilities/Timer.h"
#include <glm/glm.hpp>
#include "PauseState.h"
//#include "../StateMachines/PlayerStateMachine.h"


class GameState : public State
{
public:
	// Statics 
	static int scrollRupees;
	static int totalRupees;
	static int totalPrevRupees;
	static int totalBombs;
	static int totalPrevBombs;
	static int totalKeys;
	static int totalPrevKeys;
	static bool firstEntered;
	static bool unpause;
	static bool buyItem;

	// Constructor
	GameState();
	~GameState();

	GameState(glm::vec2 cameraOffset);
	
	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;

	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event);
	virtual void OnBtnUp(SDL_Event* event) override;

	virtual std::string GetStateID() const { return gameID; }
	
	
private:
	static const std::string gameID;
	glm::vec2 cameraOffset;
	class Game& game;
	class Registry& reg;

	Timer rupeeTimer;
	int index;
	const int MAX_RUPEES = 255;
	// Private Gamestate Helper Functions
	void ConvertHUDNumbers();
	void RupeeScroll();
	
};