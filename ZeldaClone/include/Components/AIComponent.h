#pragma once
#include "StateMachines/NewStateMachine.h"
#include "Utilities/Timer.h"
#include <glm/glm.hpp>
#include <memory>

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
	KEESE,
	BLADE_TRAP,
	GEL,
	STALFOS,
	GORIYA,
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

struct AIComponent
{
	glm::vec2 enemyPos{ 0.f };
	EnemyType enemyType{ EnemyType::OCTOROK };
	EnemyBossType bossType{ EnemyBossType::NOT_A_BOSS };

	bool bIsBoss{ false }, bCreated{ false }, bStunned{ false };
	Timer aiTimer{}, deathTimer{}, stunTimer{}, leeverTimer{};
	std::shared_ptr<StateMachine> esm{ nullptr };
};