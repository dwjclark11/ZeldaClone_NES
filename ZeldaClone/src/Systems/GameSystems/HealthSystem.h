#pragma once
#include "../../ECS/ECS.h"

class HealthSystem : public System
{
private:
	class Game& game;
	class Registry& reg;
	bool lowHealth = false;

public:
	static unsigned int numHearts;
	static int loadedHearts;
	
	HealthSystem();
	void Update();
	
};