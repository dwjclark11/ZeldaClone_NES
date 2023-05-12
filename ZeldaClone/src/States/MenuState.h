#pragma once
#include "State.h"
#include <string>
#include <sol/sol.hpp>

class MenuState : public State
{
private:
	static const std::string menuID;
	unsigned int slot1, slot2, slot3;
	sol::state lua;
	bool m_bEliminate, m_bFull, m_bSlotsFull;

	class Game& game;
	class GameData& m_GameData;
	class InputManager& m_InputManager;
	class Registry& reg;

	void EliminateKeys();
	void SelectorKeys();

public:
	MenuState();
	virtual ~MenuState() { Logger::Err(GetStateID() + " was Destroyed!"); };

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;
	virtual std::string GetStateID() const { return menuID;} 
};