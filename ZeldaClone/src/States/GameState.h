#pragma once
#include "State.h"
#include <sol/sol.hpp>
#include "../Utilities/Timer.h"
#include <glm/glm.hpp>
#include "PauseState.h"

class GameState : public State
{
private:
	static const std::string gameID;
	glm::vec2 cameraOffset;
	class Game& game;
	class GameData& gameData;
	class Registry& reg;
	class InputManager& inputManager;

	SDL_Rect hudRect;
	void UpdatePlayerKeys();
	void UpdatePauseContol();
public:	
	static bool firstEntered;

	// Constructor
	GameState();
	~GameState();

	GameState(glm::vec2 cameraOffset);
	
	virtual void Update(const float& deltaTime) override;
	virtual void Render() override;

	virtual bool OnEnter() override;
	virtual bool OnExit() override;

	virtual void ProcessEvents(SDL_Event& event) override;
	virtual std::string GetStateID() const { return gameID; }
};