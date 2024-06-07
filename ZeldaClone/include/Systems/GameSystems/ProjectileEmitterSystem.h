#pragma once
#include "ECS/ECS.h"
#include "Events/KeyPressedEvent.h"
#include "Utilities/Timer.h"
#include "Game/Game.h"
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <map>

class ProjectileEmitterSystem : public System
{
  private:
	struct ProjectileAttrib
	{
		std::string group{ "" }, sprite_name{ "" };
		int width{ 0 }, height{ 0 };
		int srcRectX{ 0 }, srcRectY{ 0 };
		int duration{ 3000 }, numFrames{ 0 };
		glm::vec2 scale{ 0.f }, transOffsetUp{ 0.f }, transOffsetDown{ 0.f };
		glm::vec2 transOffsetLeft{ 0.f }, transOffsetRight{ 0.f }, boxSizeUp{ 0.f };
		glm::vec2 boxSizeDown{ 0.f }, boxSizeLeft{ 0.f }, boxSizeRight{ 0.f };
		glm::vec2 upOffset{ 0.f }, downOffset{ 0.f }, leftOffset{ 0.f }, rightOffset{ 0.f };
		bool bAnimation{ false }, bVertical{ false };

		std::string ToString();
	};

  private:
	Timer m_SwordTimer;
	class Game& m_Game;
	class GameData& m_GameData;
	class InputManager& m_InputManager;
	Registry& m_Registry;
	bool m_bFullLife;

	std::map<std::string, ProjectileAttrib> m_mapProjectileAttribs;

	void UseItem( ProjectileAttrib attrib );
	void UseSword();
	void UseMagicWand();
	void LoadMapAttributes( sol::state& lua, const std::string& fileName = "ProjectileAttributes" );

#ifdef _DEBUG
	// Testing Functions --> Use these to quickly reload Lua Table Attributes for adjusting positions
	void ClearItemMap();
	void ReloadItemMap();
#endif

	void ItemUsed();
	void SwordUsed();
	void EnemyProjectileUpdate( Entity& entity );
	void BossProjectileUpdate( Entity& entity );

  public:
	ProjectileEmitterSystem();
	void SubscribeKeyToEvents( EventManager& eventManager );
	void SubscribeBtnToEvents( EventManager& eventManager );
	void OnKeyPressed( KeyPressedEvent& event );
	void OnBtnPressed( GamePadButtonPressedEvent& event );
	void Update();
};