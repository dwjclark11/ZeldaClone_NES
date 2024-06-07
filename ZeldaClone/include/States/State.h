#pragma once
#include <SDL.h>
#include <string>

class State
{
  public:
	virtual ~State() = default;

	virtual void Update( const float& deltaTime ) = 0;
	virtual void Render() = 0;

	virtual bool OnEnter() = 0;
	virtual bool OnExit() = 0;

	virtual void ProcessEvents( SDL_Event& event ) = 0;

	virtual std::string GetStateID() const = 0;
};
