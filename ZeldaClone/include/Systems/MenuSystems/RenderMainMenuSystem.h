#pragma once
#include "ECS/ECS.h"

class RenderMainMenuSystem : public System
{
  private:
	class GameData& m_GameData;

  public:
	RenderMainMenuSystem();
	~RenderMainMenuSystem() = default;

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager );
	void OnExit();
};
