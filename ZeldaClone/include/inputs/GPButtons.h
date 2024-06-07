#pragma once
/*
*  Gamepad Button Definitions
*  These buttons are defined based on an
*  X_BOX style Controller and are based on
*  the SDL_GameControllerButton values
*/
constexpr int GP_BTN_INVALID		= -1;

// ==============================================
// Action Buttons
// ==============================================
constexpr int GP_BTN_A				= 0;
constexpr int GP_BTN_B				= 1;
constexpr int GP_BTN_X				= 2;
constexpr int GP_BTN_Y				= 3;

// ==============================================
// Menu/Control Buttons
// ==============================================
constexpr int GP_BTN_BACK			= 4;
constexpr int GP_BTN_GUIDE			= 5;
constexpr int GP_BTN_START			= 6;

// ==============================================
// Joystick and Shoulder/Trigger Buttons
// ==============================================
constexpr int GP_BTN_LSTICK			= 7;
constexpr int GP_BTN_RSTICK			= 8;
constexpr int GP_BTN_LSHOULDER		= 9;
constexpr int GP_BTN_RSHOULDER		= 10;

// ==============================================
// Directional Buttons - DPAD
// ==============================================
constexpr int GP_BTN_DPAD_UP		= 11;
constexpr int GP_BTN_DPAD_DOWN		= 12;
constexpr int GP_BTN_DPAD_LEFT		= 13;
constexpr int GP_BTN_DPAD_RIGHT		= 14;

constexpr int GP_BTN_LAST			= 21;
