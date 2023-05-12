#pragma once
#include "../ECS/ECS.h"

class RenderTextSystem : public System
{
private:
	class Game& game;
public: 
	RenderTextSystem();
	
	void Update();
	void OnExit();
	void OnExitSettings();
};