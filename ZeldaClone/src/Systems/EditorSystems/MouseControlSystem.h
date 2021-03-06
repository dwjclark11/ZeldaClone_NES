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
	static SpriteComponent spriteComponent;
	static ProjectileEmitterComponent projectileEmitter;
	static AnimationComponent animationComponent;
	static TriggerBoxComponent triggerBox;
	static TransformComponent transformComponent;
	static BoxColliderComponent boxColliderComponent;

	static int mouseRectX;
	static int mouseRectY;
	static int gridSize;

	static bool isCollision;
	static bool isTrigger;

	static bool createTile;
	static bool createObstacles;
	static bool createBoxCollider;
	static bool createEnemy;
	static bool createTrigger;

	static bool secretSelected;
	static bool spriteSelected;

	// Animation  attributes
	static bool animation;
	
	// Projectile Emitter
	static bool projectile;


	// Rigid Body
	static bool rigidBody;
	static glm::vec2 rigidBodyVelocity;

	static bool gridSnap;
	static int imageWidth;
	static int imageHeight;
	
	// Secret Attributes
	static SecretComponent secretComponent;

	static int CanvasWidth;
	static int CanvasHeight;
	static bool OverImGuiWindow;

	MouseControlSystem();
	~MouseControlSystem() {}

	void Update(const std::unique_ptr<AssetManager>& assets, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);

	void CreateTile(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateObstacles(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateBoxCollider(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateTrigger(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);
	void CreateEnemy(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera);

	void SetEnemyImage();

	void MouseBox(const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox, SDL_Rect& camera, bool collider = false, bool trigger = false);

	inline void SetImageName(std::string imageName) { this->imageName = imageName; }
	inline std::string& GetImageName() { return imageName; }
	void SetImageID(std::string& imageID) { this->imageIDs.push_back(imageID); }
	std::string& GetImageID() { return spriteComponent.assetID; }
	
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
