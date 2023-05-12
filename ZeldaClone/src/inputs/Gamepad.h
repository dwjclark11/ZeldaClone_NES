#pragma once
#pragma once
#include "GPButtons.h"
#include "Button.h"
#include <sol/sol.hpp>
#include "../Utilities/Utility.h"

class Gamepad
{
private:
	Button buttons[GP_BTN_LAST];
	int mBtnPressed;
	std::string mBtnString;
	/*
	*  Register the defined	buttons to lua using sol::set()
	*  so they can be accessed by name in lua script
	*/
	static void RegisterLuaGPBtnNames(sol::state& lua);
	Controller mController;

public:
	Gamepad();
	~Gamepad();

	/*
	*   Gamepad.Update() resets the status of all
	*   the buttons in the buttons[].
	*/
	void Update();

	/*
	*  OnButtonDown is used to detect if there is a button down
	*  while polling the inputs during an SDL_Event.
	*  This needs to be placed in the SDL event loop.
	*/
	void OnButtonDown(int button);

	/*
	*  OnButtonUp is used to detect if there is a button up
	*  while polling the inputs during an SDL_Event.
	*  This needs to be placed in the SDL event loop.
	*/
	void OnButtonUp(int button);

	/*
		Check to see if the given button is held down
	*/
	bool IsButtonHeld(int button) const;

	/*
		Check to see if the given button has been momentarily
		pressed. The button must be released to reset it's state
		and check again
	*/
	bool IsButtonJustPressed(int button) const;

	/*
	* Check to see if the given button has just been
	* released.
	*/
	bool IsButtonJustReleased(int button) const;

	/*
	*	IsGamepadPresent() - Returns the controller pointer.
	*	if it is null --> it will be false!
	*/
	const bool IsGamepadPresent() const;

	Controller& GetController() { return mController; }
};
