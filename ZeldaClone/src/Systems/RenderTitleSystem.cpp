#include "RenderTitleSystem.h"

int RenderTitleSystem::titleTimer = 0;

void RenderTitleSystem::OnExit()
{
	for (auto entity : GetSystemEntities())
	{
		entity.Kill();
	}


	titleTimer = 0;
	offset = false;
}
