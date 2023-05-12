#pragma once
#include <map>
#include <SDL.h>
#include <memory>

class Keyboard;
class Gamepad;

class InputManager
{

public: 
	enum class Action
	{
		MOVE_UP = 0, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
		ATTACK, USE_ITEM, PAUSE, SELECT, CANCEL,
	};
private:
	std::map<Action, SDL_Keycode> mMappedKeys;
	std::map<Action, SDL_GameControllerButton> mMappedButtons;
	void CreateDefaultBtnBindings();
	void CreateDefaultKeyBindings();

	InputManager();
	static std::unique_ptr<InputManager> m_pInstance;
	std::unique_ptr<Keyboard> m_pKeyboard;
	std::unique_ptr<Gamepad> m_pGamepad;
	bool m_bKeydown, m_bPaused, m_bAttack;

public:
	static InputManager& GetInstance();
	~InputManager();
	void AddKeyToMap(Action action, SDL_Keycode key);
	void ChangeKeyBinding(Action action, SDL_Keycode key);
	const std::map<Action, SDL_Keycode>& GetKeyBindings() const { return mMappedKeys; }
	
	const SDL_Keycode GetKeyCode(Action action);
	const SDL_GameControllerButton GetBtnCode(Action action);
	void AddBtnToMap(Action action, SDL_GameControllerButton button);
	void ChangeBtnBinding(Action action, SDL_GameControllerButton button);
	const std::map<Action, SDL_GameControllerButton>& GetBtnBindings() const { return mMappedButtons; }
	Keyboard& GetKeyboard() { return *m_pKeyboard; }
	Gamepad& GetGamepad() { return *m_pGamepad; }
	const bool IsPaused() const { return m_bPaused; }
	void SetPaused(bool pause) { m_bPaused = pause; }
	const bool IsAttacking() const { return m_bAttack; }
	void SetAttack(bool attack) { m_bAttack = attack; }
};