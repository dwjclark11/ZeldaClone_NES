#include "Systems/GameSystems/CollisionSystem.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ProjectileComponent.h"
#include "Events/EventManager.h"
#include "Events/CollisionEvent.h"
#include "Game/Game.h"
#include "Game/Player.h"
#include "Utilities/Utility.h"

/*
* RayCheck and SweptAABB were inspired by Javidx9 
* https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_Rectangles.cpp
* I have adjusted the code to work for me; however, it is highly inspired by the above project. The collision algorithm
* that I was using previously was not working correclty. This works perfect for my use. 
* 
* This is the license that pertains to the RayCheck and SweptAABB functions
* 
* /*
	One-Size-Fits-All Rectangle Vs Rectangle Collisions
	"Stupid scanners... making me miss at archery..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2020 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Relevant Video: https://www.youtube.com/watch?v=8JJ-4JgR7Dg

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Community Blog: https://community.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018, 2019, 2020

	Modified By: 
	~~~~~~
	Dusitn Clark, 2024
*/
 
bool CollisionSystem::CheckAABB( const CollisionRect& rect1, const CollisionRect& rect2 )
{
	return rect1.position.x < rect2.position.x + rect2.width && rect1.position.x + rect1.width > rect2.position.x &&
		   rect1.position.y < rect2.position.y + rect2.height && rect1.position.y + rect1.height > rect2.position.y;
}

bool CollisionSystem::RayCheck( const glm::vec2& rayOrigin, const glm::vec2& rayDir, const CollisionRect& rect,
								ContactInfo& contactInfo )
{
	auto t_near = ( rect.position - rayOrigin ) / rayDir;
	auto t_far = ( rect.position + glm::vec2{ rect.width, rect.height } - rayOrigin ) / rayDir;

	// Check to see if t_far/t_near are real numbers
	if ( std::isnan( t_far.y ) || std::isnan( t_far.x ) )
		return false;
	if ( std::isnan( t_near.y ) || std::isnan( t_near.x ) )
		return false;

	// Check to see if near is greater than far, swap if that is the case
	if ( t_near.x > t_far.x )
		std::swap( t_near.x, t_far.x );

	if ( t_near.y > t_far.y )
		std::swap( t_near.y, t_far.y );

	if ( t_near.x > t_far.y || t_near.y > t_far.x )
		return false;

	// Get the max and min
	auto t_hit_near = std::max( t_near.x, t_near.y );
	auto t_hit_far = std::min( t_far.x, t_far.y );

	// Reject any position that is behind the ray
	if ( t_hit_far < 0.f )
		return false;

	// Calculate the collision point
	auto contactPoint = rayOrigin + ( t_hit_near * rayDir );

	// Construct the Contact Normals
	if ( t_near.x > t_near.y )
	{
		if ( rayDir.x < 0 )
		{
			contactInfo.contactNormal = glm::vec2{ 1.f, 0.f };
		}
		else
		{
			contactInfo.contactNormal = glm::vec2{ -1.f, 0.f };
		}
	}
	else if ( t_near.x < t_near.y )
	{
		if ( rayDir.y < 0 )
		{
			contactInfo.contactNormal = glm::vec2{ 0.f, 1.f };
		}
		else
		{
			contactInfo.contactNormal = glm::vec2{ 0.f, -1.f };
		}
	}

	contactInfo.contactPoint = contactPoint;
	contactInfo.contactTime = t_hit_near;

	return true;
}

bool CollisionSystem::SweptAABB( const CollisionRect& checkedRect, const CollisionRect& targetRect,
								 ContactInfo& contactInfo, const float dt )
{
	if ( checkedRect.velocity.x == 0.f && checkedRect.velocity.y == 0.f )
	{
		return false;
	}

	glm::vec2 halfInRect{ glm::vec2{ checkedRect.width * 0.5f, checkedRect.height * 0.5f } };

	CollisionRect expandedTarget{ .position = targetRect.position - halfInRect,
								  .velocity = targetRect.velocity,
								  .width = targetRect.width + checkedRect.width,
								  .height = targetRect.height + checkedRect.height };

	if ( RayCheck( checkedRect.position + halfInRect,
				   ( checkedRect.velocity - expandedTarget.velocity ) * dt,
				   expandedTarget,
				   contactInfo ) )
	{
		return contactInfo.contactTime >= 0.f && contactInfo.contactTime <= 1.f;
	}

	return false;
}

CollisionSystem::CollisionSystem( EventManager& eventManager )
	: m_Game( Game::Instance() )
	, m_EventManager( eventManager )
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<BoxColliderComponent>();
}

