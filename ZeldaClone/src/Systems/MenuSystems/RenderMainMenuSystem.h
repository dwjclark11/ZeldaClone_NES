#pragma once
#include "../../ECS/ECS.h"


class RenderMainMenuSystem : public System
{
public:

	RenderMainMenuSystem();

	void Update(struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager);
	void OnExit();
};
