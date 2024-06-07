#pragma once
#include "ECS/ECS.h"

class RenderHUDSystem : public System
{
  private:
	class Game& m_Game;
	class GameData& m_GameData;
	bool m_bPause, m_bGamePlay;

  public:
	RenderHUDSystem();
	~RenderHUDSystem() = default;

	void Update( struct SDL_Renderer* renderer, std::unique_ptr<class AssetManager>& assetManager );
	void OnExit();
};
