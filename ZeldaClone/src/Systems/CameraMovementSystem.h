#pragma once
#include "../ECS/ECS.h"
#include "../Utilities/Timer.h"

class CameraMovementSystem : public System
{
private:
	class Game& game;
	bool first;
	bool west;
	bool east;
	bool north;
	bool south;
	bool titleScreenScroll_1;
	bool titleScreenScroll_2;
	bool titleScreenScroll_3;
	bool timerStart;
	bool titleScreenScroll_Finished;

	int scrollTimer;
	Timer timer;
public:

	CameraMovementSystem();

	void Update(struct SDL_Rect& camera);
	inline const bool GetScrollFinished() const { return titleScreenScroll_Finished; }
	inline void SetScrollFinished(bool finished) { titleScreenScroll_Finished = finished; }
	void OnExit();

};
