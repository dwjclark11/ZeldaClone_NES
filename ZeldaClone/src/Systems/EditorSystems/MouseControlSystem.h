#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/EditorComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/HealthComponent.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <SDL_ttf.h>


class MouseControlSystem : public System
{
public:
	static int imageSrcX;
	static int imageSrcY;
	static int mouseRectX;
	static int mouseRectY;

	static int boxColliderWidth;
	static int boxColliderHeight;
	static int boxColliderOffsetX;
	static int boxColliderOffsetY;
	
	static int tileScaleX;
	static int tileScaleY;
	
	static int gridSize;

	static bool isCollision;
	static bool isTrigger;
	
	static bool createTile;
	static bool createObstacles;
	static bool createBoxCollider;
	static bool createEnemy;
	
	// Animation  attributes
	static bool animation;
	static bool animationLooped;
	static bool animationVerticalScroll;
	static int animationFrameRateSpeed;
	static int animationNumFrames;
	static int animationFrameOffset;
	
	// Projectile Emitter
	static bool projectile;
	static glm::vec2 projectileVelocity;
	static int projectileRepeatFrequency; 
	static int projectileDuration; 
	static int projectileHitPercentDamage; 
	static bool projectileIsFriendly;

	// Rigid Body
	static bool rigidBody;
	static glm::vec2 rigidBodyVelocity;

	static bool gridSnap;

	static std::string imageID;
	static int layer;
	static unsigned triggerNum;
	static TriggerType triggerType;
	static std::string triggerLevelNum;

	static int CanvasWidth;
	static int CanvasHeight;


	MouseControlSystem()
	{
		mapSize = 100;
		layer = 0;
		leftPressed = false;
		rightPressed = false;
		textOffsetX = -100;
		imageName = "";
	}

	~MouseControlSystem() {}

