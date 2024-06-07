#pragma once
#include "ECS/ECS.h"

class RenderHealthSystem : public System
{
  public:
	RenderHealthSystem();
	~RenderHealthSystem() = default;

	void Update( struct SDL_Renderer* renderer, const class Camera& camera );
	void OnExit();
};
