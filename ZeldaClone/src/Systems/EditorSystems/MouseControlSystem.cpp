#include "MouseControlSystem.h"
#include "../../Components/SecretComponent.h"

#include "../../Game/Game.h"

int MouseControlSystem::imageSrcX = 0;
int MouseControlSystem::imageSrcY = 0;
int MouseControlSystem::mouseRectX = 0;
int MouseControlSystem::mouseRectY = 0;
//int MouseControlSystem::ScaleX = 0;
//int MouseControlSystem::ScaleY = 0;
int MouseControlSystem::gridSize = 64;
int MouseControlSystem::boxColliderWidth = 0;
int MouseControlSystem::boxColliderHeight = 0;
//int MouseControlSystem::boxColliderOffsetX = 0;
//int MouseControlSystem::boxColliderOffsetY = 0;
int MouseControlSystem::layer = 0;

 glm::vec2 MouseControlSystem::boxColliderOffset = glm::vec2(0);
 glm::vec2 MouseControlSystem::Scale = glm::vec2(0);



bool MouseControlSystem::isCollision = false;
bool MouseControlSystem::isTrigger = false;
bool MouseControlSystem::createTile = false;
bool MouseControlSystem::createObstacles = false;
bool MouseControlSystem::createBoxCollider = false;
bool MouseControlSystem::createEnemy = false;
bool MouseControlSystem::createTrigger = false;

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

bool MouseControlSystem::secretSelected = false;
bool MouseControlSystem::spriteSelected = false;

std::string MouseControlSystem::imageID = "";

// Trigger Attributes

TriggerType MouseControlSystem::triggerType = NO_TRIGGER;
//unsigned MouseControlSystem::triggerNum = 0;

std::string MouseControlSystem::levelMusic = "stop";
std::string MouseControlSystem::assetFile = "no_file";
std::string MouseControlSystem::enemyNewFile = "no_file";
std::string MouseControlSystem::colliderFile = "no_file";
std::string MouseControlSystem::TileMapName = "no_file";
std::string MouseControlSystem::TileMapImageName = "no_file";
std::string MouseControlSystem::entityFileName = "no_file";
std::string MouseControlSystem::triggerFile = "no_file";
bool MouseControlSystem::triggerCollider = false;


int MouseControlSystem::newSpriteWidth = 0;
int MouseControlSystem::newSpriteHeight = 0;
int MouseControlSystem::newSpriteSrcX = 0;
int MouseControlSystem::newSpriteSrcY = 0;

std::string MouseControlSystem::locationID = "none";
std::string MouseControlSystem::newTriggerType = "none";
std::string MouseControlSystem::newSpriteAssetID = "none";

glm::vec2 MouseControlSystem::transportOffset = glm::vec2(0);
glm::vec2 MouseControlSystem::cameraOffset = glm::vec2(0);
int MouseControlSystem::imageWidth = 0;
int MouseControlSystem::imageHeight = 0;

int MouseControlSystem::CanvasWidth = 6144;
int MouseControlSystem::CanvasHeight = 4224;


bool MouseControlSystem::OverImGuiWindow = false;


AIComponent::EnemyType MouseControlSystem::enemyType = AIComponent::EnemyType::OCTOROK;

MouseControlSystem::MouseControlSystem()
{
	mapSize = 100;
	layer = 0;
	leftPressed = false;
	rightPressed = false;
	textOffsetX = -100;
	imageName = "";
	prevEnemyType = AIComponent::EnemyType::NO_TYPE;
}

