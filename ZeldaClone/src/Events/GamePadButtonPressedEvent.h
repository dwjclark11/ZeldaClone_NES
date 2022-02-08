#pragma once
#include "../ECS/ECS.h"
#include "EventManager.h"
#include <SDL.h>

class GamePadButtonPressedEvent : public Event
{
public:
	
	//SDL_GameControllerButton button;

	GamePadButtonPressedEvent() {}
};

/*
Look into this
SDL_Joystick *joy;

// Initialize the joystick subsystem
SDL_InitSubSystem(SDL_INIT_JOYSTICK);

// Check for joystick
if (SDL_NumJoysticks() > 0) {
    // Open joystick
    joy = SDL_JoystickOpen(0);

    if (joy) {
        printf("Opened Joystick 0\n");
        printf("Name: %s\n", SDL_JoystickNameForIndex(0));
        printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
        printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
    } else {
        printf("Couldn't open Joystick 0\n");
    }

    // Close if opened
    if (SDL_JoystickGetAttached(joy)) {
        SDL_JoystickClose(joy);
    }
}

*/