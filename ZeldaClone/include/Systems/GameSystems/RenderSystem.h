#pragma once
#include "ECS/ECS.h"

class RenderSystem : public System
{
  private:
	class Game& m_Game;

  public:
	RenderSystem();
	~RenderSystem() = default;

	void Update();
	void OnExit();
};