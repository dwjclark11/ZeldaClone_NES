#pragma once
#include <SDL.h>

class CameraComponent
{
public:
	SDL_Rect camera;

	CameraComponent(SDL_Rect camera = { 0,0, 1024, 960 })
	{
		this->camera = camera;
	}
};