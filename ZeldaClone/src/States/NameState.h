#pragma once

#include "State.h"

class NameState : public State
{
public:
	static std::string name;

	NameState();
	~NameState() {}

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return nameID; }
private:
	bool editor;
	bool keyDown;
	static const std::string nameID;

	class Game& game;
	class Registry& reg;
	
};