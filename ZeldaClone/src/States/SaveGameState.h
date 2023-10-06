#pragma once
#include "State.h"

class SaveGameState : public State
{
private:
	static const std::string saveID;
	class Game& game;
	class GameData& m_GameData;
	class InputManager& m_InputManager;
	class Registry& reg;

public:
	SaveGameState();
	~SaveGameState() = default;

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;
	virtual std::string GetStateID() const override{ return saveID; }
};
