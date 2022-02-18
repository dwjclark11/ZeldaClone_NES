#pragma once
#include "State.h"

class TitleState : public State
{
public:
	TitleState();
	~TitleState() { Logger::Err(GetStateID() + " was Destroyed!"); };

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit()  override;

	virtual void ProcessEvents(SDL_Event& event) override;

	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event) override;
	virtual void OnBtnUp(SDL_Event* event) override;
	virtual std::string GetStateID() const override { return titleID; }


private:
	class Game& game;
	class Registry& reg;

	int timer;
	bool scroll;
	
	static const std::string titleID;

	
};