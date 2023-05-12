#pragma once
#include "../../ECS/ECS.h"

class RenderSystem : public System
{
private:
	class Game& game;

public:
	RenderSystem();

	void Update();
	void OnExit();
	
};