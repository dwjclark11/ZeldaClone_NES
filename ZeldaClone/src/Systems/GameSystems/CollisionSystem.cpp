#include "CollisionSystem.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/SoundComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/RupeeTypeComponent.h"
#include "../../Components/ColliderComponent.h"
#include "../../Components/PlayerComponent.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Game/Game.h"
#include "TriggerSystem.h"

CollisionSystem::CollisionSystem()
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<BoxColliderComponent>();
}

void CollisionSystem::Update(std::unique_ptr<EventManager>& eventManager)
{
	auto entities = GetSystemEntities();
	// Loop all the entities that the system is interested in
	for (auto i = entities.begin(); i != entities.end(); i++)
	{
		Entity a = *i;
		auto& aTransform = a.GetComponent<TransformComponent>();
		auto& aCollider = a.GetComponent<BoxColliderComponent>();

		// Loop all entities that still need to be checked 
		for (auto j = i; j != entities.end(); j++)
		{
			Entity b = *j;

			// Bypass if we are testing the same entity
			if (a == b)
				continue;
			// bypass if both are colliders
			if (a.HasComponent<ColliderComponent>() && b.HasComponent<ColliderComponent>())
				continue;
			if (a.HasComponent<ColliderComponent>() && b.HasComponent<TriggerBoxComponent>())
				continue;
			if (a.HasComponent<TriggerBoxComponent>() && b.HasComponent<ColliderComponent>())
				continue;

			auto& bTransform = b.GetComponent<TransformComponent>();
			auto& bCollider = b.GetComponent<BoxColliderComponent>();

			// Perform the AABB collision check between entities a and b
			bool collisionHappened = CheckAABBCollision(
				aTransform.position.x + aCollider.offset.x,
				aTransform.position.y + aCollider.offset.y,
				aCollider.width * aTransform.scale.x,
				aCollider.height * aTransform.scale.y,
				bTransform.position.x + bCollider.offset.x,
				bTransform.position.y + bCollider.offset.y,
				bCollider.width * bTransform.scale.x,
				bCollider.height * bTransform.scale.y
			);

			if (collisionHappened)
			{
				eventManager->EmitEvent<CollisionEvent>(a, b);
			}
			else
			{
				aTransform.collision = false;
				bTransform.collision = false;
			}
		}
	}
}

bool CollisionSystem::CheckAABBCollision(double aX, double aY, double aWidth, double aHeight,
	double bX, double bY, double bWidth, double bHeight)
{
	return (
		aX < bX + bWidth &&
		aX + aWidth > bX &&
		aY < bY + bHeight &&
		aY + aHeight > bY
		);
}
