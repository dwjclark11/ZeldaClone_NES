#pragma once
#include "State.h"
#include "Editor/EditorFileLoader.h"

class EditorState : public State
{
  public:
	EditorState();
	~EditorState() = default;

	virtual void Update( const float& deltaTime ) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents( SDL_Event& event ) override;

	virtual std::string GetStateID() const override { return m_sEditorID; }

  private:
	class Game& m_Game;
	class GameData& m_GameData;
	class InputManager& m_InputManager;
	class Registry& m_Registry;
	EditorFileLoader m_Loader;
	std::unique_ptr<class MouseControlSystem> m_pMouseControl;
	bool m_bEditor, m_bKeyDown;

	const std::string m_sEditorID{ "EDITOR" };
};