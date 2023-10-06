#include "Gamepad.h"
#include "../Logger/Logger.h"

Gamepad::Gamepad()
	: mController(nullptr)
	, mBtnPressed(-1)
{
	Logger::Log("Gamepad Constructor Called!");

	if (SDL_NumJoysticks() < 1)
	{
		Logger::Err("Gamepad Not Connected");
	}
	else
	{
		// Load Joystick
		mController = static_cast<Controller>(SDL_GameControllerOpen(0));
		if (mController == nullptr)
		{
			Logger::Err("Unable to open game controller!");
		}
	}
}

Gamepad::~Gamepad()
{
	Logger::Log("Gamepad Destructor Called");
}

void Gamepad::Update()
{
	// If there are no controllers plugged in, we are going to want to check if one gets plugged in later.
	if (!mController)
	{
		if (SDL_NumJoysticks() >= 1)
		{
			// Load Joystick
			mController = static_cast<Controller>(SDL_GameControllerOpen(0));
			if (mController == nullptr)
			{
				Logger::Err("Unable to open game controller!");
			}
			else
				Logger::Log("Gamepad Controller Opened Successfully!");
		}
		else
		{
			// There is nothing to update if there is no controller
			return;
		}
	}
	else // This checks to see if the controller was unplugged or lost and Closes it
	{
		if (SDL_NumJoysticks() == 0)
		{
			SDL_GameController* temp = mController.release();
			SDL_GameControllerClose(temp);
			temp = nullptr;
		}
	}

	// Reset the Gamepad Button states
	for (int i = 0; i < GP_BTN_LAST; i++)
	{
		buttons[i].justPressed = false;
		buttons[i].justReleased = false;
	}
}

void Gamepad::OnButtonDown(int button)
{
	// Check to see if the button index is defined
	if (button > GP_BTN_LAST)
	{
		Logger::Err("FILE: Gamepad.cpp __LINE: 30 - [" + std::to_string(button) + "] button is beyond the index!");
		return;
	}
	//JADE_LOG("Gamepad button [" + std::to_string(button) + "] pressed!");

	mBtnPressed = button;

	// Update the Down event
	buttons[button].Update(true);
}

void Gamepad::OnButtonUp(int button)
{
	// Check to see if the button index is defined
	if (button > GP_BTN_LAST)
	{
		Logger::Err("FILE: Gamepad.cpp __LINE: 42 - [" + std::to_string(button) + "] button is beyond the index!");
		return;
	}
	// Update the Button up event
	buttons[button].Update(false);
}

bool Gamepad::IsButtonHeld(int button) const
{
	// Check to see if the button index is defined
	if (button > GP_BTN_LAST)
	{
		Logger::Err("FILE: Gamepad.cpp __LINE: 54 - [" + std::to_string(button) + "] button is beyond the index!");
		return false;
	}
	// Update the Down event
	return buttons[button].isDown;

}

bool Gamepad::IsButtonJustPressed(int button) const
{
	// Check to see if the button index is defined
	if (button > GP_BTN_LAST)
	{
		Logger::Err("FILE: Gamepad.cpp __LINE: 67 - [" + std::to_string(button) + "] button is beyond the index!");
		return false;
	}
	// Update the Down event
	return buttons[button].justPressed;
}

bool Gamepad::IsButtonJustReleased(int button) const
{
	// Check to see if the button index is defined
	if (button > GP_BTN_LAST)
	{
		Logger::Err("FILE: Gamepad.cpp __LINE: 79 - [" + std::to_string(button) + "] button is beyond the index!");
		return false;
	}
	// Update the Down event
	return buttons[button].justReleased;
}

const bool Gamepad::IsGamepadPresent() const
{
	return (mController != nullptr) && (SDL_NumJoysticks() > 0);
}
