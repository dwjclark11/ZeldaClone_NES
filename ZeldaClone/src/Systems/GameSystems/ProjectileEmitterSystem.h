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

	struct ProjectileAttrib
	{
		std::string group;
		std::string sprite_name;
		int width;
		int height;
		int srcRectX;
		int srcRectY;
		int numFrames;
		glm::vec2 scale;
		glm::vec2 transOffsetUp;
		glm::vec2 transOffsetDown;
		glm::vec2 transOffsetLeft;
		glm::vec2 transOffsetRight;
		glm::vec2 boxSizeUp;
		glm::vec2 boxSizeDown;
		glm::vec2 boxSizeLeft;
		glm::vec2 boxSizeRight;
		glm::vec2 upOffset;
		glm::vec2 downOffset;
		glm::vec2 leftOffset;
		glm::vec2 rightOffset;
		int duration;
		bool animation;
		bool vertical;

		ProjectileAttrib()
			: group("")
			, sprite_name("")
			, width(0)
			, height(0)
			, srcRectX(0)
			, srcRectY(0)
			, numFrames(0)
			, scale(glm::vec2(0))
			, transOffsetUp(glm::vec2(0))
			, transOffsetDown(glm::vec2(0))
			, transOffsetLeft(glm::vec2(0))
			, transOffsetRight(glm::vec2(0))
			, boxSizeUp(glm::vec2(0))
			, boxSizeDown(glm::vec2(0))
			, boxSizeRight(glm::vec2(0))
			, boxSizeLeft(glm::vec2(0))
			, upOffset(glm::vec2(0))
			, downOffset(glm::vec2(0))
			, rightOffset(glm::vec2(0))
			, leftOffset(glm::vec2(0))
			, duration(3000)
			, animation(false)
			, vertical(false)
		{
			
		};

		ProjectileAttrib(std::string group, std::string sprite_name, int width, int height, int src_rect_x, int src_rect_y, int num_frames, glm::vec2 scale,
			glm::vec2 trans_offset_up, glm::vec2 trans_offset_down, glm::vec2 trans_offset_right, glm::vec2 trans_offset_left, glm::vec2 box_size_up,
			glm::vec2 box_size_down, glm::vec2 box_size_left, glm::vec2 box_size_right, glm::vec2 box_up_offset, glm::vec2 box_down_offset,
			glm::vec2 box_right_offset, glm::vec2 box_left_offset, int duration = 3000, bool animation = false, bool vertical = false)
		{
			this->group = group;
			this->sprite_name = sprite_name;
			this->width = width;
			this->height = height;
			srcRectX = src_rect_x;
			srcRectY = src_rect_y;
			numFrames = num_frames;
			this->scale = scale;
			transOffsetUp = trans_offset_up;
			transOffsetDown = trans_offset_down;
			transOffsetLeft = trans_offset_left;
			transOffsetRight = trans_offset_right;
			boxSizeUp = box_size_up;
			boxSizeDown = box_size_down;
			boxSizeLeft = box_size_left;
			boxSizeRight = box_size_right;
			upOffset = box_up_offset;
			downOffset = box_down_offset;
			leftOffset = box_left_offset;
			rightOffset = box_right_offset;
			this->duration = duration;
			this->animation = animation;
			this->vertical = vertical;
		}

		// This is also for testing to see what the current projectile attributes are
		std::string ToString();

	};

	bool magicBeam;
	bool fullLife;
	bool boomerangReturned;

	std::map<std::string, ProjectileAttrib> projectileAttributeMap;

	void UseItem(ProjectileAttrib attrib);
	void UseSword();
	void UseMagicWand();
	void LoadMapAttributes(sol::state& lua, const std::string& fileName = "ProjectileAttributes");
	
	// Testing Functions --> Use these to quickly reload Lua Table Attributes for adjusting positions
	void ClearItemMap() 
	{
		if (projectileAttributeMap.size() > 0)
			projectileAttributeMap.clear();

		if (projectileAttributeMap.size() == 0)
			Logger::Log("Item Map Cleared");
	}
	void ReloadItemMap()
	{
		if (projectileAttributeMap.size() == 0)
		{
			LoadMapAttributes(Game::Instance().GetLuaState());
			Logger::Log("Item Map Reloaded!");
		}
		else
			Logger::Err("Please Empty the item Map");
	}

	void ItemUsed();
	void SwordUsed();

public:

	//GamePadSystem controller;

	ProjectileEmitterSystem();
	void SubscribeKeyToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitterSystem::OnKeyPressed);
	}
	void SubscribeBtnToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<GamePadButtonPressedEvent>(this, &ProjectileEmitterSystem::OnBtnPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event);
	void OnBtnPressed(GamePadButtonPressedEvent& event);
	void Update(Registry& registry);
	void EnemyProjectileUpdate(Entity& entity);
	void BossProjectileUpdate(Entity& entity);
	void UpdateBoomerang(Entity& entity);
};