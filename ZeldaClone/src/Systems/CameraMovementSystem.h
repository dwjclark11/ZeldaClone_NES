#pragma once
#include "../ECS/ECS.h"
#include "../Utilities/Timer.h"

class Camera;

class CameraMovementSystem : public System
{
private:
	class Game& game;
	class GameData& gameData;
	bool first;
	bool titleScreenScroll_1;
	bool titleScreenScroll_2;
	bool titleScreenScroll_3;
	bool timerStart;
	bool titleScreenScroll_Finished;

	
	int m_CamSpeed, m_PrevCamX, m_PrevCamY;
	bool m_bStartEast, m_bStartWest, m_bStartNorth, m_bStartSouth;

	int scrollTimer;
	Timer timer;
public:

	CameraMovementSystem();

	void UpdateTitleCam(Camera& camera, const float& dt);
	void UpdatePlayerCam(Camera& camera, const float& dt);
	void UpdateEditorCam(Camera& camera, const float& dt);
	inline const bool GetScrollFinished() const { return titleScreenScroll_Finished; }
	inline void SetScrollFinished(bool finished) { titleScreenScroll_Finished = finished; }
	void OnExit();

};
