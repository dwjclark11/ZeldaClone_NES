#pragma once
#include "ECS/ECS.h"

struct SDL_Renderer;
struct SDL_Rect;
class AssetManager;

class RenderTileSystem : public System
{
  private:
	class Game& m_Game;

  public:
	RenderTileSystem();
	~RenderTileSystem() = default;

	void Update();
	void OnExit();
};