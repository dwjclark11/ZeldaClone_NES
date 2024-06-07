#pragma once
#include "ECS/ECS.h"

class RenderCollisionSystem : public System
{
  public:
	RenderCollisionSystem();
	~RenderCollisionSystem() = default;

	void Update( struct SDL_Renderer* renderer, struct SDL_Rect& camera );
	void OnExit();
};