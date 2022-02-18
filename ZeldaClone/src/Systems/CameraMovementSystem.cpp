#include "CameraMovementSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Systems/GameSystems/KeyboardControlSystem.h"
#include <chrono>
#include <thread>

#include <SDL.h>
#include "../Game/Game.h"


CameraMovementSystem::CameraMovementSystem()
	: game(Game::Instance())
{
	RequiredComponent<CameraFollowComponent>();
	RequiredComponent<TransformComponent>();

	// Initialize all the member variables
	first = false;
	west = false;
	east = false;
	north = false;
	south = false;
	titleScreenScroll_1 = false;
	titleScreenScroll_2 = false;
	titleScreenScroll_3 = false;
	timerStart = false;
	titleScreenScroll_Finished = false;
	scrollTimer = 0;

}

void CameraMovementSystem::Update(SDL_Rect& camera)
{
	// Common Variables
	const auto& currentState = game.GetStateMachine()->GetCurrentState();
	auto& cameraMoving = game.GetCameraMoving();
	
	for (const auto& entity : GetSystemEntities())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& sprite = entity.GetComponent<SpriteComponent>();
		auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
		const auto& boxCollider = entity.GetComponent<BoxColliderComponent>();

		// Check to see what the current state is and move the camera according to the required state
		if (currentState == "EDITOR")
		{
			if (!first)
			{
				camera.x = 0;
				camera.y = 0;
				first = true;
			}

			// Keep the camera rectange view inside the screen limits
			camera.x = camera.x < -100 ? 100 : camera.x; // If the camera.x is < 0 make it 0
			camera.y = camera.y < -100 ? 100 : camera.y;
			camera.x = (camera.x + camera.w > 100000) ? 100000 - camera.w : camera.x; // if Camera.x is > than the width of the screen
			camera.y = (camera.y + camera.h > 100000) ? 100000 - camera.h : camera.y;
		}
		else if (currentState == "GAMESTATE")
		{
			/*
				Inside GameState the camera has a scrolling effect. Once the player is beyond the camera bounds,
				the camera will scroll to the next panel at a speed of 24px per interval; The Game::Instance->GetCameraMoving() method
				prevents the player from overstepping the camera.
			*/
			// Update the player position for the location map in the HUD!
			if (entity.HasTag("player"))
			{
				for (int i = 0; i < game.GetLevelWidth(); i++)
				{
					for (int j = 0; j < game.GetLevelHeight(); j++)
					{
						if (transform.position.x + boxCollider.width * transform.scale.x + boxCollider.offset.x >= (1024 * i) 
							&& transform.position.x + boxCollider.width * transform.scale.x + boxCollider.offset.x <= 1024 + (1024 * i) && east)
						{
							if (east)
							{
								rigidBody.velocity.x = 0;
								camera.x += 400 * game.GetDeltaTime();
							}

							if (camera.x >= i * 1024 && east)
							{
								camera.x = i * 1024;
								east = false;
								cameraMoving = false;
							}
						}
						else if (transform.position.x >= (1024 * i) && transform.position.x <= 1024 + (1024 * i) && west)
						{
							if (west)
							{
								rigidBody.velocity.x = 0;
								camera.x -= 400 * game.GetDeltaTime();
							}

							if (camera.x <= i * 1024 && west)
							{
								camera.x = i * 1024;
								west = false;
								cameraMoving = false;
							}
						}

						if (transform.position.y >= (672 * j) && transform.position.y <= 672 + (672 * j) && north)
						{
							if (north)
							{
								rigidBody.velocity.y = 0;
								camera.y -= 200 * game.GetDeltaTime();
							}

							if (camera.y <= (j * 672) - 288 && north)
							{
								camera.y = (j * 672) - 288;
								north = false;
								cameraMoving = false;
							}
						}
						else if (transform.position.y + boxCollider.height * transform.scale.y + boxCollider.offset.y >= (672 * j) 
							&& transform.position.y + boxCollider.height * transform.scale.y + boxCollider.offset.y <= 672 + (672 * j) && south)
						{
							if (south)
							{
								rigidBody.velocity.y = 0;
								camera.y += 200 * game.GetDeltaTime();
							}
							if (camera.y >= (j * 672) - 288 && south)
							{
								camera.y = (j * 672) - 288;
								south = false;
								cameraMoving = false;
							}
						}
					}
				}
			}

			if (entity.HasComponent<PlayerComponent>())
			{
				if (camera.x > (transform.position.x + boxCollider.offset.x) 
					&& !west && !cameraMoving && rigidBody.velocity.x < 0)
				{
					west = true;
					cameraMoving = true;
					rigidBody.velocity.x = 0;
				}

				if ((camera.x + camera.w) < transform.position.x + sprite.width / 4 && !east && !cameraMoving
					&& rigidBody.velocity.x > 0)
				{
					east = true;
					cameraMoving = true;
					rigidBody.velocity.x = 0;
				}

				if (camera.y > (transform.position.y + boxCollider.offset.y) - (game.windowHeight / 6 + (game.tilePixels * game.gameScale) + 64) 
					&& !north && !cameraMoving && (rigidBody.velocity.y < 0 || (game.GetRaft() && KeyboardControlSystem::dir == UP)))
				{
					north = true;
					cameraMoving = true;
					rigidBody.velocity.y = 0;
				}

				if ((camera.y + camera.h) < transform.position.y + boxCollider.height * transform.scale.y + boxCollider.offset.y 
					&& !south && !cameraMoving && (rigidBody.velocity.y > 0 || (game.GetRaft() && KeyboardControlSystem::dir == DOWN)))
				{
					south = true;
					cameraMoving = true;
					rigidBody.velocity.y = 0;
				}
			}

			// Keep the camera rectange view inside the screen limits
			camera.x = camera.x < 0 ? 0 : camera.x; // If the camera.x is < 0 make it 0
			camera.y = camera.y < -288 ? -288 : camera.y;
			camera.x = (camera.x + camera.w > 16384) ? 16384 - camera.w : camera.x; // if Camera.x is > than the width of the screen
			camera.y = (camera.y + camera.h > 5376) ? 5376 - camera.h : camera.y;

			if (camera.x == 0)
			{
				west = false;
			}

			if (camera.y == -288)
				north = false;

		}
	}
	// If the current State is the Title State, Run this Camera 
	if (currentState == "TITLESTATE")
	{
		if (!timer.isStarted())
			timer.Start();

		//if (timer.isStarted()) Logger::Log("ticks: " + std::to_string(timer.GetTicks()));
		/*
			This part of the system controls the scroll of the camera during the title screen animations
			The camera will wait for a certain amount of time and then the screen should appear to scroll
			in a upwards motion, pausing for 3 seconds at 960
		*/

		// Where to place the Origin of the camera during the first entrance of the state
		if (!first)
		{
			
			camera.x = 0;
			camera.y = 0;
			first = true;
		}

		if (!titleScreenScroll_1 && !titleScreenScroll_2 && !game.GetCameraMoving())
		{
			titleScreenScroll_1 = true;
			cameraMoving = true;
		}

		if (titleScreenScroll_1)
		{
			camera.y += 100 * game.GetDeltaTime();
			std::this_thread::sleep_for(std::chrono::microseconds(5));

			if (camera.y >= 960)
			{
				camera.y = 960;
				titleScreenScroll_1 = false;
				cameraMoving = false;
				
			}
		}

		if (!titleScreenScroll_1 && !titleScreenScroll_2 && !titleScreenScroll_3)
		{	
			if (timer.GetTicks() > 18000)
				titleScreenScroll_2 = true;
		}

		if (titleScreenScroll_2)
		{
			camera.y += 100 * game.GetDeltaTime();

			if (camera.y >= 6720)
			{
				camera.y = 6720;
				titleScreenScroll_3 = true;
			}
		}
		if (titleScreenScroll_3)
		{
			//if (timer.isStarted()) Logger::Log("ticks: " + std::to_string(timer.GetTicks()));
			if (timer.GetTicks() > 120000)
			{
				titleScreenScroll_Finished = true;
				titleScreenScroll_3 = false;
				titleScreenScroll_2 = false;
				titleScreenScroll_1 = false;
				timer.Stop();
				camera.y = 0;
			}
		}
		// Keep the camera rectange view inside the screen limits
		camera.x = camera.x < 0 ? 0 : camera.x; // If the camera.x is < 0 make it 0
		camera.y = camera.y < 0 ? 0 : camera.y;
	}
}

void CameraMovementSystem::OnExit()
{
	first = false;
	game.GetCameraMoving() = false;
	scrollTimer = 0;
}
