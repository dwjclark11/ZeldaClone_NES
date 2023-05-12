#pragma once
#include "../../ECS/ECS.h"
#include "../../Events/KeyPressedEvent.h"
#include "../../Utilities/Timer.h"
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include "../../Game/Game.h"

class ProjectileEmitterSystem : public System
{
private:
	Timer swordTimer;
	class Game& game;
	class GameData& gameData;
	class InputManager& inputManager;
	Registry& registry;

	struct ProjectileAttrib
	{
		std::string group, sprite_name;
		int width, height;
		int srcRectX, srcRectY;
		glm::vec2 scale, transOffsetUp, transOffsetDown;
		glm::vec2 transOffsetLeft, transOffsetRight, boxSizeUp;
		glm::vec2 boxSizeDown, boxSizeLeft, boxSizeRight;
		glm::vec2 upOffset, downOffset, leftOffset, rightOffset;
		
		// Animation attributes
		int duration, numFrames;
		bool animation, vertical;

		ProjectileAttrib()
			: ProjectileAttrib("", "", 0, 0, 0, 0, 0, 
				glm::vec2(0), glm::vec2(0), glm::vec2(0),
				glm::vec2(0), glm::vec2(0), glm::vec2(0), 
				glm::vec2(0), glm::vec2(0), glm::vec2(0),
				glm::vec2(0), glm::vec2(0), glm::vec2(0), 
				glm::vec2(0), 3000, false, false)
		{
			
		};

		ProjectileAttrib(const std::string& group, const std::string& sprite_name, 
			int width, int height, int src_rect_x, int src_rect_y, int num_frames, glm::vec2 scale,
			glm::vec2 trans_offset_up, glm::vec2 trans_offset_down, glm::vec2 trans_offset_right, 
			glm::vec2 trans_offset_left, glm::vec2 box_size_up, glm::vec2 box_size_down, 
			glm::vec2 box_size_left, glm::vec2 box_size_right, glm::vec2 box_up_offset, glm::vec2 box_down_offset,
			glm::vec2 box_right_offset, glm::vec2 box_left_offset, int duration = 3000, 
			bool animation = false, bool vertical = false)
			: group{group}, sprite_name{sprite_name}, width{width}, height{height}
			, srcRectX{src_rect_x}, srcRectY{src_rect_y}, scale{scale}
			, transOffsetUp{trans_offset_up}, transOffsetDown{ trans_offset_down }
			, transOffsetLeft{trans_offset_left}, transOffsetRight{ trans_offset_right }
			, boxSizeUp{box_size_up}, boxSizeDown{ box_size_down}
			, boxSizeLeft{box_size_left}, boxSizeRight{ box_size_right}
			, upOffset{box_up_offset}, downOffset{ box_down_offset}
			, leftOffset{box_left_offset}, rightOffset{ box_right_offset}
			, duration{duration}, numFrames{num_frames}, animation{animation}, vertical{vertical}
		{

		}

		// This is also for testing to see what the current projectile attributes are
		std::string ToString();

	};

	bool fullLife;
	
	std::map<std::string, ProjectileAttrib> projectileAttributeMap;

	void UseItem(ProjectileAttrib attrib);
	void UseSword();
	void UseMagicWand();
	void LoadMapAttributes(sol::state& lua, const std::string& fileName = "ProjectileAttributes");
#ifdef _DEBUG
	// Testing Functions --> Use these to quickly reload Lua Table Attributes for adjusting positions
	void ClearItemMap();
	void ReloadItemMap();
#endif
	void ItemUsed();
	void SwordUsed();
	void EnemyProjectileUpdate(Entity& entity);
	void BossProjectileUpdate(Entity& entity);

public:

	ProjectileEmitterSystem();
	void SubscribeKeyToEvents(std::unique_ptr<EventManager>& eventManager);
	void SubscribeBtnToEvents(std::unique_ptr<EventManager>& eventManager);
	void OnKeyPressed(KeyPressedEvent& event);
	void OnBtnPressed(GamePadButtonPressedEvent& event);
	void Update();
};