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

#include <future>
#include <thread>
#include <exception>

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
		Entity& a = *i;

		auto& aTransform = a.GetComponent<TransformComponent>();
		const int checkAX = aTransform.position.x / 1024;
		const int checkAY = aTransform.position.y / 672;

		const auto& playerPos = Game::Instance().GetPlayerPos();

		// Check to see if the collider is in the same panel as the player, if not skip the check
		if (checkAX != playerPos.x || checkAY != playerPos.y)
			continue;

		// Loop all entities that still need to be checked 
		for (auto j = i; j != entities.end(); j++)
		{
			Entity& b = *j;
			// Bypass if we are testing the same entity
			if (a == b)
				continue;

			auto& bTransform = b.GetComponent<TransformComponent>();
			const int checkBX = bTransform.position.x / 1024;
			const int checkBY = bTransform.position.y / 672;

			// Check to see if the collider is in the same panel as the player, if not skip the check
			if (checkBX != playerPos.x || checkBY != playerPos.y)
				continue;

			auto bCol = std::async([&b]() { return b.HasComponent<ColliderComponent>(); });
			auto aCol = a.HasComponent<ColliderComponent>();

			// bypass if both are colliders
			if (bCol.get() && aCol)
				continue;


			/*if (a.HasComponent<ColliderComponent>() && b.HasComponent<ColliderComponent>())
				continue;*/


			auto bTrig = std::async([&b]() { return b.HasComponent<TriggerBoxComponent>(); });
			if (bTrig.get() && aCol)
			{
				// Check to see if the trigger is a collider
				if (!b.GetComponent<TriggerBoxComponent>().collider)
					continue;
			}

			//if (a.HasComponent<ColliderComponent>() && b.HasComponent<TriggerBoxComponent>())
			//{
			//	// Check to see if the trigger is a collider
			//	if (!b.GetComponent<TriggerBoxComponent>().collider)
			//		continue;
			//}


			auto bCol2 = std::async([&b]() { return b.HasComponent<ColliderComponent>(); });
			auto aTrig = a.HasComponent<TriggerBoxComponent>();

			if (bCol2.get() && aTrig)
			{
				// Check to see if the trigger is a collider
				if (!a.GetComponent<TriggerBoxComponent>().collider)
					continue;
			}

			//if (a.HasComponent<TriggerBoxComponent>() && b.HasComponent<ColliderComponent>())
			//{
			//	// Check to see if the trigger is a collider
			//	if (!a.GetComponent<TriggerBoxComponent>().collider)
			//		continue;
			//}

			const auto& aCollider = a.GetComponent<BoxColliderComponent>();
			const auto& bCollider = b.GetComponent<BoxColliderComponent>();

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
