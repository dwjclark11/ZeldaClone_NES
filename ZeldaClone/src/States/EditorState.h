#pragma once
#include "State.h"
#include "../Editor/EditorFileLoader.h"

class EditorState : public State
{
public:
	EditorState();

	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;

	virtual std::string GetStateID() const override { return editorID; }
private:
	bool editor;
	bool keyDown;
	static const std::string editorID;
	class Game& game;
	class GameData& gameData;
	class InputManager& inputManager;
	class Registry& reg;
	EditorFileLoader loader;
};