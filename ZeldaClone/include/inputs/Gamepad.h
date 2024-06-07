#pragma once
#include "inputs/GPButtons.h"
#include "inputs/Button.h"
#include "Utilities/Utility.h"
#include <sol/sol.hpp>

class Gamepad
{
  private:
	Button m_Buttons[ GP_BTN_LAST ];
	int m_BtnPressed;
	std::string m_sBtnString;

	Controller m_pController;

  public:
	Gamepad();
	~Gamepad() = default;

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
	void OnButtonDown( int button );

	/*
	 *  OnButtonUp is used to detect if there is a button up
	 *  while polling the inputs during an SDL_Event.
	 *  This needs to be placed in the SDL event loop.
	 */
	void OnButtonUp( int button );

	/*
		Check to see if the given button is held down
	*/
	bool IsButtonHeld( int button ) const;

	/*
		Check to see if the given button has been momentarily
		pressed. The button must be released to reset it's state
		and check again
	*/
	bool IsButtonJustPressed( int button ) const;

	/*
	 * Check to see if the given button has just been
	 * released.
	 */
	bool IsButtonJustReleased( int button ) const;

	/*
	 *	IsGamepadPresent() - Returns the controller pointer.
	 *	if it is null --> it will be false!
	 */
	const bool IsGamepadPresent() const;

	inline Controller& GetController() { return m_pController; }
};
