#pragma once
#include "../../ECS/ECS.h"
#include <sol/sol.hpp>
#include <tuple>

// First Declare some Native c++ functions that will bind with Lua Functions
// In Order to return multiple values to a lua/sol table, a Tuple must be used.
std::tuple<double, double> GetEntityPosition(Entity entity);


std::tuple<double, double> GetEntityVelocity(Entity entity);

std::tuple<double, double> GetPlayerPosition();

std::tuple<bool, bool> GetEntityCollision(Entity entity);

// Get Animation Component
//std::tuple <int, int, int, bool, bool, int, int, int> GetAnimationComponent(Entity entity)
//{
//	if (entity.HasComponent<AnimationComponent())
//	{
//		const auto& animation = entity.GetComponent<AnimationComponent>();
//		return std::make_tuple(animation.numFrames, animation.currentFrame, animation.frameSpeedRate, 
//		animation.vertical, animation.isLooped, animation.startTime, animation.frameOffset, animation.lastFrame);
//	}
//	else
//	{
//		Logger::Log("This entity Has No Animation");
//		return NULL;
//	}
//}

std::tuple <int, int> GetSpriteComponent(Entity entity);

void SetEntitySprite(Entity entity, int width, int height, int srcRectX, int srcRectY, int offsetX, int offsetY);

void SetEntityPosition(Entity entity, double x, double y);

void SetEntityVelocity(Entity entity, double x, double y);

void SetEntityRotation(Entity entity, double angle);

void SetEntityAnimationFrame(Entity entity, int frame);

class ScriptSystem : public System
{
public:
	ScriptSystem();
	
	void CreateLuaBindings(sol::state& lua);
	void Update(double deltaTime, int elapsedTime);
};