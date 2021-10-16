#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/HealthComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/SoundComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/RupeeTypeComponent.h"
#include "../../Events/EventManager.h"
#include "../../Events/CollisionEvent.h"
#include "../../AssetManager/AssetManager.h"
#include "../SoundFXSystem.h"
#include "../../Game/Game.h"
#include "TriggerSystem.h"


class CollisionSystem : public System
{
public:

	CollisionSystem()
	{
		RequiredComponent<TransformComponent>();
		RequiredComponent<BoxColliderComponent>();
	}

	void Update(std::unique_ptr<EventManager>& eventManager, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
	{
		auto entities = GetSystemEntities();
		// Loop all the entities that the system is interested in
		for (auto i = entities.begin(); i != entities.end(); i++)
		{
			Entity a = *i;
			auto& aTransform = a.GetComponent<TransformComponent>();
			auto& aCollider = a.GetComponent<BoxColliderComponent>();
			auto& aRigidbody = a.GetComponent<RigidBodyComponent>();

			// Loop all entities that still need to be checked 
			for (auto j = i; j != entities.end(); j++)
			{
				Entity b = *j;

				// Bypass if we are testing the same entity
				if (a == b)
				{
					continue;
				}

				auto& bTransform = b.GetComponent<TransformComponent>();
				auto& bCollider = b.GetComponent<BoxColliderComponent>();
				auto& bRigidbody = b.GetComponent<RigidBodyComponent>();
				
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
					//Logger::Log("Entity " + std::to_string(a.GetID()) + " is Colliding with " + std::to_string(b.GetID()));
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

	bool CheckAABBCollision(double aX, double aY, double aWidth, double aHeight,
		double bX, double bY, double bWidth, double bHeight)
	{
		return (
			aX < bX + bWidth &&
			aX + aWidth > bX &&
			aY < bY + bHeight &&
			aY + aHeight > bY
			);
	}
	
	// Calculate the distance before the entities collide? 
	glm::vec2 CalculateAABBDistance(Entity a, Entity b)
	{
		auto& aPos = a.GetComponent<TransformComponent>();
		const auto& aBox = a.GetComponent<BoxColliderComponent>();
		auto& bPos = b.GetComponent<TransformComponent>();
		const auto& bBox = b.GetComponent<BoxColliderComponent>();
		
		glm::vec2 delta = glm::vec2(0, 0);
		
		if (aPos.position.x < bPos.position.x)
		{
			delta.x = bPos.position.x - (aPos.position.x + aBox.width);
		}
		else if (aPos.position.x > bPos.position.x)
		{
			delta.x = aPos.position.x - (bPos.position.x + bBox.width);
		}

		if (aPos.position.y < bPos.position.y)
		{
			delta.y = bPos.position.y - (aPos.position.y + aBox.height);
		}
		else if (aPos.position.y > bPos.position.y)
		{
			delta.y = aPos.position.y - (bPos.position.y + bBox.height);
			
		}
		return delta;
	}

	
};
