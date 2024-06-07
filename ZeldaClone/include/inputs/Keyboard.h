#pragma once
#include "inputs/Keys.h"
#include "inputs/Button.h"
#include <sol/sol.hpp>
#include <SDL.h>

class Keyboard
{
  private:
	// An array of buttons that holds the key status
	Button m_Keys[ KEY_LAST ];
	SDL_KeyCode m_KeyPressed;
	std::string m_sKeyString;

  public:
	Keyboard();
	~Keyboard() = default;

	/*
	 *   Keyboard.Update() resets the status of all
	 *   the keys in the keys[].
	 */
	void Update();

	/*
	 *  OnKeyDown is used to detect if there is a key down
	 *  while polling the inputs during an SDL_Event.
	 *  This needs to be placed in the SDL event loop.
	 */
	void OnKeyDown( int key );

	/*
	 *  OnKeyUp is used to detect if there is a key up
	 *  while polling the inputs during an SDL_Event.
	 *  This needs to be placed in the SDL event loop.
	 */
	void OnKeyUp( int key );

	/*
		Checks to see if the key is being held down
	*/
	bool IsKeyHeld( int key ) const;

	/*
		Checks to see if the key has just be pressed
		The key must be released to reset it's state
		and check again
	*/
	bool IsKeyJustPressed( int key ) const;

	/*
		Checks to see if the key has been released
	*/
	bool IsKeyJustReleased( int key ) const;

	/*
	 *  SetSDLKeyString() -- Uses the SDL_KeyCode that was determined in
	 *  OnKeyPressed() to call SDL_GetKeyName and sets mKeyString
	 */
	void SetSDLKeyString();

	/*
	 *	GetKeyString() returns the mKeyString variable after converting it
	 *	from the SDL const char* value to a Mapped Engine/Lua Value
	 */
	std::string GetKeyString();
	const int GetPressedKeyCode();
	/*
	 *	ConvertKeyString() -- Tries to convert the mKeystring variable from
	 *	an SDL value to a mapped Engine/Lua value.
	 *	--- NOT ALL KEYS ARE MAPPED!!! ---
	 */
	void ConvertKeyString();
};
