#pragma once
#include "ECS/ECS.h"
#include "Utilities/Timer.h"

class Camera;

class CameraMovementSystem : public System
{
  private:
	class Game& m_Game;
	class GameData& m_GameData;

	int m_CamSpeed, m_PrevCamX, m_PrevCamY;
	bool m_bStartEast, m_bStartWest, m_bStartNorth, m_bStartSouth;

  public:
	CameraMovementSystem();
	~CameraMovementSystem() = default;

	void UpdatePlayerCam( Camera& camera, const float& dt );
	void UpdateEditorCam( Camera& camera, const float& dt );
};
