#pragma once
#include "../../ECS/ECS.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/BoxColliderComponent.h"
#include "../../Components/TriggerBoxComponent.h"
#include "../../AssetManager/AssetManager.h"
#include "../../Components/SpriteComponent.h"
#include "../../Components/EditorComponent.h"
#include "../../Components/ProjectileEmitterComponent.h"
#include "../../Components/AnimationComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/HealthComponent.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <SDL_ttf.h>

#include "../../Editor/EditorFileLoader.h"

class MouseControlSystem : public System
{
public:
	enum class EditorEnemyType
	{
		NOT_SELECTED = 0,
		OCTOROK,
		MOBLIN,
		DARKNUT,
		LEEVER,
		TEKTITE,
		PEAHAT,
		ARMOS,
		GHINI,
		LYNEL,
		ZORA
	};

	static EditorEnemyType enemyType;

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

	static bool OverImGuiWindow;

	MouseControlSystem()
	{
		mapSize = 100;
		layer = 0;
		leftPressed = false;
		rightPressed = false;
		textOffsetX = -100;
		imageName = "";
		prevEnemyType = EditorEnemyType::OCTOROK;
	}

	~MouseControlSystem() {}

	void Update(const std::unique_ptr<AssetManager>& assets, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera)
	{
		if (createTile) CreateTile(assets, renderer, mouseBox, event, camera);
		if (createObstacles) CreateObstacles(assets, renderer, mouseBox, event, camera);
		if (createBoxCollider) CreateBoxCollider(assets, renderer, mouseBox, event, camera);
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

	// Are we creating Tiles?
	void CreateTile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	// Are we creating obstacles --> Tables/Box cases etc
	void CreateObstacles(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateBoxCollider(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateEnemy(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);

	void SetEnemyImage();





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

	std::string enemyFile = "";
	std::string enemy_Type = "";

	glm::vec2 mousePosGrid;
	glm::vec2 mousePosView;
	glm::vec2 mousePosScreen;
	std::vector<std::string> imageIDs;
	EditorFileLoader editor_loader;

	EditorEnemyType prevEnemyType;
};
