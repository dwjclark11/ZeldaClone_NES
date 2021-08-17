#pragma once
#include "State.h"
#include <string>
#include <sol/sol.hpp>

class MenuState : public State
{
public:
	static std::string player1Name;
	static std::string player2Name;
	static std::string player3Name;

	static bool slotsFull;

	MenuState() : full(false), slot1(1), slot2(2), slot3(3) {}

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);
	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return menuID;} 
private:
	bool full;
	static const std::string menuID;
	unsigned int slot1;
	unsigned int slot2;
	unsigned int slot3;
	sol::state lua;

};