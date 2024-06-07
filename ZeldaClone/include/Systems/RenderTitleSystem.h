#pragma once
#include "ECS/ECS.h"

class RenderTitleSystem : public System
{
  public:
	RenderTitleSystem();
	~RenderTitleSystem() = default;

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager,
				 const class Camera& camera );
	void OnExit();
};
