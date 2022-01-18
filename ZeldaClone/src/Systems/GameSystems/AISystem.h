#pragma once
#include "../../ECS/ECS.h"

class Game;
class AISystem : public System
{
private:
	Game& game;
public:
	AISystem();
	
	void Update();
};