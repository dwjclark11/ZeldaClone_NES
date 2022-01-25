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
		NO_TYPE,
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

	inline EnemyStateMachine& GetEnemyStateMachine() { return *esm; }
	inline void GarbageCollect() { delete esm; esm = nullptr; }
	inline glm::vec2& GetEnemyPos() { return enemyPos; }
	inline void SetStunned(bool stun) { stunned = stun; }
	inline const bool GetStunned() { return stunned; }
	inline const EnemyType GetEnemyType() const { return enemyType; }

	const bool StateMachineCreated();
	void CreateStateMachine();
};