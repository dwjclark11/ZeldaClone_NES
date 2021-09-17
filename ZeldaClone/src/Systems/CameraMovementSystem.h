#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"
#include <SDL.h>
#include "../Game/Game.h"

// Fix for linux
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
				// Update the player position for the location map in the HUD!
				if (entity.HasTag("player"))
				{
					for (int i = 0; i < Game::Instance()->GetLevelWidth(); i++)
					{
						for (int j = 0; j < Game::Instance()->GetLevelHeight(); j++)
						{
							if (transform.position.x >= (1024 * i) && transform.position.x <= 1024 + (1024 * i))
							{
								if (transform.position.y >= (672 * j) && transform.position.y <= 672 + (672 * j))
								{
									if (west)
									{
										rigidBody.velocity.x = 0;
										camera.x -= 24;
										Sleep(1);
									}

									if (east)
									{
										rigidBody.velocity.x = 0;
										camera.x += 24;
										Sleep(1);
									}

									if (camera.x >= i * 1024 && east)
									{
										camera.x = i * 1024;
										east = false;
										Game::Instance()->GetCameraMoving() = false;
									}

									if (camera.x <= i * 1024 && west)
									{
										camera.x = i * 1024;
										west = false;
										Game::Instance()->GetCameraMoving() = false;
									}

									if (south)
									{
										rigidBody.velocity.y = 0;
										camera.y += 24;
										Sleep(1);
									}

									if (north)
									{
										rigidBody.velocity.y = 0;
										camera.y -= 24;
										Sleep(1);
									}

									if (camera.y >= (j * 672) - 288 && south)
									{
										camera.y = (j * 672) - 288;
										south = false;
										Game::Instance()->GetCameraMoving() = false;
									}

									if (camera.y <= (j * 672) - 288 && north)
									{
										camera.y = (j * 672) - 288;
										north = false;
										Game::Instance()->GetCameraMoving() = false;
									}
								}
							}
						}
					}
				}

				if (entity.HasTag("player") || entity.HasTag("the_sword") || entity.HasTag("the_shield"))
				{

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
				}
				
				// Keep the camera rectange view inside the screen limits
				camera.x = camera.x < -288 ? -288 : camera.x; // If the camera.x is < 0 make it 0
				camera.y = camera.y < -288 ? -288 : camera.y;
				camera.x = (camera.x + camera.w > 16384) ? 16384 - camera.w : camera.x; // if Camera.x is > than the width of the screen
				camera.y = (camera.y + camera.h > 5376) ? 5376 - camera.h : camera.y;
			}
			else if (Game::Instance()->GetStateMachine()->GetCurrentState() == "DUNGEON")
			{
			/*
				Inside GameState the camera has a scrolling effect. Once the player is beyond the camera bounds,
				the camera will scroll to the next panel at a speed of 24px per interval; The Game::Instance->GetCameraMoving() method
				prevents the player from overstepping the camera.
			*/
			// Update the player position for the location map in the HUD!
			if (entity.HasTag("player"))
			{
				for (int i = 0; i < Game::Instance()->GetLevelWidth(); i++)
				{
					for (int j = 0; j < Game::Instance()->GetLevelHeight(); j++)
					{
						if (transform.position.x >= (1024 * i) && transform.position.x <= 1024 + (1024 * i))
						{
							if (transform.position.y >= (704 * j) && transform.position.y <= 704 + (704 * j))
							{
								if (west)
								{
									rigidBody.velocity.x = 0;
									camera.x -= 24;
									Sleep(1);
								}

								if (east)
								{
									rigidBody.velocity.x = 0;
									camera.x += 24;
									Sleep(1);
								}

								if (camera.x >= i * 1024 && east)
								{
									camera.x = i * 1024;
									east = false;
									Game::Instance()->GetCameraMoving() = false;
								}

								if (camera.x <= i * 1024 && west)
								{
									camera.x = i * 1024;
									west = false;
									Game::Instance()->GetCameraMoving() = false;
								}

								if (south)
								{
									rigidBody.velocity.y = 0;
									camera.y += 24;
									Sleep(1);
								}

								if (north)
								{
									rigidBody.velocity.y = 0;
									camera.y -= 24;
									Sleep(1);
								}

								if (camera.y >= (j * 704) - 288 && south)
								{
									camera.y = (j * 704) - 288;
									south = false;
									Game::Instance()->GetCameraMoving() = false;
								}

								if (camera.y <= (j * 704) - 288 && north)
								{
									camera.y = (j * 704) - 288;
									north = false;
									Game::Instance()->GetCameraMoving() = false;
								}
							}
						}
					}
				}
			}

			if (entity.HasTag("player") || entity.HasTag("the_sword") || entity.HasTag("the_shield"))
			{

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
					Logger::Err("GOING SOUTH");
					south = true;
					Game::Instance()->GetCameraMoving() = true;
					rigidBody.velocity.y = 0;
				}
			}

			// Keep the camera rectange view inside the screen limits
			camera.x = camera.x < -288 ? -288 : camera.x; // If the camera.x is < 0 make it 0
			camera.y = camera.y < -288 ? -288 : camera.y;
			camera.x = (camera.x + camera.w > 16384) ? 16384 - camera.w : camera.x; // if Camera.x is > than the width of the screen
			camera.y = (camera.y + camera.h > 5376) ? 5376 - camera.h : camera.y;
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
