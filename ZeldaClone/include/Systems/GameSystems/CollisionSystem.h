#pragma once
#include "ECS/ECS.h"
#include <glm/glm.hpp>

class EventManager;

class CollisionSystem : public System
{
  private:
	class Game& m_Game;
	EventManager& m_EventManager;

	struct CollisionRect
	{
		glm::vec2 position{ 0.f };
		glm::vec2 velocity{ 0.f };
		float width{ 0.f }, height{ 0.f };
	};

	struct ContactInfo
	{
		glm::vec2 contactPoint{ 0.f };
		glm::vec2 contactNormal{ 0.f };
		float contactTime{ 0.f };
	};

  private:
	bool CheckAABB( const CollisionRect& rect1, const CollisionRect& rect2 );
	bool RayCheck( const glm::vec2& rayOrigin, const glm::vec2& rayDir, const CollisionRect& rect2,
				   ContactInfo& contactInfo );
	bool SweptAABB( const CollisionRect& checkedRect, const CollisionRect& targetRect, ContactInfo& contactInfo,
					const float dt );

  public:
	CollisionSystem( EventManager& eventManager );
	~CollisionSystem() = default;

	void Update( const float dt );
};
