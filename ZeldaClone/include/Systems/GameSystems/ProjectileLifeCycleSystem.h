#pragma once
#include "ECS/ECS.h"

class ProjectileLifeCycleSystem : public System
{
  private:
	class Game& m_Game;

  private:
	void UpdateBombAnimation( Entity entity );
	void UpdateBeanAnimation( Entity entity );

  public:
	ProjectileLifeCycleSystem();
	void Update();
};