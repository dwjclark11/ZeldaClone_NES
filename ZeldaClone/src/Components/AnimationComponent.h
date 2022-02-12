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
	{
		this->numFrames = numFrames;
		this->currentFrame = 1;
		this->frameSpeedRate = frameSpeedRate;
		this->vertical = vertical;
		this->isLooped = isLooped;
		this->startTime = SDL_GetTicks();
		this-> frameOffset = frameOffset;
		this->lastFrame = 0;
	}
};