void MouseControlSystem::Update(const std::unique_ptr<AssetManager>& assets, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
{
	if (createTile) CreateTile(assets, renderer, mouseBox, event, camera);
	if (createObstacles) CreateObstacles(assets, renderer, mouseBox, event, camera);
	if (createBoxCollider) CreateBoxCollider(assets, renderer, mouseBox, event, camera);
	if (createTrigger) CreateTrigger(assets, renderer, mouseBox, event, camera);
	if (createEnemy)
	{
		if (enemyType != prevEnemyType)
		{
			SetEnemyImage();
			prevEnemyType = enemyType;
		}

		CreateEnemy(assets, renderer, mouseBox, event, camera);
	}
}

void MouseControlSystem::CreateTile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
{
	MouseBox(assetManager, renderer, mouseBox, camera);

	// This is a test
	int posX = static_cast<int>(Game::Instance()->GetMouseBox().x + Game::Instance()->GetCamera().x) / MouseControlSystem::gridSize;
	int posY = static_cast<int>(Game::Instance()->GetMouseBox().y + Game::Instance()->GetCamera().y) / MouseControlSystem::gridSize;


	if (event.type == SDL_MOUSEBUTTONDOWN || leftPressed)
	{
		if (event.type == SDL_MOUSEBUTTONUP)
			leftPressed = false;

		if ((event.button.button == SDL_BUTTON_LEFT || leftPressed) && !OverImGuiWindow && (posX != prevMouseX || posY != prevMouseY))
		{
			//Logger::Log("pos x: " + std::to_string(posX) + "pos y: " + std::to_string(posY));
			//Logger::Log("prev x: " + std::to_string(prevMouseX) + "pos y: " + std::to_string(prevMouseY));

			Entity tile = Registry::Instance()->CreateEntity();
			tile.Group("tiles");
			tile.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
				glm::vec2(Scale.x, Scale.y), 0.0);
			tile.AddComponent<SpriteComponent>(imageID, mouseRectX, mouseRectY, MouseControlSystem::layer, false, imageSrcX, imageSrcY);
			tile.AddComponent<EditorComponent>();

			if (isCollision)
			{
				tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffset.x, boxColliderOffset.y));
			}

			Logger::Log("Left Pressed");
			leftPressed = true;
			prevMouseX = posX;
			prevMouseY = posY;
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
}

void MouseControlSystem::CreateObstacles(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
{
	MouseBox(assetManager, renderer, mouseBox, camera);

	if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
	{
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
		{
			Entity tile = Registry::Instance()->CreateEntity();
			tile.Group("tiles"); // Why tiles?
			tile.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
				glm::vec2(Scale.x, Scale.y), 0.0);
			// TODO: Add some sort of layer clamp for the obstacles!
			tile.AddComponent<SpriteComponent>(imageID, mouseRectX, mouseRectY, MouseControlSystem::layer, false, imageSrcX, imageSrcY);
			tile.AddComponent<EditorComponent>();
			// Does the object have a box Collider?
			if (isCollision)
			{
				tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffset.x, boxColliderOffset.y));
			}
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
}



void MouseControlSystem::CreateBoxCollider(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
{
	MouseBox(assetManager, renderer, mouseBox, camera, true, false);

	if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
	{
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
		{
			Entity tile = Registry::Instance()->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y), glm::vec2(Scale.x, Scale.y), 0.0);

			if (isCollision)
			{
				tile.Group("colliders");
				tile.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffset.x, boxColliderOffset.y));
				Logger::Log("Collider Created");
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
}

void MouseControlSystem::CreateTrigger(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
{
	// This shows the green Trigger box on the mouse pointer
	MouseBox(assetManager, renderer, mouseBox, camera, true, true);

	if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
	{
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
		{
			Entity newTrigger = Registry::Instance()->CreateEntity();
			newTrigger.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
				glm::vec2(Scale.x, Scale.y), 0.0);
			newTrigger.AddComponent<EditorComponent>();
			// TODO: Create a trigger function that reads the currently selected trigger attributes!
			if (isCollision)
			{
				newTrigger.AddComponent<BoxColliderComponent>(boxColliderWidth, boxColliderHeight, glm::vec2(boxColliderOffset.x, boxColliderOffset.y));
				
			}
			newTrigger.AddComponent<TriggerBoxComponent>(
					triggerType, 
					glm::vec2(transportOffset.x, transportOffset.y), 
					glm::vec2(cameraOffset.x, cameraOffset.y), 
					levelMusic, assetFile, enemyNewFile, colliderFile,
					TileMapName, TileMapImageName, entityFileName, 
					imageWidth, imageHeight, triggerFile, triggerCollider);

			if (secretSelected)
			{
				auto& startPos = newTrigger.GetComponent<TransformComponent>().position;
				newTrigger.Group("secret");
				newTrigger.AddComponent<SecretComponent>(
					locationID, newTriggerType, 
					newSpriteAssetID, newSpriteWidth, 
					newSpriteHeight, newSpriteSrcX, 
					newSpriteSrcY, startPos);
			}
			else
			{
				newTrigger.Group("trigger");
			}
			
			Logger::Log("CREATED Trigger");
			leftPressed = true;

		}
		if (event.button.button == SDL_BUTTON_RIGHT && !OverImGuiWindow)
		{
			for (auto entity : GetSystemEntities())
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				if (entity.BelongsToGroup("trigger") && transform.position.x == (mousePosGrid.x * gridSize)
					&& transform.position.y == (mousePosGrid.y * gridSize) && !rightPressed)
				{
					entity.Kill();
					rightPressed = true;
					Logger::Err("Trigger with ID: " + std::to_string(entity.GetID()) + " has been removed");
				}
			}
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		leftPressed = false;
		rightPressed = false;
	}
}