	void Update(const std::unique_ptr<AssetManager>& assets, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
	{
		if (createTile) CreateTile(assets, renderer, mouseBox, event, camera);
		if (createObstacles) CreateObstacles(assets, renderer, mouseBox, event, camera);
		if (createBoxCollider) CreateBoxCollider(assets, renderer, mouseBox, event, camera);
		if (createEnemy) CreateEnemy(assets, renderer, mouseBox, event, camera);
	}
	
	// Are we creating Tiles?
	void CreateTile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event,SDL_Rect& camera)
	{
		SDL_GetMouseState(&mousePosX, &mousePosY);

		mousePosX += camera.x;
		mousePosY += camera.y;
		mousePosScreen.x = mousePosX;
		mousePosScreen.y = mousePosY;
		mousePosGrid.x = static_cast<int>(mousePosScreen.x) * gridSize;
		mousePosGrid.y = static_cast<int>(mousePosScreen.y) * gridSize;

		if (mousePosScreen.x >= 0) mousePosGrid.x = (static_cast<int>(mousePosScreen.x)) / gridSize;
		if (mousePosScreen.y >= 0) mousePosGrid.y = (static_cast<int>(mousePosScreen.y)) / gridSize;

		mouseBox.x = (mousePosGrid.x * gridSize) - camera.x;
		mouseBox.y = (mousePosGrid.y * gridSize) - camera.y;

		SDL_Rect srcRect = {
			imageSrcX,
			imageSrcY,
			mouseRectX,
			mouseRectY
		};

		// Set the destination Rectangle with the x, y position to be rendererd;
		SDL_Rect dstRect = {
			mouseBox.x,
			mouseBox.y,
			mouseBox.w * mouseRectX * tileScaleX,
			mouseBox.h * mouseRectY * tileScaleY
		};

		SDL_RenderCopyEx(
			renderer,
			assetManager->GetTexture(imageID),
			&srcRect,
			&dstRect,
			0, // Passed as an angle in degrees
			NULL,
			SDL_FLIP_NONE
		);

		if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				Entity tile = Registry::Instance()->CreateEntity();
				tile.Group("tiles");
				tile.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
					glm::vec2(tileScaleX, tileScaleY), 0.0);
				tile.AddComponent<SpriteComponent>(imageID, mouseRectX, mouseRectY, MouseControlSystem::layer, false, imageSrcX, imageSrcY);
				tile.AddComponent<EditorComponent>();
				if (isCollision)
				{
					tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffsetX, boxColliderOffsetY));
				}
				else if (isTrigger)
				{
					tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffsetX, boxColliderOffsetY));
					tile.Group("trigger");
				}
				Logger::Log("Left Pressed");
				leftPressed = true;
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				for (auto entity : GetSystemEntities())
				{
					auto& transform = entity.GetComponent<TransformComponent>();

					if (entity.BelongsToGroup("tiles") && transform.position.x == (mousePosGrid.x * gridSize) 
						&& transform.position.y == (mousePosGrid.y * gridSize) && !rightPressed)
					{
						entity.Kill();
						rightPressed = true;
						Logger::Err("Tile with ID: " + std::to_string(entity.GetID()) + " has been removed");
					}
				}
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			leftPressed = false;
			rightPressed = false;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Color white = { 255,255,255,255 };

		// Set MouseBox to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderDrawRect(renderer, &mouseBox);
	}

	// Are we creating obstacles --> Tables/Box cases etc
	void CreateObstacles(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
	{
		SDL_GetMouseState(&mousePosX, &mousePosY);

		mousePosX += camera.x;
		mousePosY += camera.y;
		mousePosScreen.x = mousePosX;
		mousePosScreen.y = mousePosY;
		mousePosGrid.x = static_cast<int>(mousePosScreen.x) * gridSize;
		mousePosGrid.y = static_cast<int>(mousePosScreen.y) * gridSize;

		if (mousePosScreen.x >= 0) mousePosGrid.x = (static_cast<int>(mousePosScreen.x)) / gridSize;
		if (mousePosScreen.y >= 0) mousePosGrid.y = (static_cast<int>(mousePosScreen.y)) / gridSize;

		mouseBox.x = (mousePosGrid.x * gridSize) - camera.x;
		mouseBox.y = (mousePosGrid.y * gridSize) - camera.y;

		SDL_Rect srcRect = {
			imageSrcX,
			imageSrcY,
			mouseRectX,
			mouseRectY
		};

		// Set the destination Rectangle with the x, y position to be rendererd;
		SDL_Rect dstRect = {
			mouseBox.x,
			mouseBox.y,
			mouseBox.w * mouseRectX * tileScaleX,
			mouseBox.h * mouseRectY * tileScaleY
		};

		SDL_RenderCopyEx(
			renderer,
			assetManager->GetTexture(imageID),
			&srcRect,
			&dstRect,
			0, // Passed as an angle in degrees
			NULL,
			SDL_FLIP_NONE
		);

		if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				Entity tile = Registry::Instance()->CreateEntity();
				tile.Group("tiles"); // Why tiles?
				tile.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
					glm::vec2(tileScaleX, tileScaleY), 0.0);
				// TODO: Add some sort of layer clamp for the obstacles!
				tile.AddComponent<SpriteComponent>(imageID, mouseRectX, mouseRectY, MouseControlSystem::layer, false, imageSrcX, imageSrcY);
				tile.AddComponent<EditorComponent>();
				// Does the object have a box Collider?
				if (isCollision)
				{
					tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffsetX, boxColliderOffsetY));
				}
				else if (isTrigger)
				{
					tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffsetX, boxColliderOffsetY));
					tile.Group("trigger");
				}
				leftPressed = true;
			}
			// Remove the current Entity the mouse is hovering over
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				for (auto entity : GetSystemEntities())
				{
					auto& transform = entity.GetComponent<TransformComponent>();

					if (entity.BelongsToGroup("tiles") && transform.position.x == (mousePosGrid.x * gridSize)
						&& transform.position.y == (mousePosGrid.y * gridSize) && !rightPressed)
					{
						entity.Kill();
						rightPressed = true;
						Logger::Err("Tile with ID: " + std::to_string(entity.GetID()) + " has been removed");
					}
				}
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			leftPressed = false;
			rightPressed = false;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Color white = { 255,255,255,255 };


		// Set MouseBox to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderDrawRect(renderer, &mouseBox);
	}


	void CreateBoxCollider(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
	{
		SDL_GetMouseState(&mousePosX, &mousePosY);

		mousePosX += camera.x;
		mousePosY += camera.y;
		mousePosScreen.x = mousePosX;
		mousePosScreen.y = mousePosY;
		mousePosGrid.x = static_cast<int>(mousePosScreen.x) * gridSize;
		mousePosGrid.y = static_cast<int>(mousePosScreen.y) * gridSize;

		if (mousePosScreen.x >= 0) mousePosGrid.x = (static_cast<int>(mousePosScreen.x)) / gridSize;
		if (mousePosScreen.y >= 0) mousePosGrid.y = (static_cast<int>(mousePosScreen.y)) / gridSize;

		mouseBox.x = (mousePosGrid.x * gridSize) - camera.x;
		mouseBox.y = (mousePosGrid.y * gridSize) - camera.y;

		SDL_Rect srcRect = {
			imageSrcX,
			imageSrcY,
			mouseRectX,
			mouseRectY
		};

		// Set the destination Rectangle with the x, y position to be rendererd;
		SDL_Rect dstRect = {
			mouseBox.x,
			mouseBox.y,
			mouseBox.w * mouseRectX * tileScaleX,
			mouseBox.h * mouseRectY * tileScaleY
		};

		SDL_RenderCopyEx(
			renderer,
			assetManager->GetTexture(imageID),
			&srcRect,
			&dstRect,
			0, // Passed as an angle in degrees
			NULL,
			SDL_FLIP_NONE
		);

		if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				Entity tile = Registry::Instance()->CreateEntity();
				tile.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y), glm::vec2(tileScaleX, tileScaleY), 0.0);

				if (isCollision)
				{
					tile.Group("colliders");
					tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffsetX, boxColliderOffsetY));
					Logger::Log("Collider Created");
				}
				else if (isTrigger)
				{
					tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffsetX, boxColliderOffsetY));
					tile.AddComponent<TriggerBoxComponent>(triggerType, glm::vec2(0,0), glm::vec2(0,0), triggerLevelNum);
					tile.Group("trigger");
					Logger::Log("Trigger Created");
				}
				
				leftPressed = true;
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				for (auto entity : GetSystemEntities())
				{
					auto& transform = entity.GetComponent<TransformComponent>();

					if (entity.BelongsToGroup("colliders") && transform.position.x == (mousePosGrid.x * gridSize)
						&& transform.position.y == (mousePosGrid.y * gridSize) && !rightPressed)
					{
						entity.Kill();
						rightPressed = true;
						Logger::Err("Collider with ID: " + std::to_string(entity.GetID()) + " has been removed");
					}
					else if (entity.BelongsToGroup("trigger") && transform.position.x == (mousePosGrid.x * gridSize)
						&& transform.position.y == (mousePosGrid.y * gridSize) && !rightPressed)
					{
						entity.Kill();
						rightPressed = true;
						Logger::Err("trigger with ID: " + std::to_string(entity.GetID()) + " has been removed");
					}
				}
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			leftPressed = false;
			rightPressed = false;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Color white = { 255,255,255,255 };

		// Set MouseBox to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderDrawRect(renderer, &mouseBox);
	}
	
	void CreateEnemy(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
	{
		SDL_GetMouseState(&mousePosX, &mousePosY);

		mousePosX += camera.x;
		mousePosY += camera.y;
		mousePosScreen.x = mousePosX;
		mousePosScreen.y = mousePosY;
		mousePosGrid.x = static_cast<int>(mousePosScreen.x) * gridSize;
		mousePosGrid.y = static_cast<int>(mousePosScreen.y) * gridSize;

		if (mousePosScreen.x >= 0) mousePosGrid.x = (static_cast<int>(mousePosScreen.x)) / gridSize;
		if (mousePosScreen.y >= 0) mousePosGrid.y = (static_cast<int>(mousePosScreen.y)) / gridSize;

		mouseBox.x = (mousePosGrid.x * gridSize) - camera.x;
		mouseBox.y = (mousePosGrid.y * gridSize) - camera.y;

		SDL_Rect srcRect = {
			imageSrcX,
			imageSrcY,
			mouseRectX,
			mouseRectY
		};

		// Set the destination Rectangle with the x, y position to be rendererd;
		SDL_Rect dstRect = {
			mouseBox.x,
			mouseBox.y,
			mouseBox.w * mouseRectX * tileScaleX,
			mouseBox.h * mouseRectY * tileScaleY
		};

		SDL_RenderCopyEx(
			renderer,
			assetManager->GetTexture(imageID),
			&srcRect,
			&dstRect,
			0, // Passed as an angle in degrees
			NULL,
			SDL_FLIP_NONE
		);

		if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				Entity enemy = Registry::Instance()->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
					glm::vec2(tileScaleX, tileScaleY), 0.0);
				enemy.AddComponent<SpriteComponent>(imageID, mouseRectX, mouseRectY, MouseControlSystem::layer, false, imageSrcX, imageSrcY);
				enemy.AddComponent<EditorComponent>();
				enemy.AddComponent<HealthComponent>();
				
				if (isCollision)
					enemy.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffsetX, boxColliderOffsetY));
				//if (rigidBody)
					enemy.AddComponent<RigidBodyComponent>(rigidBodyVelocity);
				if (projectile)
					enemy.AddComponent<ProjectileEmitterComponent>();
				if (animation)
					enemy.AddComponent<AnimationComponent>(animationNumFrames, animationFrameRateSpeed, animationVerticalScroll, animationLooped, animationFrameOffset);
				
				Logger::Log("Left Pressed");
				leftPressed = true;
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				for (auto entity : GetSystemEntities())
				{
					auto& transform = entity.GetComponent<TransformComponent>();

					if (entity.BelongsToGroup("tiles") && transform.position.x == (mousePosGrid.x * gridSize)
						&& transform.position.y == (mousePosGrid.y * gridSize) && !rightPressed)
					{
						entity.Kill();
						rightPressed = true;
						Logger::Err("Tile with ID: " + std::to_string(entity.GetID()) + " has been removed");
					}
				}
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			leftPressed = false;
			rightPressed = false;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Color white = { 255,255,255,255 };

		// Set MouseBox to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderDrawRect(renderer, &mouseBox);
	}


	void SetImageName(std::string imageName)
	{
		this->imageName = imageName;
	}

	std::string& GetImageName()
	{
		return imageName;
	}

	void SetImageID(std::string& imageID)
	{
		this->imageIDs.push_back(imageID);
	}

	std::string& GetImageID()
	{
		return imageID;
	}

