#pragma once
#include "../../ECS/ECS.h"

class CaptionSystem : public System
{
private:
	
public:
	CaptionSystem();

	Entity& AddCaption(const Entity& entity, int& spaceX, int& spaceY);
	Entity& AddNumberCaption(const Entity& entity, int spaceX, int spaceY);

	void Update();

};