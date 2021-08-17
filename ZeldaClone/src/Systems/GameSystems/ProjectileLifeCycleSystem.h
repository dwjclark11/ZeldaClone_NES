#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/ProjectileComponent.h"

class ProjectileLifeCycleSystem : public System
{
public:
	ProjectileLifeCycleSystem()
	{
		RequiredComponent<ProjectileComponent>();
	}

	void Update();

};