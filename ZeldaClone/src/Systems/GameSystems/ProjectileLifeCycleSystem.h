#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/ProjectileComponent.h"

class ProjectileLifeCycleSystem : public System
{
private:
	class Game& game;
public:
	ProjectileLifeCycleSystem();
	void Update();

};