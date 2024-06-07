#pragma once
#include "ECS/ECS.h"

class MovementSystem : public System
{
  private:
	class Game& m_Game;

  private:
	void UpdateBoomerang( const double& deltaTime );
	void UpdateEnemies( const double& deltaTime );
	void UpdatePlayer( const double& deltaTime );
	void UpdateProjectiles( const double& deltaTime );
	void SetDirection( class RigidBodyComponent& rigidBody );

  public:
	MovementSystem();
	~MovementSystem() = default;

	void Update( const double& deltaTime );
};