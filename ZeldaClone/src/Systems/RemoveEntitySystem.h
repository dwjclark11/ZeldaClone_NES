#pragma once
#pragma once
#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

class RemoveEntitySystem : public System
{
public:
	RemoveEntitySystem()
	{
		RequiredComponent<SpriteComponent>();
		RequiredComponent<TransformComponent>();
	}

	void RemoveEntities()
	{
		auto entity = GetSystemEntities();

		if (GetSystemEntities().size() != 0)
		{
			for (int i = 0; i < entity.size(); i++)
			{
				if (entity[i].HasTag("player"))
				{
					continue;
				}
				else
					entity[i].Kill();
			}
		}
	}
	
};