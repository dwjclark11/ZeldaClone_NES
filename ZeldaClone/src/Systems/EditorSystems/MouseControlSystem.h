#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TriggerBoxComponent.h"
#include "../AssetManager/AssetManager.h"
#include "../Components/SpriteComponent.h"
#include "../Components/EditorComponent.h"
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
	static bool gridSnap;
	static std::string imageID;
	static unsigned layer;
	static unsigned triggerNum;
	static TriggerType triggerType;
	static int triggerLevelNum;
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
	}

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

		// ====================================================================================================================
		// Create Text
		// ====================================================================================================================
		std::string collisionText = "Collision: " + std::to_string(isCollision);
		std::string layerText = "Layer: " + std::to_string(layer);
		std::string tilePositionText = "X: " + std::to_string(static_cast<int>(mouseBox.x + camera.x) / gridSize)
			+ ", Y: " + std::to_string(static_cast<int>(mouseBox.y + camera.y) / gridSize);

		SDL_Surface* surface1 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			collisionText.c_str(),
			white);

		SDL_Surface* surface2 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			layerText.c_str(),
			white);

		SDL_Surface* surface3 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			tilePositionText.c_str(),
			white);

		SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
		SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
		SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);

		// Free the surface
		SDL_FreeSurface(surface1);
		SDL_FreeSurface(surface2);
		SDL_FreeSurface(surface3);

		int labelWidth = 0;
		int labelHeight = 0;

		SDL_QueryTexture(texture1, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture2, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture3, NULL, NULL, &labelWidth, &labelHeight);

		// Check Where the offset needs to be
		if (mousePosX < 90) textOffsetX = 100;
		if (mousePosX > 150) textOffsetX = -100;

		SDL_Rect collisionTextRect = {
			static_cast<int>(mouseBox.x) + textOffsetX,
			static_cast<int>(mouseBox.y) + 15,
			labelWidth,
			labelHeight
		};

		SDL_Rect layerTextRext = {
		static_cast<int>(mouseBox.x) + textOffsetX,
		static_cast<int>(mouseBox.y) + 30,
		labelWidth,
		labelHeight
		};

		SDL_Rect positionTextRect = {
		static_cast<int>(mouseBox.x) + textOffsetX,
		static_cast<int>(mouseBox.y) - 0,
		labelWidth,
		labelHeight
		};

		// ====================================================================================================================
		// Render Everything
		// ====================================================================================================================
		SDL_RenderCopy(renderer, texture1, NULL, &collisionTextRect);
		SDL_RenderCopy(renderer, texture2, NULL, &layerTextRext);
		SDL_RenderCopy(renderer, texture3, NULL, &positionTextRect);

		SDL_DestroyTexture(texture1);
		SDL_DestroyTexture(texture2);
		SDL_DestroyTexture(texture3);

		// Set MouseBox to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderDrawRect(renderer, &mouseBox);
	}

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
				tile.Group("tiles");
				tile.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
					glm::vec2(tileScaleX, tileScaleY), 0.0);
				//tile.AddComponent<SpriteComponent>(imageID, mouseRectX, mouseRectY, MouseControlSystem::layer, false, imageSrcX, imageSrcY);

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

		// ====================================================================================================================
		// Create Text
		// ====================================================================================================================
		std::string collisionText = "Collision: " + std::to_string(isCollision);
		std::string layerText = "Layer: " + std::to_string(layer);
		std::string tilePositionText = "X: " + std::to_string(static_cast<int>(mouseBox.x + camera.x) / gridSize)
			+ ", Y: " + std::to_string(static_cast<int>(mouseBox.y + camera.y) / gridSize);

		SDL_Surface* surface1 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			collisionText.c_str(),
			white);

		SDL_Surface* surface2 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			layerText.c_str(),
			white);

		SDL_Surface* surface3 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			tilePositionText.c_str(),
			white);

		SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
		SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
		SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);

		// Free the surface
		SDL_FreeSurface(surface1);
		SDL_FreeSurface(surface2);
		SDL_FreeSurface(surface3);

		int labelWidth = 0;
		int labelHeight = 0;

		SDL_QueryTexture(texture1, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture2, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture3, NULL, NULL, &labelWidth, &labelHeight);

		// Check Where the offset needs to be
		if (mousePosX < 90) textOffsetX = 100;
		if (mousePosX > 150) textOffsetX = -100;

		SDL_Rect collisionTextRect = {
			static_cast<int>(mouseBox.x) + textOffsetX,
			static_cast<int>(mouseBox.y) + 15,
			labelWidth,
			labelHeight
		};

		SDL_Rect layerTextRext = {
		static_cast<int>(mouseBox.x) + textOffsetX,
		static_cast<int>(mouseBox.y) + 30,
		labelWidth,
		labelHeight
		};

		SDL_Rect positionTextRect = {
		static_cast<int>(mouseBox.x) + textOffsetX,
		static_cast<int>(mouseBox.y) - 0,
		labelWidth,
		labelHeight
		};

		// ====================================================================================================================
		// Render Everything
		// ====================================================================================================================
		SDL_RenderCopy(renderer, texture1, NULL, &collisionTextRect);
		SDL_RenderCopy(renderer, texture2, NULL, &layerTextRext);
		SDL_RenderCopy(renderer, texture3, NULL, &positionTextRect);

		SDL_DestroyTexture(texture1);
		SDL_DestroyTexture(texture2);
		SDL_DestroyTexture(texture3);

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
					tile.AddComponent<TriggerBoxComponent>(triggerType, glm::vec2(0,0), triggerLevelNum);
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

		// ====================================================================================================================
		// Create Text
		// ====================================================================================================================
		std::string collisionText = "Collision: " + std::to_string(isCollision);
		std::string triggerText = "Trigger: " + std::to_string(isTrigger);
		std::string trigType = "";
		std::string trigLevelNum = "Level Num: " + std::to_string(triggerLevelNum);
		std::string layerText = "Layer: " + std::to_string(layer);
		std::string tilePositionText = "X: " + std::to_string(static_cast<int>(mouseBox.x + camera.x) / gridSize)
			+ ", Y: " + std::to_string(static_cast<int>(mouseBox.y + camera.y) / gridSize);

		switch (triggerType)
		{
		case NO_TRIGGER:
			trigType = "NO_TRIGGER";
			break;

		case SECRET_AREA:
			trigType = "SECRET_AREA";
			break;

		case ENTER_DUNGEON:
			trigType = "ENTER_DUNGEON";
			break;

		case BURN_BUSHES:
			trigType = "BURN_BUSHES";
			break;

		case PUSH_ROCKS:
			trigType = "PUSH_ROCKS";
			break;

		case CAMERA_RIGHT:
			trigType = "CAMERA_RIGHT";
			break;

		case TRAP:
			trigType = "TRAP";
			break;

		case HIDDEN_SWITCH:
			trigType = "HIDDEN_SWITCH";
			break;

		case HIDDEN_OBJECT:
			trigType = "HIDDEN_OBJECT";
			break;
		}

		SDL_Surface* surface1 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			collisionText.c_str(),
			white);

		SDL_Surface* surface2 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			triggerText.c_str(),
			white);

		SDL_Surface* surface3 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			layerText.c_str(),
			white);
		SDL_Surface* surface4 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			tilePositionText.c_str(),
			white);

		SDL_Surface* surface5 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			trigType.c_str(),
			white);
		
		SDL_Surface* surface6 = TTF_RenderText_Blended(
			assetManager->GetFont("charriot-font-20"),
			trigLevelNum.c_str(),
			white);

		SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
		SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
		SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
		SDL_Texture* texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
		SDL_Texture* texture6 = SDL_CreateTextureFromSurface(renderer, surface6);
		SDL_Texture* texture5 = SDL_CreateTextureFromSurface(renderer, surface5);
	

		// Free the surface
		SDL_FreeSurface(surface1);
		SDL_FreeSurface(surface2);
		SDL_FreeSurface(surface3);
		SDL_FreeSurface(surface4);
		SDL_FreeSurface(surface5);
		SDL_FreeSurface(surface6);

		int labelWidth = 0;
		int labelHeight = 0;

		SDL_QueryTexture(texture1, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture2, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture3, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture4, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture6, NULL, NULL, &labelWidth, &labelHeight);
		SDL_QueryTexture(texture5, NULL, NULL, &labelWidth, &labelHeight);
	

		// Check Where the offset needs to be
		if (mousePosX < 90) textOffsetX = 100;
		if (mousePosX > 150) textOffsetX = -100;

		SDL_Rect collisionTextRect = {
			static_cast<int>(mouseBox.x) + textOffsetX,
			static_cast<int>(mouseBox.y) + 15,
			labelWidth,
			labelHeight
		};

		SDL_Rect triggerTextRect = {
			static_cast<int>(mouseBox.x) + textOffsetX,
			static_cast<int>(mouseBox.y) + 30,
			labelWidth,
			labelHeight
		};

		SDL_Rect trigTypeTextRect = {
			static_cast<int>(mouseBox.x) + textOffsetX,
			static_cast<int>(mouseBox.y) + 60,
			labelWidth,
			labelHeight
		};

		SDL_Rect trigLevelNumTextRect = {
			static_cast<int>(mouseBox.x) + textOffsetX,
			static_cast<int>(mouseBox.y) + 75,
			labelWidth,
			labelHeight
		};


		SDL_Rect layerTextRext = {
		static_cast<int>(mouseBox.x) + textOffsetX,
		static_cast<int>(mouseBox.y) + 45,
		labelWidth,
		labelHeight
		};

		SDL_Rect positionTextRect = {
		static_cast<int>(mouseBox.x) + textOffsetX,
		static_cast<int>(mouseBox.y) - 0,
		labelWidth,
		labelHeight
		};

		// ====================================================================================================================
		// Render Everything
		// ====================================================================================================================
		SDL_RenderCopy(renderer, texture1, NULL, &collisionTextRect);
		SDL_RenderCopy(renderer, texture2, NULL, &triggerTextRect);
		SDL_RenderCopy(renderer, texture3, NULL, &layerTextRext);
		SDL_RenderCopy(renderer, texture4, NULL, &positionTextRect);
		SDL_RenderCopy(renderer, texture5, NULL, &trigTypeTextRect);
		
		
		if(triggerType == SECRET_AREA || triggerType == ENTER_DUNGEON)
			SDL_RenderCopy(renderer, texture6, NULL, &trigLevelNumTextRect);

		SDL_DestroyTexture(texture1);
		SDL_DestroyTexture(texture2);
		SDL_DestroyTexture(texture3);
		SDL_DestroyTexture(texture4);
		SDL_DestroyTexture(texture5);
		SDL_DestroyTexture(texture6);

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
unsigned MouseControlSystem::layer = 0;
bool MouseControlSystem::isCollision = false;
bool MouseControlSystem::isTrigger = false;
bool MouseControlSystem::createTile = false;
bool MouseControlSystem::createObstacles = false;
bool MouseControlSystem::createBoxCollider = false;
bool MouseControlSystem::gridSnap = false;
std::string MouseControlSystem::imageID = "";
TriggerType MouseControlSystem::triggerType = NO_TRIGGER;
unsigned MouseControlSystem::triggerNum = 0;
int MouseControlSystem::triggerLevelNum = 0;