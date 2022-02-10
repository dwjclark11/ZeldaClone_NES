#pragma once
#include "State.h"
#include "../Editor/EditorFileLoader.h"

class EditorState : public State
{
public:
	EditorState();

	virtual void Update(const double& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;

	virtual void OnKeyDown(SDL_Event* event) override;
	virtual void OnKeyUp(SDL_Event* event) override;
	virtual void OnBtnDown(SDL_Event* event)override {}
	virtual void OnBtnUp(SDL_Event* event)override {}
	virtual std::string GetStateID() const override { return editorID; }
private:
	bool editor;
	bool keyDown;
	static const std::string editorID;
	class Game& game;
	class Registry& reg;
	EditorFileLoader loader;
};