#include "inputs/Gamepad.h"
#include "Logger/Logger.h"

Gamepad::Gamepad()
	: m_pController( nullptr )
	, m_BtnPressed( -1 )
{
	if ( SDL_NumJoysticks() < 1 )
		return;

	// Load Joystick
	m_pController = MakeController(SDL_GameControllerOpen( 0 ));

	if ( m_pController == nullptr )
	{
		Logger::Err( "Unable to open game controller!" );
	}
}

void Gamepad::Update()
{
	// If there are no controllers plugged in, we are going to want to check if one gets plugged in later.
	if ( !m_pController )
	{
		if ( SDL_NumJoysticks() >= 1 )
		{
			// Load Joystick
			m_pController = static_cast<Controller>( SDL_GameControllerOpen( 0 ) );
			if ( m_pController == nullptr )
			{
				Logger::Err( "Unable to open game controller!" );
			}
			else
				Logger::Log( "Gamepad Controller Opened Successfully!" );
		}
		else
		{
			// There is nothing to update if there is no controller
			return;
		}
	}
	else // This checks to see if the controller was unplugged or lost and Closes it
	{
		if ( SDL_NumJoysticks() == 0 )
		{
			SDL_GameController* temp = m_pController.release();
			SDL_GameControllerClose( temp );
			temp = nullptr;
		}
	}

	// Reset the Gamepad Button states
	for ( int i = 0; i < GP_BTN_LAST; i++ )
	{
		m_Buttons[ i ].bJustPressed = false;
		m_Buttons[ i ].bJustReleased = false;
	}
}

void Gamepad::OnButtonDown( int button )
{
	// Check to see if the button index is defined
	if ( button >= GP_BTN_LAST )
	{
		Logger::Err( "FILE: Gamepad.cpp __LINE: 30 - [" + std::to_string( button ) + "] button is beyond the index!" );
		return;
	}
	// JADE_LOG("Gamepad button [" + std::to_string(button) + "] pressed!");

	m_BtnPressed = button;

	// Update the Down event
	m_Buttons[ button ].Update( true );
}

void Gamepad::OnButtonUp( int button )
{
	// Check to see if the button index is defined
	if ( button >= GP_BTN_LAST )
	{
		Logger::Err( "FILE: Gamepad.cpp __LINE: 42 - [" + std::to_string( button ) + "] button is beyond the index!" );
		return;
	}
	// Update the Button up event
	m_Buttons[ button ].Update( false );
}

bool Gamepad::IsButtonHeld( int button ) const
{
	// Check to see if the button index is defined
	if ( button >= GP_BTN_LAST )
	{
		Logger::Err( "FILE: Gamepad.cpp __LINE: 54 - [" + std::to_string( button ) + "] button is beyond the index!" );
		return false;
	}
	// Update the Down event
	return m_Buttons[ button ].bIsDown;
}

bool Gamepad::IsButtonJustPressed( int button ) const
{
	// Check to see if the button index is defined
	if ( button >= GP_BTN_LAST )
	{
		Logger::Err( "FILE: Gamepad.cpp __LINE: 67 - [" + std::to_string( button ) + "] button is beyond the index!" );
		return false;
	}
	// Update the Down event
	return m_Buttons[ button ].bJustPressed;
}

bool Gamepad::IsButtonJustReleased( int button ) const
{
	// Check to see if the button index is defined
	if ( button >= GP_BTN_LAST )
	{
		Logger::Err( "FILE: Gamepad.cpp __LINE: 79 - [" + std::to_string( button ) + "] button is beyond the index!" );
		return false;
	}
	// Update the Down event
	return m_Buttons[ button ].bJustReleased;
}

const bool Gamepad::IsGamepadPresent() const
{
	return ( m_pController != nullptr ) && ( SDL_NumJoysticks() > 0 );
}
