#pragma once
#include "State.h"

class SaveGameState : public State
{
public:
	SaveGameState();
	~SaveGameState() {}

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;

	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event) override;
	virtual void OnBtnUp(SDL_Event* event) override;
	virtual std::string GetStateID() const override{ return saveID; }
private:

	static const std::string saveID;
	class Game& game;
	class Registry& reg;
};
