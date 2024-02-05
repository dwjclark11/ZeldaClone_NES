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

	// Game Systems Refs
	class TriggerSystem& m_TriggerSystem;
	class CollectItemSystem& m_CollectItemSystem;
	class AnimationSystem& m_AnimationSystem;
	class MovementSystem& m_MovementSystem;
	class ProjectileEmitterSystem& m_ProjectileEmitterSystem;
	class CameraMovementSystem& m_CameraMovementSystem;
	class ProjectileLifeCycleSystem& m_ProjectileLifeCycleSystem;
	class AISystem& m_AISystem;
	class CaptionSystem& m_CaptionSystem;
	class CollisionSystem& m_CollisionSystem;
	class DamageSystem& m_DamageSystem;

	class HealthSystem& m_HealthSystem;
	class RenderSystem& m_RenderSystem;
	class RenderTileSystem& m_RenderTileSystem;
	class RenderHUDSystem& m_RenderHUDSystem;
	class RenderCollisionSystem& m_RenderCollisionSystem;

	SDL_Rect hudRect;
	
	bool m_bPaused;

private:
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