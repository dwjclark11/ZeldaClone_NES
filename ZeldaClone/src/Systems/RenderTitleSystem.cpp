#include "RenderTitleSystem.h"

int RenderTitleSystem::titleTimer = 0;

void RenderTitleSystem::OnExit()
{
	/*for (auto entity : GetSystemEntities())
	{
		entity.Kill();
	}*/
	auto entities = GetSystemEntities();

	for (auto i = entities.begin(); i != entities.end() - 1; i++)
	{
		Entity& entity = *i;

		Registry::Instance().RemoveEntityFromSystems(*i);
	}

	titleTimer = 0;
	offset = false;
}
