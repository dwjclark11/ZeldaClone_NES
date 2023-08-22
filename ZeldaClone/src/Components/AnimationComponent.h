#pragma once
#include <SDL.h>

struct AnimationComponent
{

	int numFrames, currentFrame, frameSpeedRate;
	bool vertical, isLooped;
	int startTime, frameOffset, lastFrame;

	AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool vertical = true, bool isLooped = true, int frameOffset = 0)
		: numFrames{numFrames}, currentFrame{1}, frameSpeedRate{frameSpeedRate}
		, vertical{vertical}, isLooped{isLooped}, startTime{static_cast<int>(SDL_GetTicks())}
		, frameOffset{frameOffset}, lastFrame{0}

	{

	}
};