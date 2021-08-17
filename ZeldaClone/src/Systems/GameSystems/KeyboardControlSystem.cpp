#include "../GameSystems/KeyboardControlSystem.h"

directionE KeyboardControlSystem::dir = NONE;
bool KeyboardControlSystem::keyDown = false;

void KeyboardControlSystem::UpdatePlayer()
{
	if (!Game::Instance()->GetCameraMoving())
	{
		for (auto entity : GetSystemEntities())
		{
			if (entity.HasTag("the_sword"))
			{
				auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
				auto& box = entity.GetComponent<BoxColliderComponent>();
				rigidBody = glm::vec2(0);
				box.offset = glm::vec2(64, 60);
				box.height = 4;
				box.width = 4;
			}

			if (entity.HasTag("player"))
			{
				auto& playerBox = entity.GetComponent<BoxColliderComponent>(); 
				auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();
				
				// Reset velocity and box collider
				rigidBody = glm::vec2(0);
				playerBox.offset = glm::vec2(45, 35);

				if (dir == UP)
				{
					sprite.srcRect.x = sprite.width * 2;
					sprite.srcRect.y = sprite.height * 0;
				}
				else if (dir == RIGHT)
				{
					sprite.srcRect.x = sprite.width * 3;
					sprite.srcRect.y = sprite.height * 0;
				}
				else if (dir == DOWN)
				{
					sprite.srcRect.x = sprite.width * 0;
					sprite.srcRect.y = sprite.height * 0;
				}
				else if (dir == LEFT)
				{
					sprite.srcRect.x = sprite.width * 1;
					sprite.srcRect.y = sprite.height * 0;
				}
			}
			else if (entity.HasTag("the_shield"))
			{
				auto& shieldBox = entity.GetComponent<BoxColliderComponent>(); 
				auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();
				rigidBody = glm::vec2(0);

				if (dir == UP)
				{
					shieldBox.height = 2;
					shieldBox.width = 24;
					shieldBox.offset = glm::vec2(48, 32);
				}
				else if (dir == RIGHT)
				{
					shieldBox.height = 30;
					shieldBox.width = 2;
					shieldBox.offset = glm::vec2(90, 56);
				}
				else if (dir == DOWN)
				{
					shieldBox.height = 2;
					shieldBox.width = 24;
					shieldBox.offset = glm::vec2(40, 84);
				}
				else if (dir == LEFT)
				{
					shieldBox.height = 30;
					shieldBox.width = 2;
					shieldBox.offset = glm::vec2(30, 50);
				}
			}
		}
	}
}
