#pragma once
#include "ECS/ECS.h"

class RenderPauseSystem : public System
{
  public:
	RenderPauseSystem();
	~RenderPauseSystem() = default;

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager );
	void OnExit();
};
