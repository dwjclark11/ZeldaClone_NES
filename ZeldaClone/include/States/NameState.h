#pragma once
#include "States/State.h"

class NameState : public State
{
  private:
	static const std::string nameID;
	std::string m_sName;
	int m_Slot, m_Row, m_Col;
	bool m_bEditor, m_bKeyDown;

	class Game& game;
	class GameData& m_GameData;
	class Registry& reg;
	class InputManager& m_InputManager;

  public:
	NameState();
	~NameState() {}

	virtual void Update( const float& deltaTime ) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents( SDL_Event& event ) override;
	virtual std::string GetStateID() const override { return nameID; }
};