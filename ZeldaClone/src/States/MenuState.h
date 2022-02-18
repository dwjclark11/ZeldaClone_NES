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
	static bool eliminate;



	MenuState();
	virtual ~MenuState() { Logger::Err(GetStateID() + " was Destroyed!"); };

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;
	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event) override;
	virtual void OnBtnUp(SDL_Event* event) override;
	virtual std::string GetStateID() const { return menuID;} 

private:
	bool full;
	static const std::string menuID;
	unsigned int slot1;
	unsigned int slot2;
	unsigned int slot3;
	sol::state lua;

	class Game& game;
	class Registry& reg;

};