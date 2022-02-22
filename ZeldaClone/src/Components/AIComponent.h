#pragma once
#include "../StateMachines/NewStateMachine.h"
#include "../Utilities/Timer.h"
#include <glm/glm.hpp>
#include <memory>

class AIComponent
{
public: 
	
	enum class EnemyType
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
	
	enum class EnemyBossType
	{
		NOT_A_BOSS = 0,
		AQUAMENTUS,
		DODONGO,
		MANHANDLA,
		GLEEOK,
		DIGDOGGER,
		GOHMA,
		GANON
	};

private:
	StateMachine* esm;
	//std::unique_ptr<StateMachine> esm;
	glm::vec2 enemyPos;
	EnemyType enemyType;
	EnemyBossType bossType;

	bool created;
	bool stunned;
	bool boss;
	
public:
	// Should I make these private?
	Timer aiTimer;
	Timer deathTimer;
	Timer stunTimer;
	Timer leeverTimer;
	
	
	AIComponent(glm::vec2 enemyPos = glm::vec2(0, 0), EnemyType enemyType = EnemyType::OCTOROK, EnemyBossType = EnemyBossType::NOT_A_BOSS, bool boss = false);
	inline void GarbageCollect() { delete esm; esm = nullptr; }
	
	inline StateMachine& GetEnemyStateMachine() { return *esm; }
	
	inline glm::vec2& GetEnemyPos() { return enemyPos; }
	inline void SetStunned(bool stun) { stunned = stun; }
	inline const bool GetStunned() { return stunned; }
	inline const EnemyType GetEnemyType() const { return enemyType; }
	inline void SetEnemyType(EnemyType type) { enemyType = type; }
	inline const EnemyBossType GetBossType() const { return bossType; }
	inline const bool IsABoss() const { return boss; }
	inline void SetCreated(bool create) { created = create; }
	inline const bool IsCreated() const { return created; }
	const bool StateMachineCreated();
	void CreateStateMachine();
	
};