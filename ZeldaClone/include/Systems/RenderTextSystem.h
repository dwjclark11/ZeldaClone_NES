#pragma once
#include "ECS/ECS.h"

class RenderTextSystem : public System
{
  private:
	class Game& m_Game;

  public:
	RenderTextSystem();
	~RenderTextSystem() = default;

	void Update();
	void OnExit();
	void OnExitSettings();
};