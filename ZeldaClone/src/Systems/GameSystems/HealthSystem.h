#pragma once
#include "../../ECS/ECS.h"

class HealthSystem : public System
{
private:
	class Game& m_Game;
	class Registry& m_Registry;
	bool m_bLowHealth;

public:
	HealthSystem();
	~HealthSystem() = default;
	void Update();
	
	// TODO: Remove these statics
	static unsigned int numHearts;
	static int loadedHearts;
};