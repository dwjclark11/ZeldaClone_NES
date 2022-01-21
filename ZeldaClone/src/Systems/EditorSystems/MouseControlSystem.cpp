#include "MouseControlSystem.h"
#include "../../Game/Game.h"

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


bool MouseControlSystem::OverImGuiWindow = false;


MouseControlSystem::EditorEnemyType MouseControlSystem::enemyType = MouseControlSystem::EditorEnemyType::NOT_SELECTED;

void MouseControlSystem::CreateTile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
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
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
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
		if (event.button.button == SDL_BUTTON_RIGHT && !OverImGuiWindow)
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

void MouseControlSystem::CreateObstacles(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
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
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
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
		if (event.button.button == SDL_BUTTON_RIGHT && !OverImGuiWindow)
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



void MouseControlSystem::CreateBoxCollider(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
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
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
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
				tile.AddComponent<TriggerBoxComponent>(triggerType, glm::vec2(0, 0), glm::vec2(0, 0), triggerLevelNum);
				tile.Group("trigger");
				Logger::Log("Trigger Created");
			}

			leftPressed = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT && !OverImGuiWindow)
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

void MouseControlSystem::CreateEnemy(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
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
	//SetEnemyImage();
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
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
		{
			Entity enemy = Registry::Instance()->CreateEntity();
			enemy.Group("enemies");
			enemy.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
				glm::vec2(tileScaleX, tileScaleY), 0.0);
			enemy.AddComponent<EditorComponent>();

			editor_loader.CreateNewEnemy(Game::Instance()->GetLuaState(), enemyFile, enemy_Type, enemy);

			Logger::Log("Left Pressed");
			leftPressed = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT && !OverImGuiWindow)
		{
			for (auto entity : GetSystemEntities())
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				if (entity.BelongsToGroup("enemies") && transform.position.x == (mousePosGrid.x * gridSize)
					&& transform.position.y == (mousePosGrid.y * gridSize) && !rightPressed)
				{
					entity.Kill();
					rightPressed = true;
					Logger::Err("Enemy with ID: " + std::to_string(entity.GetID()) + " has been removed");
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

void MouseControlSystem::SetEnemyImage()
{
	switch (enemyType)
	{
	case EditorEnemyType::OCTOROK:
		enemyFile = "Octorok";
		enemy_Type = "octorok_red";
		break;
	case EditorEnemyType::MOBLIN:
		enemyFile = "Moblin";
		enemy_Type = "moblin";
		break;
	case EditorEnemyType::DARKNUT:
		enemyFile = "Darknut";
		enemy_Type = "darknut";
		break;
	case EditorEnemyType::LEEVER:
		enemyFile = "Leever";
		enemy_Type = "leever";
		break;
	case EditorEnemyType::TEKTITE:
		enemyFile = "Tektite";
		enemy_Type = "tektite";
		break;
	case EditorEnemyType::PEAHAT:
		enemyFile = "Peahat";
		enemy_Type = "peahat";
		break;
	case EditorEnemyType::ARMOS:
		enemyFile = "Armos";
		enemy_Type = "armos";
		break;
	case EditorEnemyType::GHINI:
		enemyFile = "Ghini";
		enemy_Type = "ghini";
		break;
	case EditorEnemyType::LYNEL:
		enemyFile = "Lynel";
		enemy_Type = "lynel";
		break;
	case EditorEnemyType::ZORA:
		enemyFile = "Zora";
		enemy_Type = "zora";
		break;
	default:
		break;
	}

	// Call the load Enemy Attributes --> These attributes are kept in a lua table
	editor_loader.LoadEnemiesAttributes(Game::Instance()->GetLuaState(), enemyFile, enemy_Type);
}
