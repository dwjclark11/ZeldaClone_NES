#pragma once
#include "../../ECS/ECS.h"

class ProjectileLifeCycleSystem : public System
{
private:
	class Game& game;
public:
	ProjectileLifeCycleSystem();
	void Update();

};