void CollisionSystem::Update( const float dt )
{
	auto entities = GetSystemEntities();
	const auto& playerPos = m_Game.GetPlayer()->GetPlayerPos();

	for ( auto& a : entities )
	{
		auto& aTransform = a.GetComponent<TransformComponent>();
		const int checkAX = aTransform.position.x / PANEL_WIDTH;
		const int checkAY = aTransform.position.y / PANEL_HEIGHT;

		// Check to see if the bCollider is in the same panel as the player, if not skip the check
		if ( checkAX != playerPos.x || checkAY != playerPos.y )
			continue;

		bool a_has_bCollider = a.HasComponent<ColliderComponent>();
		bool a_has_trigger = a.HasComponent<TriggerBoxComponent>();
		bool a_has_rigid = a.HasComponent<RigidBodyComponent>();
		bool a_is_projectile = a.HasComponent<ProjectileComponent>();

		for ( auto& b : entities )
		{
			// Bypass if we are testing the same entity
			if ( a == b )
				continue;

			auto& bTransform = b.GetComponent<TransformComponent>();
			const int checkBX = bTransform.position.x / PANEL_WIDTH;
			const int checkBY = bTransform.position.y / PANEL_HEIGHT;

			// Check to see if the bCollider is in the same panel as the player, if not skip the check
			if ( checkBX != playerPos.x || checkBY != playerPos.y )
				continue;

			bool b_has_bCollider{ b.HasComponent<ColliderComponent>() };
			bool b_has_trigger{ b.HasComponent<TriggerBoxComponent>() };
			bool b_has_rigid{ b.HasComponent<RigidBodyComponent>() };
			bool b_is_projectile = b.HasComponent<ProjectileComponent>();

			// If they are both bColliders, skip check
			if ( a_has_bCollider && b_has_bCollider )
				continue;

			if ( a_has_bCollider && b_has_trigger )
			{
				// Check to see if the trigger is a bCollider
				if ( !b.GetComponent<TriggerBoxComponent>().bCollider )
					continue;
			}

			if ( a_has_trigger && b_has_bCollider )
			{
				// Check to see if the trigger is a bCollider
				if ( !a.GetComponent<TriggerBoxComponent>().bCollider )
					continue;
			}

			const auto& aCollider = a.GetComponent<BoxColliderComponent>();
			const auto& bCollider = b.GetComponent<BoxColliderComponent>();

			CollisionRect rectA{ .position = aTransform.position + aCollider.offset,
								 .width = aCollider.width * aTransform.scale.x,
								 .height = aCollider.height * aTransform.scale.y };

			CollisionRect rectB{ .position = bTransform.position + bCollider.offset,
								 .width = bCollider.width * bTransform.scale.x,
								 .height = bCollider.height * bTransform.scale.y };

			ContactInfo contactInfo{};

			if ( a_has_rigid && !a_is_projectile && b_has_bCollider )
			{
				auto& aRigid = a.GetComponent<RigidBodyComponent>();
				rectA.velocity = aRigid.velocity;

				if ( SweptAABB( rectA, rectB, contactInfo, dt ) )
				{
					glm::vec2 multiplier = contactInfo.contactNormal *
										   glm::vec2{ std::abs( aRigid.velocity.x ), std::abs( aRigid.velocity.y ) } *
										   ( 1 - contactInfo.contactTime );

					if ( b_has_trigger )
					{
						if ( b.GetComponent<TriggerBoxComponent>().bCollider )
							aRigid.velocity += multiplier;

						m_EventManager.EmitEvent<CollisionEvent>( CollisionEvent{ .a = a, .b = b } );
					}
					else
					{
						aRigid.velocity += multiplier;
					}
				}
			}
			else if ( a_has_bCollider && !b_is_projectile && b_has_rigid )
			{
				auto& bRigid = b.GetComponent<RigidBodyComponent>();
				rectB.velocity = bRigid.velocity;

				if ( SweptAABB( rectB, rectA, contactInfo, dt ) )
				{
					glm::vec2 multiplier = contactInfo.contactNormal *
										   glm::vec2{ std::abs( bRigid.velocity.x ), std::abs( bRigid.velocity.y ) } *
										   ( 1 - contactInfo.contactTime );

					if ( a_has_trigger )
					{
						if ( a.GetComponent<TriggerBoxComponent>().bCollider )
							bRigid.velocity += multiplier;

						m_EventManager.EmitEvent<CollisionEvent>( CollisionEvent{ .a = a, .b = b } );
					}
					else
					{
						bRigid.velocity += multiplier;
					}
				}
			}
			else if ( CheckAABB( rectA, rectB ) )
			{
				m_EventManager.EmitEvent<CollisionEvent>( CollisionEvent{ .a = a, .b = b } );
			}
			else
			{
				aTransform.bCollision = false;
				bTransform.bCollision = false;
			}
		}
	}
}
