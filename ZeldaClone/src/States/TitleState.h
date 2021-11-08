#pragma once
#include "State.h"
#include "../Systems/GameSystems/AnimationSystem.h"
class TitleState : public State
{
public:
	TitleState();
	~TitleState() {};

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return titleID; }


private:
	int timer;
	bool scroll;
	static const std::string titleID;

	class Game& game;
	class Registry& reg;
};