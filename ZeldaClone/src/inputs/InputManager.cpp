#include "InputManager.h"
#include "Keyboard.h"
#include "Gamepad.h"
std::unique_ptr<InputManager> InputManager::m_pInstance = nullptr;

void InputManager::CreateDefaultBtnBindings()
{
	AddBtnToMap(Action::MOVE_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
	AddBtnToMap(Action::MOVE_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	AddBtnToMap(Action::MOVE_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	AddBtnToMap(Action::MOVE_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	AddBtnToMap(Action::ATTACK, SDL_CONTROLLER_BUTTON_A);
	AddBtnToMap(Action::USE_ITEM, SDL_CONTROLLER_BUTTON_X);
	AddBtnToMap(Action::PAUSE, SDL_CONTROLLER_BUTTON_START);
	AddBtnToMap(Action::SELECT, SDL_CONTROLLER_BUTTON_A);
	AddBtnToMap(Action::CANCEL, SDL_CONTROLLER_BUTTON_B);
}

void InputManager::CreateDefaultKeyBindings()
{
	AddKeyToMap(Action::MOVE_UP, SDLK_w);
	AddKeyToMap(Action::MOVE_RIGHT, SDLK_d);
	AddKeyToMap(Action::MOVE_DOWN, SDLK_s);
	AddKeyToMap(Action::MOVE_LEFT, SDLK_a);
	AddKeyToMap(Action::ATTACK, SDLK_RSHIFT);
	AddKeyToMap(Action::USE_ITEM, SDLK_SPACE);
	AddKeyToMap(Action::PAUSE, SDLK_q);
	AddKeyToMap(Action::SELECT, SDLK_SPACE);
	AddKeyToMap(Action::CANCEL, SDLK_BACKSPACE);
}

InputManager::InputManager()
	: m_pKeyboard{ std::make_unique<Keyboard>() }, m_pGamepad{ std::make_unique<Gamepad>() }
	, m_bKeydown{false} , m_bPaused{false}, m_bAttack{false}
{
	CreateDefaultBtnBindings();
	CreateDefaultKeyBindings();
}

InputManager& InputManager::GetInstance()
{
	if (!m_pInstance)
		m_pInstance.reset(new InputManager());

	return *m_pInstance;
}

void InputManager::AddKeyToMap(Action action, SDL_Keycode key)
{
	if (!mMappedKeys.contains(action))
		mMappedKeys.emplace(action, key);	
}

void InputManager::ChangeKeyBinding(Action action, SDL_Keycode key)
{
	auto keyItr = mMappedKeys.find(action);
	if (keyItr != mMappedKeys.end())
		keyItr->second = key;
}

const SDL_Keycode InputManager::GetKeyCode(Action action)
{
	auto keyItr = mMappedKeys.find(action);
	if (keyItr != mMappedKeys.end())
		return keyItr->second;

	return SDLK_UNKNOWN;
}

const SDL_GameControllerButton InputManager::GetBtnCode(Action action)
{
	auto btnItr = mMappedButtons.find(action);
	if (btnItr != mMappedButtons.end())
		return btnItr->second;

	return SDL_CONTROLLER_BUTTON_INVALID;
}

void InputManager::AddBtnToMap(Action action, SDL_GameControllerButton button)
{
	if (!mMappedButtons.contains(action))
		mMappedButtons.emplace(action, button);	
}

void InputManager::ChangeBtnBinding(Action action, SDL_GameControllerButton button)
{
	auto btnItr = mMappedButtons.find(action);

	if (btnItr != mMappedButtons.end())
		btnItr->second = button;
}
