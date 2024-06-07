#pragma once
#include "ECS/ECS.h"

class RenderNameSystem : public System
{
  public:
	RenderNameSystem();

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager );
	void OnExit();
};
