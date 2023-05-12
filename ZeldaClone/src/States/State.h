#pragma once
#include <SDL.h>
#include <string>
#include "GameStateMachine.h"
#include "../Logger/Logger.h"

class State
{
public:
	virtual ~State() { Logger::Err("State Destroyed!!"); }
	

	virtual void Update(const float& deltaTime) = 0;
	virtual void Render() = 0;

	virtual bool OnEnter() = 0;
	virtual bool OnExit() = 0;

	virtual void ProcessEvents(SDL_Event& event) = 0;

	virtual std::string GetStateID() const = 0;

};

