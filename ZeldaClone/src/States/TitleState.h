#pragma once
#include "State.h"

class TitleState : public State
{
private:
	class Game& game;
	class Registry& reg;

	int timer;
	bool scroll;

	static const std::string titleID;

public:
	TitleState();
	~TitleState() = default;

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit()  override;

	virtual void ProcessEvents(SDL_Event& event) override;
	virtual std::string GetStateID() const override { return titleID; }
};