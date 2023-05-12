#pragma once
#include <SDL.h>

class AnimationComponent
{
public:
	int numFrames;
	int currentFrame;
	int frameSpeedRate;
	bool vertical;
	bool isLooped;
	int startTime;
	int frameOffset;
	int lastFrame;

	AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool vertical = true, bool isLooped = true, int frameOffset = 0)
		: numFrames{numFrames}, currentFrame{1}, frameSpeedRate{frameSpeedRate}
		, vertical{vertical}, isLooped{isLooped}, startTime{static_cast<int>(SDL_GetTicks())}
		, frameOffset{frameOffset}, lastFrame{0}

	{

	}
};