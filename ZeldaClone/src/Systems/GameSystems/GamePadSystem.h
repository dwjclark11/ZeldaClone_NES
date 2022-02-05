#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/KeyboardControlComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/SpriteComponent.h"
#include "KeyboardControlSystem.h"
#include <SDL.h>
#include "../../States/PauseState.h"
#include "../../States/MenuState.h"
#include "../../States/GameState.h"
#include "../../Game/Game.h"

class GamePadSystem : public System
{
private:
	class Game& game;
public:
	SDL_GameController* gameController;
	const int JOYSTICK_DEAD_ZONE = 25000;
	
	/*
		Should this be a struct?
	*/
	static bool xPressed;
	static bool yPressed;
	static bool aPressed;
	static bool bPressed;
	static bool upPressed;
	static bool downPressed;
	static bool leftPressed;
	static bool rightPressed;
	static bool buttonDown;
	static bool buttonDirDown;
	static bool paused;

	GamePadSystem() : gameController(nullptr), game(Game::Instance())
	{
		RequiredComponent<KeyboardControlComponent>();
	}

	void Init()
	{
		if (SDL_NumJoysticks() < 1)
		{
			Logger::Err("Gamepad Not Connected");
		}
		else
		{
			// Load Joystick
			gameController = SDL_GameControllerOpen(0);
			if (gameController == nullptr)
			{
				Logger::Err("Unable to open game controller!");
			}
		}
	}
	
	void Update(SDL_Event& event)
	{
		for (auto entity : GetSystemEntities())
		{
			const auto& keyboardControl = entity.GetComponent<KeyboardControlComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			//rigidbody.velocity = glm::vec2(0);
			if (entity.HasTag("player"))
			{
				if (!game.GetCameraMoving())
				{
					if (event.type == SDL_JOYAXISMOTION)
					{
						// Motion on Controller 0
						if (event.jaxis.which == 0)
						{
							// X axis Motion
							if (event.jaxis.axis == 1)
							{
								// Left of dead Zone
								if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
								{
									rigidbody.velocity = keyboardControl.upVelocity;
									sprite.srcRect.x = sprite.width * 2;
									KeyboardControlSystem::dir = UP;
								}
								else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
								{
									rigidbody.velocity = keyboardControl.downVelocity;
									sprite.srcRect.x = sprite.width * 0;
									KeyboardControlSystem::dir = DOWN;
								}
								else
									rigidbody.velocity = glm::vec2(0);
							}
							// Y axis Motion
							if (event.jaxis.axis == 0)
							{
								// Left of dead Zone
								if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
								{
									rigidbody.velocity = keyboardControl.leftVelocity;
									sprite.srcRect.x = sprite.width * 1;
									KeyboardControlSystem::dir = LEFT;
								}
								else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
								{
									rigidbody.velocity = keyboardControl.rightVelocity;
									sprite.srcRect.x = sprite.width * 3;
									KeyboardControlSystem::dir = RIGHT;
								}
								else
								rigidbody.velocity = glm::vec2(0);
							}
						}
					}

					if (event.type == SDL_CONTROLLERBUTTONDOWN)
					{
						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) == 1)
						{
							aPressed = true;
						}
						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B) == 1)
						{
							bPressed = true;
						}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_X) == 1)
						{
							xPressed = true;
						}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_Y) == 1)
						{
							yPressed = true;
						}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1)
						{
							if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE")
							{
								rigidbody.velocity = keyboardControl.upVelocity;
								sprite.srcRect.x = sprite.width * 2;
								KeyboardControlSystem::dir = UP;
							}
							else
							{
								upPressed = true;
							}
						}
						else if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1)
						{
							if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE")
							{
								rigidbody.velocity = keyboardControl.downVelocity;
								sprite.srcRect.x = sprite.width * 0;
								KeyboardControlSystem::dir = DOWN;
							}
							else
							{
								downPressed = true;
							}
						}
						else if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1)
						{
							if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE")
							{
								rigidbody.velocity = keyboardControl.leftVelocity;
								sprite.srcRect.x = sprite.width * 1;
								KeyboardControlSystem::dir = LEFT;
							}
							else
							{
								leftPressed = true;
							}
						}
						else if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1)
						{
							if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE")
							{
								rigidbody.velocity = keyboardControl.rightVelocity;
								sprite.srcRect.x = sprite.width * 3;
								KeyboardControlSystem::dir = RIGHT;
							}
							else
							{
								rightPressed = true;
							}
						}
						else if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_START) == 1)
						{
							if (game.GetStateMachine()->GetCurrentState() == "GAMESTATE")
							{
								paused = true;
								game.GetStateMachine()->PushState(new PauseState());
							}
							else if (game.GetStateMachine()->GetCurrentState() == "PAUSE")
							{
								game.GetStateMachine()->PopState();
							}
						}
					}

					if (event.type == SDL_CONTROLLERBUTTONUP)
					{
						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) == 0)
						{
							aPressed = false;
						}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B) == 0)
						{
							bPressed = false;
						}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_X) == 0)
						{
							xPressed = false;
						}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_Y) == 0)
						{
							yPressed = false;
						}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 0
							&& SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 0)
							{
								rigidbody.velocity = glm::vec2(0);

								if (game.GetStateMachine()->GetCurrentState() == "PAUSE" || game.GetStateMachine()->GetCurrentState() == "GAMEOVER")
								{
									upPressed = false;
									downPressed = false;
									leftPressed = false;
									rightPressed = false;
									buttonDirDown = false;
								}
							}

						if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_X) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_Y) == 0
							&& SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B) == 0)
						{
							buttonDown = false;
						}
					}
				}
			}
		}
	}

	void UpdateOtherStates(SDL_Event& event)
	{
		if (game.GetStateMachine()->GetCurrentState() == "TITLESTATE")
		{
			if (event.type == SDL_CONTROLLERBUTTONDOWN)
			{
				if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_START) == 1)
				{
					game.GetStateMachine()->PopState();
					game.GetStateMachine()->PushState(new MenuState());
					buttonDown = true;
				}
			}

			if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_X) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_Y) == 0
				&& SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B) == 0)
			{
				buttonDown = false;
			}
		}
		else if (game.GetStateMachine()->GetCurrentState() == "MENU" || game.GetStateMachine()->GetCurrentState() == "GAMEOVER")
		{
			if (event.type == SDL_CONTROLLERBUTTONDOWN)
			{
				if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) == 1)
				{
					aPressed = true;
				}

				if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1)
				{
					upPressed = true;
				}
				else if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1)
				{
					downPressed = true;
				}
				else if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1)
				{
					leftPressed = true;
				}
				else if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1)
				{
					rightPressed = true;
				}
				if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_START) == 1)
				{
					//game.GetStateMachine()->PopState();
					//game.GetStateMachine()->PushState(new MenuState());
					//buttonDown = true;
				}
			}

			if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 0
				&& SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 0)
			{
				upPressed = false;
				downPressed = false;
				leftPressed = false;
				rightPressed = false;
				buttonDirDown = false;
			}

			if (SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_X) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_Y) == 0
				&& SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) == 0 && SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B) == 0)
			{
				buttonDown = false;
			}
		}
	}
	//bool GetXButton() { return xPressed; }
	//bool GetYButton() { return yPressed; }
	//bool GetAButton() { return aPressed; }
	//bool GetBButton() { return bPressed; }
	void EmptyFunction();
private:

};
