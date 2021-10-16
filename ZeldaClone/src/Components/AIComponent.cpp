#include "AIComponent.h"

AIComponent::AIComponent(glm::vec2 enemyPos)
{
	esm = new EnemyStateMachine();
	this->enemyPos = enemyPos;
}
