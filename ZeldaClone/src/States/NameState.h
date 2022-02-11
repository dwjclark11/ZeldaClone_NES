#pragma once
#include "State.h"

class NameState : public State
{
public:
	static std::string name;
	static int slot;
	static int row;
	static int col;

	NameState();
	~NameState() {}

	virtual void Update(const double& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;

	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event) override;
	virtual void OnBtnUp(SDL_Event* event) override;
	virtual std::string GetStateID() const override{ return nameID; }
private:
	bool editor;
	bool keyDown;
	static const std::string nameID;

	class Game& game;
	class Registry& reg;
	
};