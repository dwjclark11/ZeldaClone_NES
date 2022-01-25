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
#include "../../Components/AIComponent.h"

#include <SDL.h>
#include <glm/glm.hpp>
#include <SDL_ttf.h>

#include "../../Editor/EditorFileLoader.h"

class MouseControlSystem : public System
{
public:

	static AIComponent::EnemyType enemyType;

	// Common Properties
	static int imageSrcX;
	static int imageSrcY;
	static int mouseRectX;
	static int mouseRectY;

	static int boxColliderWidth;
	static int boxColliderHeight;
	static glm::vec2 boxColliderOffset;
	static glm::vec2 Scale;

	static int gridSize;

	static bool isCollision;
	static bool isTrigger;

	static bool createTile;
	static bool createObstacles;
	static bool createBoxCollider;
	static bool createEnemy;
	static bool createTrigger;
	static bool secretSelected;

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

	static std::string levelMusic;
	static std::string assetFile;
	static std::string enemyNewFile;
	static std::string colliderFile;
	static std::string TileMapName;
	static std::string TileMapImageName;
	static std::string entityFileName;
	static std::string triggerFile;

	static glm::vec2 transportOffset;
	static glm::vec2 cameraOffset;
	static int imageWidth;
	static int imageHeight;

	// Secret Attributes
	static int newSpriteWidth;
	static int newSpriteHeight;
	static std::string locationID;
	static std::string newTriggerType;
	static std::string newSpriteAssetID;

	static int newSpriteSrcX;
	static int newSpriteSrcY;


	static int CanvasWidth;
	static int CanvasHeight;

	static bool OverImGuiWindow;

	MouseControlSystem();
	~MouseControlSystem() {}

	void Update(const std::unique_ptr<AssetManager>& assets, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);

	// Are we creating Tiles?
	void CreateTile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	// Are we creating obstacles --> Tables/Box cases etc
	void CreateObstacles(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateBoxCollider(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateTrigger(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateEnemy(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);

	void SetEnemyImage();

	void MouseBox(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Rect& camera, bool collider = false, bool trigger = false);

	inline void SetImageName(std::string imageName) { this->imageName = imageName; }
	inline std::string& GetImageName() { return imageName; }
	void SetImageID(std::string& imageID) { this->imageIDs.push_back(imageID); }
	std::string& GetImageID() { return imageID; }
	
private:
	int mousePosX, mousePosY;
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

	AIComponent::EnemyType prevEnemyType;
	int prevMouseX = 0;
	int prevMouseY = 0;

};
