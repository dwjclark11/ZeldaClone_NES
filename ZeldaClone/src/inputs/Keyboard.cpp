#include "Keyboard.h"
#include "../Logger/Logger.h"
#include <memory>
#include <regex>


/*
    HACK - This converts the SDLKey number to as usable number
    for the keys array so we can keep track if the key is just pressed/released
    or held
*/

int ConvertKeysNum(int key)
{
    // if the key is smaller than the defined last key, 
    // it does not have to be converted
    if (key < KEY_LAST)
        return key;

    key -= 1073741700;


    //JADE_LOG("Key" + std::to_string(key));
    return key;
}

Keyboard::Keyboard()
    : mKeyString("")
{
    mKeyPressed = static_cast<SDL_KeyCode>(-1);
}

void Keyboard::Update()
{
    for (int i = 0; i < KEY_LAST; i++)
    {
        keys[i].justPressed = false;
        keys[i].justReleased = false;
    }
}

void Keyboard::OnKeyDown(int key)
{
    int cKey = ConvertKeysNum(key);

    // Check to see if the key index is defined
    if (cKey > KEY_LAST)
    {
        Logger::Err("[" + std::to_string(cKey) + "] key is beyond the index!");
        return;
    }
    mKeyPressed = static_cast<SDL_KeyCode>(key);
    // Update the Down event
    keys[cKey].Update(true);
}

void Keyboard::OnKeyUp(int key)
{
    int cKey = ConvertKeysNum(key);
    // Check to see if the key index is defined
    if (cKey > KEY_LAST)
    {
        Logger::Err("[" + std::to_string(cKey) + "] key is beyond the index!");
        return;
    }
    // Update the Up event
    keys[cKey].Update(false);

}

bool Keyboard::IsKeyHeld(int key) const
{
    int cKey = ConvertKeysNum(key);
    // Check to see if the key index is defined
    if (cKey > KEY_LAST)
    {
        Logger::Err("[" + std::to_string(cKey) + "] key is beyond the index!");
        return false;
    }

    // return if that key is down or not
    return keys[cKey].isDown;
}

bool Keyboard::IsKeyJustPressed(int key) const
{
    int cKey = ConvertKeysNum(key);
    // Check to see if the key index is defined
    if (cKey > KEY_LAST)
    {
        Logger::Err("[" + std::to_string(cKey) + "] key is beyond the index!");
        return false;
    }

    // return if that key has just been pressed
    return keys[cKey].justPressed;
}

bool Keyboard::IsKeyJustReleased(int key) const
{
    int cKey = ConvertKeysNum(key);
    // Check to see if the key index is defined
    if (cKey >= KEY_LAST)
    {
        Logger::Err("[" + std::to_string(cKey) + "] key is beyond the index!");
        return false;
    }

    // return if that key has just been released
    return keys[cKey].justReleased;
}

void Keyboard::SetSDLKeyString()
{
    mKeyString = std::string(SDL_GetKeyName(mKeyPressed));
}

std::string Keyboard::GetKeyString()
{
    ConvertKeyString();
    return mKeyString;
}

const int Keyboard::GetPressedKeyCode()
{
    return mKeyPressed;
}

void Keyboard::ConvertKeyString()
{

    auto length = mKeyString.length();
    std::string temp = "";

    if (length > 1)
    {
        if (mKeyString == "Return")
        {
            mKeyString = "KEY_ENTER";
        }
        else if (mKeyString.find("Right Shift") != std::string::npos)
        {
            mKeyString = "KEY_RSHIFT";
        }
        else if (mKeyString.find("Left Shift") != std::string::npos)
        {
            mKeyString = "KEY_LSHIFT";
        }
        else if (mKeyString.find("Right Ctrl") != std::string::npos)
        {
            mKeyString = "KEY_RCTRL";
        }
        else if (mKeyString.find("Left Ctrl") != std::string::npos)
        {
            mKeyString = "KEY_LCTRL";
        }
        else if (mKeyString.find("Right") != std::string::npos)
        {
            mKeyString = "KEY_RIGHT";
        }
        else if (mKeyString.find("Left") != std::string::npos)
        {
            mKeyString = "KEY_LEFT";
        }
        else if (mKeyString.find("Up") != std::string::npos)
        {
            mKeyString = "KEY_UP";
        }
        else if (mKeyString.find("Down") != std::string::npos)
        {
            mKeyString = "KEY_DOWN";
        }
        else if (mKeyString.find("Space") != std::string::npos)
        {
            mKeyString = "KEY_SPACE";
        }
        else if (mKeyString.find("Backspace") != std::string::npos)
        {
            mKeyString = "KEY_BACKSPACE";
        }
        else if (mKeyString.find("Keypad") != std::string::npos)
        {
            char num = mKeyString[mKeyString.size() - 1];
            temp = "KP_KEY_";

            temp += num;

            mKeyString = temp;
        }
    }
    else
    {
        // Create regix variables
        std::regex regexp("[A-Z]");
        std::smatch m;

        if (std::regex_search(mKeyString, m, regexp))
        {
            char key = mKeyString[0];
            temp = "KEY_";
            temp += key;
            mKeyString = temp;
        }
    }
}
