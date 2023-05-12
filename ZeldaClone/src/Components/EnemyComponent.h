#pragma once
#include <glm/glm.hpp>

class EnemyComponent
{
public:
//	enum class EnemyType
//	{
//		OCTOROK = 0, MOBLIN, DARKNUT,
//		LEEVER, TEKTITE, PEAHAT,
//		ARMOS, GHINI, LYNEL,
//		ZORA, KEESE, BLADE_TRAP,
//		GEL, STALFOS, GORIYA,
//		NO_TYPE,
//	};
//
//	enum class EnemyBossType
//	{
//		NOT_A_BOSS = 0, AQUAMENTUS, DODONGO,
//		MANHANDLA, GLEEOK, DIGDOGGER,
//		GOHMA, GANON
//	};
	enum class MoveDir { NO_MOVE = 0, UP, RIGHT, DOWN, LEFT };
	int maxMoveDistance;
	MoveDir moveDir;
	glm::vec2 minDistance, maxDistance, startPos;
	bool distanceCalculated, maxMoveReached;

	EnemyComponent(int maxMoveDistance = 0, MoveDir moveDir = MoveDir::NO_MOVE, glm::vec2 startPos = glm::vec2(0))
		: maxMoveDistance{ maxMoveDistance }, moveDir{moveDir}, minDistance{glm::vec2{0}}
		, maxDistance{ glm::vec2{0} }, startPos{startPos}, distanceCalculated {false}, maxMoveReached{false}
	{

	}
};