#pragma once
#include "../StateMachines/EnemyStateMachine.h"
#include "../StateMachines/PlayerStateMachine.h"
#include "../Utilities/Timer.h"
#include <glm/glm.hpp>
#include <memory>

class AIComponent
{
	

public: 
	
	enum EnemyType
	{
		OCTOROK = 0,
		MOBLIN,
		DARKNUT,
		LEEVER,
		TEKTITE,
		PEAHAT,
		ARMOS,
		GHINI,
		LYNEL,
		ZORA,

	};

private:
	EnemyStateMachine* esm;
	glm::vec2 enemyPos;
	EnemyType enemyType;

	bool created;
	bool stunned;

public:
	// Should I make these private?
	Timer aiTimer;
	Timer deathTimer;
	Timer stunTimer;
	Timer leeverTimer;
	
	
	AIComponent(glm::vec2 enemyPos = glm::vec2(0, 0), EnemyType enemyType = OCTOROK);

	EnemyStateMachine& GetEnemyStateMachine() { return *esm; }
	void GarbageCollect() { delete esm; }
	
	glm::vec2& GetEnemyPos() { return enemyPos; }
	void SetStunned(bool stun) { stunned = stun; }
	
	const bool GetStunned() { return stunned; }
	const bool StateMachineCreated();
	
	void CreateStateMachine();
	const EnemyType GetEnemyType() const { return enemyType; }
};