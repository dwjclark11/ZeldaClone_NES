#pragma once
#include "State.h"

class EditorState : public State
{
public:
	EditorState() : editor(false), keyDown(false) {}

	virtual void Update(const double& deltaTime);
	virtual void Render();

	virtual bool OnEnter();
	virtual bool OnExit();

	virtual void ProcessEvents(SDL_Event& event);

	virtual void OnKeyDown(SDL_Event* event);
	virtual void OnKeyUp(SDL_Event* event);

	virtual std::string GetStateID() const { return editorID; }
private:
	bool editor;
	bool keyDown;
	bool bombs;
	static const std::string editorID;
};