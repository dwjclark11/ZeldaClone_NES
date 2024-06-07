#pragma once
#include "ECS/ECS.h"

class Game;
class AISystem : public System
{
  private:
	Game& m_Game;

  public:
	AISystem();
	~AISystem() = default;

	void Update();
};