#pragma once
#include <SDL.h>

struct AnimationComponent
{
	int numFrames{ 1 }, currentFrame{ 0 }, frameSpeedRate{ 1 };
	bool vertical{ true }, isLooped{ true }, bFinished{ false };
	int startTime{ static_cast<int>( SDL_GetTicks() ) }, frameOffset{ 0 }, lastFrame{ 0 };
};