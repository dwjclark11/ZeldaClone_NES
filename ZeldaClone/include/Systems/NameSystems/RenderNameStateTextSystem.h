#pragma once
#include "ECS/ECS.h"

class RenderNameStateTextSystem : public System
{
  public:
	RenderNameStateTextSystem();

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager,
				 const struct SDL_Rect& camera );

	void OnExit();
};
