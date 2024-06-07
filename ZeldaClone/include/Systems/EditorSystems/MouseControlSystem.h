#pragma once
#include "Components/TransformComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TriggerBoxComponent.h"
#include "AssetManager/AssetManager.h"
#include "Components/SpriteComponent.h"
#include "Components/EditorComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/HealthComponent.h"
#include "Components/AIComponent.h"
#include "Components/SecretComponent.h"
#include "Editor/EditorFileLoader.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>

class MouseControlSystem
{
  public:
	MouseControlSystem();
	~MouseControlSystem() = default;

	void Update( const std::unique_ptr<AssetManager>& assets, SDL_Renderer* renderer, SDL_Rect& mouseBox,
				 SDL_Event& event, SDL_Rect& camera );

	void CreateTile( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox,
					 SDL_Event& event, SDL_Rect& camera );
	void CreateObstacles( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox,
						  SDL_Event& event, SDL_Rect& camera );
	void CreateBoxCollider( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
							SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera );
	void CreateTrigger( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox,
						SDL_Event& event, SDL_Rect& camera );
	void CreateEnemy( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox,
					  SDL_Event& event, SDL_Rect& camera );
	void SetEnemyImage();

	void SetCreateTile( bool bCreateTile );
	void SetCreateObstacle( bool bCreateObstacle );
	void SetCreateBoxCollider( bool bCreateCollider );
	void SetCreateTrigger( bool bCreatetrigger );
	void SetCreateEnemy( bool bCreateEnemy );

	inline void SetGridSnap( bool bGridSnap ) { m_bGridSnap = bGridSnap; }
	inline void SetOverImGui( bool bOverGui ) { m_bOverImGuiWindow = bOverGui; }
	inline void SetCollider( bool bCollider ) { m_bIsCollision = bCollider; }
	inline void SetSpriteAssetID( const std::string& sAssetID ) { m_SpriteComponent.assetID = sAssetID; }
	inline void SetEnemyType( EnemyType enemyType ) { m_eEnemyType = enemyType; }

	inline const bool IsCreatingTiles() const { return m_bCreateTile; }
	inline const bool IsCreatingObstacles() const { return m_bCreateObstacles; }
	inline const bool IsCreatingColliders() const { return m_bCreateBoxCollider; }
	inline const bool IsCreatingTriggers() const { return m_bCreateTrigger; }
	inline const bool IsCreatingEnemies() const { return m_bCreateEnemy; }

	inline void SetBoxCollider( const BoxColliderComponent& boxCollider ) { m_BoxColliderComponent = boxCollider; }
	inline void SetTransformScale( float x, float y ) { m_TransformComponent.scale = glm::vec2{ x, y }; }
	inline void SetSpriteLayer( int layer ) { m_SpriteComponent.layer = layer; }
	inline void SetMouseRect( int x, int y )
	{
		m_MouseRectX = x;
		m_MouseRectY = y;
	}

	inline void SetSpriteSrcRect( int x, int y )
	{
		m_SpriteComponent.srcRect.x = x;
		m_SpriteComponent.srcRect.y = y;
	}
	inline const SpriteComponent& GetSpriteComponent() const { return m_SpriteComponent; }
	void MouseBox( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, SDL_Rect& mouseBox,
				   SDL_Rect& camera, bool collider = false, bool trigger = false );

	inline void SetImageName( const std::string& imageName ) { m_sImageName = imageName; }
	inline const std::string& GetImageName() const { return m_sImageName; }
	inline void SetImageID( std::string& imageID ) { m_ImageIDs.push_back( imageID ); }
	inline const std::string& GetImageID() const { return m_SpriteComponent.assetID; }
	inline const int GridSize() const { return m_GridSize; }
	inline const int GetMouseRectX() { return m_MouseRectX; }
	inline const int GetMouseRectY() { return m_MouseRectY; }

	inline const int GetCanvasWidth() const { return m_CanvasWidth; }
	inline const int GetCanvasHeight() const { return m_CanvasHeight; }

  private:
	class Registry& m_Registry;
	EditorFileLoader m_EditorLoader;
	std::vector<std::string> m_ImageIDs;

	EnemyType m_eEnemyType{ EnemyType::NO_TYPE };
	SpriteComponent m_SpriteComponent{};
	ProjectileEmitterComponent m_ProjectileEmitter{};
	AnimationComponent m_AnimationComponent{};
	TriggerBoxComponent m_TriggerBox{};
	TransformComponent m_TransformComponent{};
	BoxColliderComponent m_BoxColliderComponent{};
	SecretComponent m_SecretComponent{};

	bool m_bIsCollision{ false }, m_bIsTrigger{ false };
	bool m_bCreateTile{ false }, m_bCreateObstacles{ false };
	bool m_bCreateBoxCollider{ false }, m_bCreateEnemy{ false }, m_bCreateTrigger{ false };
	bool m_bSecretSelected{ false }, m_bSpriteSelected{ false };
	bool m_bAnimation{ false }, m_bProjectile{ false }, m_bRigidBody{ false };
	bool m_bGridSnap{ false }, m_bOverImGuiWindow{ false };
	bool m_bLeftPressed{ false }, m_bRightPressed{ false };

	int m_MouseRectX{ 0 }, m_MouseRectY{ 0 }, m_GridSize{ 64 };
	int m_ImageWidth{ 0 }, m_ImageHeight{ 0 };
	int m_MousePosX{ 0 }, m_MousePosY{ 0 };
	int m_PrevMouseX{ 0 }, m_PrevMouseY{ 0 };
	int m_MapSize{ 100 }, m_CanvasWidth{ 6144 }, m_CanvasHeight{ 4224 };
	int m_TextOffsetX{ 0 };

	std::string m_sImageName{ "" }, m_sEnemyFile{ "" }, m_sEnemyType{ "" };

	glm::vec2 m_MousePosGrid{ 0.f };
	glm::vec2 m_MousePosView{ 0.f };
	glm::vec2 m_MousePosScreen{ 0.f };
	glm::vec2 m_RigidBodyVelocity{ 0.f };

	EnemyType m_ePrevEnemyType{ EnemyType::NO_TYPE };
};
