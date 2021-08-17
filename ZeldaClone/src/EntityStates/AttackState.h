// Attack State
#pragma once 
#include "EntityStates.h"
// #include <sol/sol.hpp>

class AttackState : public EntityState
{
public:
	AttackState();

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual std::string GetStateID() const { return gameId; }

private:
	static const std::string gameId;

};