private:
	int mousePosX, mousePosY;
	//int gridSize;
	int mapSize;
	int textOffsetX;
	bool leftPressed;
	bool rightPressed;
	std::string imageName;

	glm::vec2 mousePosGrid;
	glm::vec2 mousePosView;
	glm::vec2 mousePosScreen;
	std::vector<std::string> imageIDs;
};

int MouseControlSystem::imageSrcX = 0;
int MouseControlSystem::imageSrcY = 0;
int MouseControlSystem::mouseRectX = 0;
int MouseControlSystem::mouseRectY = 0;
int MouseControlSystem::tileScaleX = 0;
int MouseControlSystem::tileScaleY = 0;
int MouseControlSystem::gridSize = 32;
int MouseControlSystem::boxColliderWidth = 0;
int MouseControlSystem::boxColliderHeight = 0;
int MouseControlSystem::boxColliderOffsetX = 0;
int MouseControlSystem::boxColliderOffsetY = 0;
int MouseControlSystem::layer = 0;

bool MouseControlSystem::isCollision = false;
bool MouseControlSystem::isTrigger = false;
bool MouseControlSystem::createTile = false;
bool MouseControlSystem::createObstacles = false;
bool MouseControlSystem::createBoxCollider = false;
bool MouseControlSystem::createEnemy = false;
bool MouseControlSystem::gridSnap = false;

