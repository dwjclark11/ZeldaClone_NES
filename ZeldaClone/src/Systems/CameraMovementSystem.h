#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"
#include <SDL.h>
#include "../Game/Game.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

class CameraMovementSystem : public System
{
public:
	bool first;
	bool west;
	bool east;
	bool north;
	bool south;
	bool titleScreenScroll_1;
	bool titleScreenScroll_2;
	bool titleScreenScroll_3;
	bool timerStart;
	bool titleScreenScroll_Finished;
	
	int scrollTimer;

	CameraMovementSystem()
	{
		
		// Declare the Components that are required in order to be updated by the 
		// Camera System
		RequiredComponent<CameraFollowComponent>();
		RequiredComponent<TransformComponent>();
		//RequiredComponent<SpriteComponent>();

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

	void Update(SDL_Rect& camera)
	{
		for (auto entity : GetSystemEntities())
		{
			
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
			const auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
			// Check to see what the current state is and move the camera according to the required state
			if (Game::Instance()->GetStateMachine()->GetCurrentState() == "EDITOR")
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
			else if (Game::Instance()->GetStateMachine()->GetCurrentState() == "GAMESTATE")
			{
				/*
					Inside GameState the camera has a scrolling effect. Once the player is beyond the camera bounds,
					the camera will scroll to the next panel at a speed of 24px per interval; The Game::Instance->GetCameraMoving() method
					prevents the player from overstepping the camera.
				*/
				if (entity.HasTag("player") || entity.HasTag("the_sword") || entity.HasTag("the_shield"))
				{
					if (!first)
					{
						camera.x = 7168;
						camera.y = 4704;
						first = true;
					}

					if (camera.x > (transform.position.x + boxCollider.offset.x) && !west && !Game::Instance()->GetCameraMoving() && rigidBody.velocity.x < 0)
					{
						west = true;
						Game::Instance()->GetCameraMoving() = true;
						rigidBody.velocity.x = 0;
					}

					if ((camera.x + camera.w) < (transform.position.x - boxCollider.offset.x) + (sprite.width * transform.scale.x) && !east && !Game::Instance()->GetCameraMoving() && rigidBody.velocity.x > 0)
					{
						east = true;
						Game::Instance()->GetCameraMoving() = true;
						rigidBody.velocity.x = 0;
					}

					if (camera.y > (transform.position.y + boxCollider.offset.y) - (Game::Instance()->windowHeight / 6 + (Game::Instance()->tilePixels * Game::Instance()->gameScale) + 64) && !north && !Game::Instance()->GetCameraMoving() && rigidBody.velocity.y < 0)
					{
						north = true;
						Game::Instance()->GetCameraMoving() = true;
						rigidBody.velocity.y = 0;
					}

					if ((camera.y + camera.h) < (transform.position.y) + ((sprite.height - boxCollider.height) * transform.scale.y) && !south && !Game::Instance()->GetCameraMoving() && rigidBody.velocity.y > 0)
					{
						south = true;
						Game::Instance()->GetCameraMoving() = true;
						rigidBody.velocity.y = 0;
					}

					// Moving West
					if (west)
					{
						rigidBody.velocity.x = 0;
						camera.x -= 24;
						Sleep(1);

						if (camera.x < (transform.position.x + boxCollider.offset.x) - Game::Instance()->windowWidth + (boxCollider.width * transform.scale.x))
						{
							camera.x = (transform.position.x + boxCollider.offset.x) - Game::Instance()->windowWidth + (boxCollider.width * transform.scale.x); // transform.position.x + sprite.width * transform.scale.x;
							west = false;
							Game::Instance()->GetCameraMoving() = false;
						}
					}

					// Moving East
					if (east)
					{
						rigidBody.velocity.x = 0;
						camera.x += 24;
						Sleep(1);

						if (camera.x + camera.w >= transform.position.x + Game::Instance()->windowWidth)
						{
							camera.x = (transform.position.x + boxCollider.offset.x) /*+ sprite.width * transform.scale.x*/;
							east = false;
							Game::Instance()->GetCameraMoving() = false;
						}
					}

					// Moving North
					if (north)
					{
						rigidBody.velocity.y = 0;
						camera.y -= 24;
						Sleep(1);

						if (camera.y < (transform.position.y + boxCollider.offset.y) - (Game::Instance()->windowHeight - (sprite.width * transform.scale.y)))
						{
							//transform.position.y -= 64;
							camera.y = (transform.position.y + boxCollider.offset.y) - Game::Instance()->windowHeight + (boxCollider.height * transform.scale.y);
							north = false;
							Game::Instance()->GetCameraMoving() = false;
						}
					}

					// Moving South
					if (south)
					{
						rigidBody.velocity.y = 0;
						camera.y += 24;
						Sleep(1);
						if (camera.y + camera.h > (transform.position.y - boxCollider.offset.y) + (Game::Instance()->windowHeight - (Game::Instance()->windowHeight / 4 + (Game::Instance()->tilePixels * Game::Instance()->gameScale))))
						{
							camera.y = transform.position.y + boxCollider.offset.y - (Game::Instance()->windowHeight / 6 + (Game::Instance()->tilePixels * Game::Instance()->gameScale) + 64);
							south = false;
							Game::Instance()->GetCameraMoving() = false;
						}
					}

					// Keep the camera rectange view inside the screen limits
					camera.x = camera.x < 0 ? 0 : camera.x; // If the camera.x is < 0 make it 0
					camera.y = camera.y < 0 ? 0 : camera.y;
					camera.x = (camera.x + camera.w > 16384) ? 16384 - camera.w : camera.x; // if Camera.x is > than the width of the screen
					camera.y = (camera.y + camera.h > 5376) ? 5376 - camera.h : camera.y;
				}
				else if (Game::Instance()->GetStateMachine()->GetCurrentState() == "DUNGEON")
				{
					if (camera.x > transform.position.x)
					{
						camera.x -= 1024;
					}
					if ((camera.x + camera.w) < transform.position.x + sprite.width)
					{
						camera.x += 1024;
					}
					if (camera.y > transform.position.y - 300)
					{
						camera.y -= 1620;
						transform.position.y -= 100;
					}
					if ((camera.y + camera.h) < transform.position.y + sprite.height)
					{
						camera.y += 960;
					}

					// Keep the camera rectange view inside the screen limits
					camera.x = camera.x < 0 ? 0 : camera.x; // If the camera.x is < 0 make it 0
					camera.y = camera.y < 0 ? 0 : camera.y;
					camera.x = (camera.x + camera.w > 16384) ? 16384 - camera.w : camera.x; // if Camera.x is > than the width of the screen
					camera.y = (camera.y + camera.h > 5376) ? 5376 - camera.h : camera.y;
				}
			}
		}

		// If the current State is the Title State, Run this Camera 
		if (Game::Instance()->GetStateMachine()->GetCurrentState() == "TITLESTATE")
		{
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

			if (!titleScreenScroll_1 && !titleScreenScroll_2 && !Game::Instance()->GetCameraMoving())
			{
				titleScreenScroll_1 = true;
				Game::Instance()->GetCameraMoving() = true;
			}

			if (titleScreenScroll_1)
			{
				camera.y += 1;
				Sleep(5);

				if (camera.y >= 960)
				{
					camera.y = 960;
					titleScreenScroll_1 = false;
					Game::Instance()->GetCameraMoving() = false;
				}
			}

			if (!titleScreenScroll_1 && !titleScreenScroll_2 && !titleScreenScroll_3)
			{
				Sleep(3000);
				titleScreenScroll_2 = true;
			}

			if (titleScreenScroll_2)
			{
				camera.y += 1;
				Sleep(5);

				if (camera.y >= 6720)
				{
					camera.y = 6720;
					titleScreenScroll_3 = true;
				}
			}
			if (titleScreenScroll_3)
			{
				if (scrollTimer < 50)
				{
					Sleep(100);
					scrollTimer++;
				}
				else
				{
					titleScreenScroll_Finished = true;
					titleScreenScroll_3 = false;
					titleScreenScroll_2 = false;
					titleScreenScroll_1 = false;
					camera.y = 0;
				}
			}
			// Keep the camera rectange view inside the screen limits
			camera.x = camera.x < 0 ? 0 : camera.x; // If the camera.x is < 0 make it 0
			camera.y = camera.y < 0 ? 0 : camera.y;
		}
		
	}

	bool& GetScrollFinished()
	{
		return titleScreenScroll_Finished;
	}

	void OnExit()
	{
		first = false;
		Game::Instance()->GetCameraMoving() = false;
		scrollTimer = 0;
	}
};