void MouseControlSystem::CreateEnemy(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
{
	// This shows the current selecter Enemy On the mouse pointer
	MouseBox(assetManager, renderer, mouseBox, camera);

	if (event.type == SDL_MOUSEBUTTONDOWN && !leftPressed)
	{
		if (event.button.button == SDL_BUTTON_LEFT && !OverImGuiWindow)
		{
			Entity enemy = Registry::Instance()->CreateEntity();
			enemy.Group("enemies");
			enemy.AddComponent<TransformComponent>(glm::vec2(mouseBox.x + camera.x, mouseBox.y + camera.y),
				glm::vec2(Scale.x, Scale.y), 0.0);
			enemy.AddComponent<EditorComponent>();

			editor_loader.CreateNewEnemy(Game::Instance()->GetLuaState(), enemyFile, enemy_Type, enemy);

			Logger::Log("CREATED Enemy");
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
}

void MouseControlSystem::SetEnemyImage()
{
	switch (enemyType)
	{
	case AIComponent::EnemyType::OCTOROK:
		enemyFile = "Octorok";
		enemy_Type = "octorok_red";
		break;
	case AIComponent::EnemyType::MOBLIN:
		enemyFile = "Moblin";
		enemy_Type = "moblin";
		break;
	case AIComponent::EnemyType::DARKNUT:
		enemyFile = "Darknut";
		enemy_Type = "darknut";
		break;
	case AIComponent::EnemyType::LEEVER:
		enemyFile = "Leever";
		enemy_Type = "leever";
		break;
	case AIComponent::EnemyType::TEKTITE:
		enemyFile = "Tektite";
		enemy_Type = "tektite";
		break;
	case AIComponent::EnemyType::PEAHAT:
		enemyFile = "Peahat";
		enemy_Type = "peahat";
		break;
	case AIComponent::EnemyType::ARMOS:
		enemyFile = "Armos";
		enemy_Type = "armos";
		break;
	case AIComponent::EnemyType::GHINI:
		enemyFile = "Ghini";
		enemy_Type = "ghini";
		break;
	case AIComponent::EnemyType::LYNEL:
		enemyFile = "Lynel";
		enemy_Type = "lynel";
		break;
	case AIComponent::EnemyType::ZORA:
		enemyFile = "Zora";
		enemy_Type = "zora";
		break;
	default:
		break;
	}

	// Call the load Enemy Attributes --> These attributes are kept in a lua table
	editor_loader.LoadEnemiesAttributes(Game::Instance()->GetLuaState(), enemyFile, enemy_Type);
}

void MouseControlSystem::MouseBox(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Rect& camera, bool collider, bool trigger)
{
	SDL_GetMouseState(&mousePosX, &mousePosY);

	mousePosX += camera.x;
	mousePosY += camera.y;
	mousePosScreen.x = mousePosX;
	mousePosScreen.y = mousePosY;

	// If Grid Snap is enabled, snap the tile/sprite to the next grid location
	if (gridSnap)
	{
		mousePosGrid.x = static_cast<int>(mousePosScreen.x) * gridSize;
		mousePosGrid.y = static_cast<int>(mousePosScreen.y) * gridSize;

		if (mousePosScreen.x >= 0) mousePosGrid.x = (static_cast<int>(mousePosScreen.x)) / gridSize;
		if (mousePosScreen.y >= 0) mousePosGrid.y = (static_cast<int>(mousePosScreen.y)) / gridSize;

		mouseBox.x = (mousePosGrid.x * gridSize) - camera.x;
		mouseBox.y = (mousePosGrid.y * gridSize) - camera.y;

	}
	else // Float the tile/sprite at the center of the mouse
	{
		mouseBox.x = mousePosScreen.x - camera.x - (mouseRectX * Scale.x) / 2; 
		mouseBox.y = mousePosScreen.y - camera.y - (mouseRectY * Scale.y) / 2; 
	}

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
		mouseBox.w * mouseRectX * Scale.x,
		mouseBox.h * mouseRectY * Scale.y
	};

	if (!collider)
	{
		SDL_RenderCopyEx(
			renderer,
			assetManager->GetTexture(imageID), // Should this not have a texture associated?
			&srcRect,
			&dstRect,
			0, // Passed as an angle in degrees
			NULL,
			SDL_FLIP_NONE
		);
	}
	else
	{
		if (!trigger)
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
		else
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);

		SDL_RenderFillRect(renderer, &dstRect);
		SDL_RenderDrawRect(renderer, &dstRect);
	}
}