// Animation attributes --> Creating enemies
bool MouseControlSystem::animation = false;
bool MouseControlSystem::animationLooped = false;
bool MouseControlSystem::animationVerticalScroll = false;
int MouseControlSystem::animationFrameRateSpeed = 0;
int MouseControlSystem::animationNumFrames = 0;
int MouseControlSystem::animationFrameOffset = 0;

// Projectile Emitter
bool MouseControlSystem::projectile = false;
glm::vec2 MouseControlSystem::projectileVelocity = glm::vec2(0, 0);
int MouseControlSystem::projectileRepeatFrequency = 0;
int MouseControlSystem::projectileDuration = 1000;
int MouseControlSystem::projectileHitPercentDamage = 10;
bool MouseControlSystem::projectileIsFriendly = false;

// Rigid Body
bool MouseControlSystem::rigidBody = false;
glm::vec2 MouseControlSystem::rigidBodyVelocity = glm::vec2(10, 10);


std::string MouseControlSystem::imageID = "";
TriggerType MouseControlSystem::triggerType = NO_TRIGGER;
unsigned MouseControlSystem::triggerNum = 0;
std::string MouseControlSystem::triggerLevelNum = "";

int MouseControlSystem::CanvasWidth = 6144;
int MouseControlSystem::CanvasHeight = 4224;