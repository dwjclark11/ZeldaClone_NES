#include "inputs/Keyboard.h"
#include "Logger/Logger.h"
#include <memory>
#include <regex>

/*
	HACK - This converts the SDLKey number to as usable number
	for the keys array so we can keep track if the key is just pressed/released
	or held
*/

int ConvertKeysNum( int key )
{
	// if the key is smaller than the defined last key,
	// it does not have to be converted
	if ( key < KEY_LAST )
		return key;

	key -= 1073741700;

	// JADE_LOG("Key" + std::to_string(key));
	return key;
}

Keyboard::Keyboard()
	: m_sKeyString( "" )
{
	m_KeyPressed = static_cast<SDL_KeyCode>( -1 );
}

void Keyboard::Update()
{
	for ( int i = 0; i < KEY_LAST; i++ )
	{
		m_Keys[ i ].bJustPressed = false;
		m_Keys[ i ].bJustReleased = false;
	}
}

void Keyboard::OnKeyDown( int key )
{
	int cKey = ConvertKeysNum( key );

	// Check to see if the key index is defined
	if ( cKey > KEY_LAST )
	{
		Logger::Err( "[" + std::to_string( cKey ) + "] key is beyond the index!" );
		return;
	}
	m_KeyPressed = static_cast<SDL_KeyCode>( key );
	// Update the Down event
	m_Keys[ cKey ].Update( true );
}

void Keyboard::OnKeyUp( int key )
{
	int cKey = ConvertKeysNum( key );
	// Check to see if the key index is defined
	if ( cKey > KEY_LAST )
	{
		Logger::Err( "[" + std::to_string( cKey ) + "] key is beyond the index!" );
		return;
	}
	// Update the Up event
	m_Keys[ cKey ].Update( false );
}

bool Keyboard::IsKeyHeld( int key ) const
{
	int cKey = ConvertKeysNum( key );
	// Check to see if the key index is defined
	if ( cKey > KEY_LAST )
	{
		Logger::Err( "[" + std::to_string( cKey ) + "] key is beyond the index!" );
		return false;
	}

	// return if that key is down or not
	return m_Keys[ cKey ].bIsDown;
}

bool Keyboard::IsKeyJustPressed( int key ) const
{
	int cKey = ConvertKeysNum( key );
	// Check to see if the key index is defined
	if ( cKey > KEY_LAST )
	{
		Logger::Err( "[" + std::to_string( cKey ) + "] key is beyond the index!" );
		return false;
	}

	// return if that key has just been pressed
	return m_Keys[ cKey ].bJustPressed;
}

bool Keyboard::IsKeyJustReleased( int key ) const
{
	int cKey = ConvertKeysNum( key );
	// Check to see if the key index is defined
	if ( cKey >= KEY_LAST )
	{
		Logger::Err( "[" + std::to_string( cKey ) + "] key is beyond the index!" );
		return false;
	}

	// return if that key has just been released
	return m_Keys[ cKey ].bJustReleased;
}

void Keyboard::SetSDLKeyString()
{
	m_sKeyString = std::string( SDL_GetKeyName( m_KeyPressed ) );
}

std::string Keyboard::GetKeyString()
{
	ConvertKeyString();
	return m_sKeyString;
}

const int Keyboard::GetPressedKeyCode()
{
	return m_KeyPressed;
}

void Keyboard::ConvertKeyString()
{

	auto length = m_sKeyString.length();
	std::string temp = "";

	if ( length > 1 )
	{
		if ( m_sKeyString == "Return" )
		{
			m_sKeyString = "KEY_ENTER";
		}
		else if ( m_sKeyString.find( "Right Shift" ) != std::string::npos )
		{
			m_sKeyString = "KEY_RSHIFT";
		}
		else if ( m_sKeyString.find( "Left Shift" ) != std::string::npos )
		{
			m_sKeyString = "KEY_LSHIFT";
		}
		else if ( m_sKeyString.find( "Right Ctrl" ) != std::string::npos )
		{
			m_sKeyString = "KEY_RCTRL";
		}
		else if ( m_sKeyString.find( "Left Ctrl" ) != std::string::npos )
		{
			m_sKeyString = "KEY_LCTRL";
		}
		else if ( m_sKeyString.find( "Right" ) != std::string::npos )
		{
			m_sKeyString = "KEY_RIGHT";
		}
		else if ( m_sKeyString.find( "Left" ) != std::string::npos )
		{
			m_sKeyString = "KEY_LEFT";
		}
		else if ( m_sKeyString.find( "Up" ) != std::string::npos )
		{
			m_sKeyString = "KEY_UP";
		}
		else if ( m_sKeyString.find( "Down" ) != std::string::npos )
		{
			m_sKeyString = "KEY_DOWN";
		}
		else if ( m_sKeyString.find( "Space" ) != std::string::npos )
		{
			m_sKeyString = "KEY_SPACE";
		}
		else if ( m_sKeyString.find( "Backspace" ) != std::string::npos )
		{
			m_sKeyString = "KEY_BACKSPACE";
		}
		else if ( m_sKeyString.find( "Keypad" ) != std::string::npos )
		{
			char num = m_sKeyString[ m_sKeyString.size() - 1 ];
			temp = "KP_KEY_";

			temp += num;

			m_sKeyString = temp;
		}
	}
	else
	{
		// Create regix variables
		std::regex regexp( "[A-Z]" );
		std::smatch m;

		if ( std::regex_search( m_sKeyString, m, regexp ) )
		{
			char key = m_sKeyString[ 0 ];
			temp = "KEY_";
			temp += key;
			m_sKeyString = temp;
		}
	}
}
