#pragma once
#include "State.h"
#include "../Utilities/GameData.h"
class PauseState : public State
{
private:
	bool static firstEnter;
	static const std::string pauseID;

	class Game& game;
	class Registry& reg;
	class InputManager& m_InputManager;
	GameData& gameData;
	GameData::ItemType m_SelectedItem;
	void UpdateSelectedItemSprite();

public:

	PauseState();
	~PauseState() {}

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;
	virtual void ProcessEvents(SDL_Event& event) override;

	virtual std::string GetStateID() const { return pauseID; }
};