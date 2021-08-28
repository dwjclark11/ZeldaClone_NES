#pragma once
#include "../../ECS/ECS.h"
#include "../../Logger/Logger.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/ScriptComponent.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/ProjectileEmitterComponent.h"
#include <tuple>

// First Declare some Native c++ functions that will bind with Lua Functions
// In Order to return multiple values to a lua/sol table, a Tuple must be used.
std::tuple<double, double> GetEntityPosition(Entity entity)
{
	if (entity.HasComponent<TransformComponent>())
	{
		const auto transform = entity.GetComponent<TransformComponent>();
		return std::make_tuple(transform.position.x, transform.position.y);
	}
	else
	{
		Logger::Err("Trying to get the position of an entity that has no Transform Component");
		return std::make_tuple(0.0, 0.0);
	}
}

std::tuple<double, double> GetEntityVelocity(Entity entity)
{
	if (entity.HasComponent<RigidBodyComponent>())
	{
		const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
		return std::make_tuple(rigidBody.velocity.x, rigidBody.velocity.y);
	}
	else
	{
		Logger::Err("Trying to get the velocity of an entity that has no RigidBody Component");
		return std::make_tuple(0.0, 0.0);
	}
}

std::tuple<double, double> GetPlayerPosition()
{
	const auto& entity = Registry::Instance()->GetEntityByTag("player");
	const auto& transform = entity.GetComponent<TransformComponent>();
	return std::make_tuple(transform.position.x, transform.position.y);
}

void SetEntityPosition(Entity entity, double x, double y)
{
	if (entity.HasComponent<TransformComponent>())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		
		transform.position.x = x;
		transform.position.y = y;
	}
	else
	{
		Logger::Err("Trying to set the position of an entity that has no Transform Component");
	}
}

void SetEntityVelocity(Entity entity, double x, double y)
{
	if (entity.HasComponent<RigidBodyComponent>())
	{
		auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
		
		rigidBody.velocity.x = x;
		rigidBody.velocity.y = y;
	}
	else
	{
		Logger::Err("Trying to set the velocity of on entity that has no RigidBody Component");
	}
}

void SetEntityRotation(Entity entity, double angle)
{
	if (entity.HasComponent<TransformComponent>())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		
		transform.rotation = angle;
	}
	else
	{
		Logger::Err("Trying to change the angle/rotation of an entity that does not have a Transform Component");
	}
}

void SetEntityAnimationFrame(Entity entity, int frame)
{
	if (entity.HasComponent<AnimationComponent>())
	{
		auto& animation = entity.GetComponent<AnimationComponent>();
		
		animation.currentFrame = frame;
	}
	else
	{
		Logger::Err("trying to change the animation frame of an entity that does not have an Animation Component");
	}
}


class ScriptSystem : public System
{
public:
	ScriptSystem()
	{
		RequiredComponent<ScriptComponent>();
	}

	void CreateLuaBindings(sol::state& lua)
	{
		// Creat the entity usertype so Lua knows what an entity is:
		lua.new_usertype<Entity>
		(
			"entity",
			"get_id", &Entity::GetID, // Callback functions do not have ()
			"destroy", &Entity::Kill,
			"has_tag", &Entity::HasTag,
			"belongs_to_group", & Entity::BelongsToGroup
		);

		// Create all the bindings between c++ and Lua Functions
		lua.set_function("get_position", GetEntityPosition);
		lua.set_function("get_velocity", GetEntityVelocity);
		lua.set_function("set_position", SetEntityPosition);
		lua.set_function("set_velocity", SetEntityVelocity);
		lua.set_function("set_animation_frame", SetEntityAnimationFrame);
		lua.set_function("get_player_pos", GetPlayerPosition);
		// TODO:
		// Add all the other Lua Bindings and c++ functions that may be needed
	}

	void Update(double deltaTime, int elapsedTime)
	{
		// Loop all the entities that have a script component and invoke their lua functions
		for (auto entity : GetSystemEntities())
		{
			const auto script = entity.GetComponent<ScriptComponent>();
			script.func(entity, deltaTime, elapsedTime); // Here is where we invoke a sol::function
		}
	}
};