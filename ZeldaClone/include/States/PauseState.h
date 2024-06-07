#pragma once
#include "States/State.h"
#include "Utilities/GameData.h"

class PauseState : public State
{
  private:
	const std::string m_sPauseID{ "PAUSE" };

	class Game& m_Game;
	class Registry& m_Registry;
	class InputManager& m_InputManager;
	GameData& m_GameData;
	GameData::ItemType m_SelectedItem;

  private:
	void UpdateSelectedItemSprite();

  public:
	PauseState();
	~PauseState() = default;

	virtual void Update( const float& deltaTime ) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;
	virtual void ProcessEvents( SDL_Event& event ) override;

	virtual std::string GetStateID() const { return m_sPauseID; }
};