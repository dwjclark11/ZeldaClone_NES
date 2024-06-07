#pragma once
#include "ECS/ECS.h"

class RenderGameOverSystem : public System
{
  public:
	RenderGameOverSystem();

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager );
	void OnExit();
};
