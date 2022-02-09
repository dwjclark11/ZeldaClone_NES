#pragma once
#include "../ECS/ECS.h"


// Fix for linux
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

class CameraMovementSystem : public System
{
private:
	class Game& game;
public:
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

	CameraMovementSystem();

	void Update(struct SDL_Rect& camera);
	inline bool& GetScrollFinished() { return titleScreenScroll_Finished; }
	void OnExit();

};
