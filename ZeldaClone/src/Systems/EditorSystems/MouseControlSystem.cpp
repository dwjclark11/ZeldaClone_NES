#include "Systems/EditorSystems/MouseControlSystem.h"
#include "Components/SecretComponent.h"
#include "Components/TransformComponent.h"

#include "Game/Game.h"
#include "Utilities/Camera.h"

MouseControlSystem::MouseControlSystem()
	: m_Registry{ Registry::Instance() }
	, m_EditorLoader{ *this }
{
}

void MouseControlSystem::Update( const std::unique_ptr<AssetManager>& assets, SDL_Renderer* renderer,
								 SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera )
{
	if ( m_bCreateTile )
		CreateTile( assets, renderer, mouseBox, event, camera );
	else if ( m_bCreateObstacles )
		CreateObstacles( assets, renderer, mouseBox, event, camera );
	else if ( m_bCreateBoxCollider )
		CreateBoxCollider( assets, renderer, mouseBox, event, camera );
	else if ( m_bCreateTrigger )
		CreateTrigger( assets, renderer, mouseBox, event, camera );
	else if ( m_bCreateEnemy )
	{
		if ( m_eEnemyType != m_ePrevEnemyType )
		{
			SetEnemyImage();
			m_ePrevEnemyType = m_eEnemyType;
		}

		CreateEnemy( assets, renderer, mouseBox, event, camera );
	}
}

void MouseControlSystem::CreateTile( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
									 SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera )
{
	MouseBox( assetManager, renderer, mouseBox, camera );

	if ( m_bOverImGuiWindow )
		return;

	// This is a test
	int posX = static_cast<int>( Game::Instance().GetMouseBox().x + Game::Instance().GetCamera().GetCameraPos().x ) /
			   m_GridSize;
	int posY = static_cast<int>( Game::Instance().GetMouseBox().y + Game::Instance().GetCamera().GetCameraPos().y ) /
			   m_GridSize;

	if ( event.type == SDL_MOUSEBUTTONDOWN || m_bLeftPressed )
	{
		if ( event.type == SDL_MOUSEBUTTONUP )
			m_bLeftPressed = false;

		if ( ( event.button.button == SDL_BUTTON_LEFT || m_bLeftPressed ) &&
			 ( posX != m_PrevMouseX || posY != m_PrevMouseY ) )
		{
			Entity tile = Registry::Instance().CreateEntity();
			tile.Group( "tiles" );
			tile.AddComponent<TransformComponent>( glm::vec2( mouseBox.x + camera.x, mouseBox.y + camera.y ),
												   m_TransformComponent.scale,
												   m_TransformComponent.rotation );
			tile.AddComponent<SpriteComponent>( SpriteComponent{
				.assetID = m_SpriteComponent.assetID,
				.width = m_MouseRectX,
				.height = m_MouseRectY,
				.layer = m_SpriteComponent.layer,
				.bIsFixed = m_SpriteComponent.bIsFixed,
				.srcRect = SDL_Rect{
					m_SpriteComponent.srcRect.x, m_SpriteComponent.srcRect.y, m_MouseRectX, m_MouseRectY } } );

			tile.AddComponent<EditorComponent>();

			if ( m_bIsCollision )
			{
				tile.AddComponent<BoxColliderComponent>( m_BoxColliderComponent );
			}

			Logger::Log( "Left Pressed" );
			m_bLeftPressed = true;
			m_PrevMouseX = posX;
			m_PrevMouseY = posY;
		}
		if ( event.button.button == SDL_BUTTON_RIGHT && !m_bOverImGuiWindow )
		{
			auto subX = ( m_MouseRectX * m_TransformComponent.scale.x ) / 2;
			auto subY = ( m_MouseRectY * m_TransformComponent.scale.y ) / 2;
			auto entities = m_Registry.GetEntitiesByGroup( "tiles" );

			for ( auto& entity : entities )
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				if ( transform.position.x <= ( m_MousePosX - subX + 15 ) &&
					 transform.position.x >= ( m_MousePosX - subX - 15 ) &&
					 transform.position.y <= ( m_MousePosY - subY + 15 ) &&
					 transform.position.y >= ( m_MousePosY - subY - 15 ) && !m_bRightPressed )
				{
					entity.Kill();
					m_bRightPressed = true;
					Logger::Err( "Tile with ID: " + std::to_string( entity.GetID() ) + " has been removed" );
				}
			}
		}
	}
	else if ( event.type == SDL_MOUSEBUTTONUP )
	{
		m_bLeftPressed = false;
		m_bRightPressed = false;
	}
}

void MouseControlSystem::CreateObstacles( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
										  SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera )
{
	MouseBox( assetManager, renderer, mouseBox, camera );

	if ( event.type == SDL_MOUSEBUTTONDOWN && !m_bLeftPressed )
	{
		if ( event.button.button == SDL_BUTTON_LEFT && !m_bOverImGuiWindow )
		{
			Entity tile = Registry::Instance().CreateEntity();
			tile.Group( "tiles" ); // Why tiles?
			tile.AddComponent<TransformComponent>( glm::vec2( mouseBox.x + camera.x, mouseBox.y + camera.y ),
												   m_TransformComponent.scale,
												   m_TransformComponent.rotation );

			tile.AddComponent<SpriteComponent>( SpriteComponent{
				.assetID = m_SpriteComponent.assetID,
				.width = m_MouseRectX,
				.height = m_MouseRectY,
				.layer = m_SpriteComponent.layer,
				.bIsFixed = m_SpriteComponent.bIsFixed,
				.srcRect = SDL_Rect{
					m_SpriteComponent.srcRect.x, m_SpriteComponent.srcRect.y, m_MouseRectX, m_MouseRectY } } );

			tile.AddComponent<EditorComponent>();
			// Does the object have a box Collider?
			if ( m_bIsCollision )
			{
				tile.AddComponent<BoxColliderComponent>( m_BoxColliderComponent );
			}
		}
		// Remove the current Entity the mouse is hovering over
		if ( event.button.button == SDL_BUTTON_RIGHT && !m_bOverImGuiWindow )
		{
			auto subX = ( m_MouseRectX * m_TransformComponent.scale.x ) / 2;
			auto subY = ( m_MouseRectY * m_TransformComponent.scale.y ) / 2;
			auto entities = m_Registry.GetEntitiesByGroup( "tiles" );
			for ( auto& entity : entities )
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				if ( transform.position.x <= ( m_MousePosX - subX + 15 ) &&
					 transform.position.x >= ( m_MousePosX - subX - 15 ) &&
					 transform.position.y <= ( m_MousePosY - subY + 15 ) &&
					 transform.position.y >= ( m_MousePosY - subY - 15 ) && !m_bRightPressed )
				{
					entity.Kill();
					m_bRightPressed = true;
					Logger::Err( "Tile with ID: " + std::to_string( entity.GetID() ) + " has been removed" );
				}
			}
		}
	}
	else if ( event.type == SDL_MOUSEBUTTONUP )
	{
		m_bLeftPressed = false;
		m_bRightPressed = false;
	}
}

void MouseControlSystem::CreateBoxCollider( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
											SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera )
{
	MouseBox( assetManager, renderer, mouseBox, camera, true, false );

	if ( event.type == SDL_MOUSEBUTTONDOWN && !m_bLeftPressed )
	{
		if ( event.button.button == SDL_BUTTON_LEFT && !m_bOverImGuiWindow )
		{
			Entity tile = Registry::Instance().CreateEntity();
			tile.AddComponent<TransformComponent>(
				glm::vec2( mouseBox.x + camera.x, mouseBox.y + camera.y ), m_TransformComponent.scale, 0.0 );

			if ( m_bIsCollision )
			{
				tile.Group( "colliders" );
				tile.AddComponent<BoxColliderComponent>(
					m_BoxColliderComponent.width, m_BoxColliderComponent.height, m_BoxColliderComponent.offset );
				Logger::Log( "Collider Created" );
			}

			m_bLeftPressed = true;
		}
		if ( event.button.button == SDL_BUTTON_RIGHT && !m_bOverImGuiWindow )
		{
			auto subX = ( m_MouseRectX * m_TransformComponent.scale.x ) / 2;
			auto subY = ( m_MouseRectY * m_TransformComponent.scale.y ) / 2;

			auto entities = m_Registry.GetEntitiesByGroup( "colliders" );
			for ( auto& entity : entities )
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				if ( transform.position.x <= ( m_MousePosX - subX + 15 ) &&
					 transform.position.x >= ( m_MousePosX - subX - 15 ) &&
					 transform.position.y <= ( m_MousePosY - subY + 15 ) &&
					 transform.position.y >= ( m_MousePosY - subY - 15 ) && !m_bRightPressed )
				{
					entity.Kill();
					m_bRightPressed = true;
					Logger::Err( "Collider with ID: " + std::to_string( entity.GetID() ) + " has been removed" );
				}
				else if ( entity.BelongsToGroup( "trigger" ) &&
						  transform.position.x == ( m_MousePosGrid.x * m_GridSize ) &&
						  transform.position.y == ( m_MousePosGrid.y * m_GridSize ) && !m_bRightPressed )
				{
					entity.Kill();
					m_bRightPressed = true;
					Logger::Err( "trigger with ID: " + std::to_string( entity.GetID() ) + " has been removed" );
				}
			}
		}
	}
	else if ( event.type == SDL_MOUSEBUTTONUP )
	{
		m_bLeftPressed = false;
		m_bRightPressed = false;
	}
}

void MouseControlSystem::CreateTrigger( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
										SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera )
{
	// This shows the green Trigger box on the mouse pointer
	MouseBox( assetManager, renderer, mouseBox, camera, false, true );

	if ( event.type == SDL_MOUSEBUTTONDOWN && !m_bLeftPressed )
	{
		if ( event.button.button == SDL_BUTTON_LEFT && !m_bOverImGuiWindow )
		{
			Entity newTrigger = Registry::Instance().CreateEntity();
			newTrigger.AddComponent<TransformComponent>( glm::vec2( mouseBox.x + camera.x, mouseBox.y + camera.y ),
														 m_TransformComponent.scale,
														 m_TransformComponent.rotation );
			newTrigger.AddComponent<EditorComponent>();
			// TODO: Create a trigger function that reads the currently selected trigger attributes!
			if ( m_bIsCollision )
			{
				newTrigger.AddComponent<BoxColliderComponent>(
					m_BoxColliderComponent.width, m_BoxColliderComponent.height, m_BoxColliderComponent.offset );
			}
			newTrigger.AddComponent<TriggerBoxComponent>();
			newTrigger.GetComponent<TriggerBoxComponent>() = m_TriggerBox;

			if ( m_bSecretSelected )
			{
				auto& startPos = newTrigger.GetComponent<TransformComponent>().position;
				newTrigger.Group( "secret" );

				newTrigger.AddComponent<SecretComponent>();
				auto& secret = newTrigger.GetComponent<SecretComponent>();
				secret = m_SecretComponent;
				secret.startPos = startPos;
			}
			else
			{
				newTrigger.Group( "trigger" );
			}

			if ( m_bSpriteSelected )
			{
				newTrigger.AddComponent<SpriteComponent>( SpriteComponent{
					.assetID = m_SpriteComponent.assetID,
					.width = m_MouseRectX,
					.height = m_MouseRectY,
					.layer = m_SpriteComponent.layer,
					.bIsFixed = m_SpriteComponent.bIsFixed,
					.srcRect = SDL_Rect{
						m_SpriteComponent.srcRect.x, m_SpriteComponent.srcRect.y, m_MouseRectX, m_MouseRectY } } );

				Logger::Log( "Sprite Asset ID: " + m_SpriteComponent.assetID +
							 "Sprite Width: " + std::to_string( m_SpriteComponent.width ) );
				Logger::Log( "Sprite Asset ID: " + newTrigger.GetComponent<SpriteComponent>().assetID +
							 "Sprite Width: " + std::to_string( newTrigger.GetComponent<SpriteComponent>().width ) );
			}

			Logger::Log( "CREATED Trigger" );
			m_bLeftPressed = true;
		}
		if ( event.button.button == SDL_BUTTON_RIGHT && !m_bOverImGuiWindow )
		{
			auto subX = ( m_MouseRectX * m_TransformComponent.scale.x ) / 2;
			auto subY = ( m_MouseRectY * m_TransformComponent.scale.y ) / 2;

			auto entities = m_Registry.GetEntitiesByGroup( "trigger" );
			for ( auto& entity : entities )
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				if ( transform.position.x <= ( m_MousePosX - subX + 15 ) &&
					 transform.position.x >= ( m_MousePosX - subX - 15 ) &&
					 transform.position.y <= ( m_MousePosY - subY + 15 ) &&
					 transform.position.y >= ( m_MousePosY - subY - 15 ) && !m_bRightPressed )
				{
					entity.Kill();
					m_bRightPressed = true;
					Logger::Err( "Trigger with ID: " + std::to_string( entity.GetID() ) + " has been removed" );
				}
			}
		}
	}
	else if ( event.type == SDL_MOUSEBUTTONUP )
	{
		m_bLeftPressed = false;
		m_bRightPressed = false;
	}
}

void MouseControlSystem::CreateEnemy( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
									  SDL_Rect& mouseBox, SDL_Event& event, SDL_Rect& camera )
{
	// This shows the current selecter Enemy On the mouse pointer
	MouseBox( assetManager, renderer, mouseBox, camera );

	if ( event.type == SDL_MOUSEBUTTONDOWN && !m_bLeftPressed )
	{
		if ( event.button.button == SDL_BUTTON_LEFT && !m_bOverImGuiWindow )
		{
			Entity enemy = Registry::Instance().CreateEntity();
			enemy.Group( "enemies" );
			enemy.AddComponent<TransformComponent>( glm::vec2( mouseBox.x + camera.x, mouseBox.y + camera.y ),
													m_TransformComponent.scale,
													m_TransformComponent.rotation );
			enemy.AddComponent<EditorComponent>();

			m_EditorLoader.CreateNewEnemy( Game::Instance().GetLuaState(), m_sEnemyFile, m_sEnemyType, enemy );

			Logger::Log( "CREATED Enemy" );
			m_bLeftPressed = true;
		}
		if ( event.button.button == SDL_BUTTON_RIGHT && !m_bOverImGuiWindow )
		{
			auto subX = ( m_MouseRectX * m_TransformComponent.scale.x ) / 2;
			auto subY = ( m_MouseRectY * m_TransformComponent.scale.y ) / 2;

			auto enemies = m_Registry.GetEntitiesByGroup( "enemies" );
			for ( auto& entity : enemies )
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				if ( transform.position.x <= ( m_MousePosX - subX + 15 ) &&
					 transform.position.x >= ( m_MousePosX - subX - 15 ) &&
					 transform.position.y <= ( m_MousePosY - subY + 15 ) &&
					 transform.position.y >= ( m_MousePosY - subY - 15 ) && !m_bRightPressed )
				{
					entity.Kill();
					m_bRightPressed = true;
					Logger::Err( "Enemy with ID: " + std::to_string( entity.GetID() ) + " has been removed" );
				}
			}
		}
	}
	else if ( event.type == SDL_MOUSEBUTTONUP )
	{
		m_bLeftPressed = false;
		m_bRightPressed = false;
	}
}

void MouseControlSystem::SetEnemyImage()
{
	switch ( m_eEnemyType )
	{
	case EnemyType::OCTOROK:
		m_sEnemyFile = "Octorok";
		m_sEnemyType = "octorok_red";
		break;
	case EnemyType::MOBLIN:
		m_sEnemyFile = "Moblin";
		m_sEnemyType = "moblin";
		break;
	case EnemyType::DARKNUT:
		m_sEnemyFile = "Darknut";
		m_sEnemyType = "darknut";
		break;
	case EnemyType::LEEVER:
		m_sEnemyFile = "Leever";
		m_sEnemyType = "leever";
		break;
	case EnemyType::TEKTITE:
		m_sEnemyFile = "Tektite";
		m_sEnemyType = "tektite";
		break;
	case EnemyType::PEAHAT:
		m_sEnemyFile = "Peahat";
		m_sEnemyType = "peahat";
		break;
	case EnemyType::ARMOS:
		m_sEnemyFile = "Armos";
		m_sEnemyType = "armos";
		break;
	case EnemyType::GHINI:
		m_sEnemyFile = "Ghini";
		m_sEnemyType = "ghini";
		break;
	case EnemyType::LYNEL:
		m_sEnemyFile = "Lynel";
		m_sEnemyType = "lynel";
		break;
	case EnemyType::ZORA:
		m_sEnemyFile = "Zora";
		m_sEnemyType = "zora";
		break;
	default: break;
	}

	// Call the load Enemy Attributes --> These attributes are kept in a lua table
	m_EditorLoader.LoadEnemiesAttributes( Game::Instance().GetLuaState(), m_sEnemyFile, m_sEnemyType );
}

void MouseControlSystem::SetCreateTile( bool bCreateTile )
{
	if ( bCreateTile )
	{
		m_bCreateObstacles = false;
		m_bCreateBoxCollider = false;
		m_bCreateEnemy = false;
		m_bCreateTrigger = false;
	}

	m_bCreateTile = bCreateTile;
}

void MouseControlSystem::SetCreateObstacle( bool bCreateObstacle )
{
	if ( bCreateObstacle )
	{
		m_bCreateTile = false;
		m_bCreateBoxCollider = false;
		m_bCreateEnemy = false;
		m_bCreateTrigger = false;
	}

	m_bCreateObstacles = bCreateObstacle;
}

void MouseControlSystem::SetCreateBoxCollider( bool bCreateCollider )
{
	if ( bCreateCollider )
	{
		m_bCreateTile = false;
		m_bCreateObstacles = false;
		m_bCreateEnemy = false;
		m_bCreateTrigger = false;
	}

	m_bCreateBoxCollider = bCreateCollider;
}

void MouseControlSystem::SetCreateTrigger( bool bCreatetrigger )
{
	if ( bCreatetrigger )
	{
		m_bCreateTile = false;
		m_bCreateObstacles = false;
		m_bCreateEnemy = false;
		m_bCreateBoxCollider = false;
	}

	m_bCreateTrigger = bCreatetrigger;
}

void MouseControlSystem::SetCreateEnemy( bool bCreateEnemy )
{
	if ( bCreateEnemy )
	{
		m_bCreateTile = false;
		m_bCreateObstacles = false;
		m_bCreateBoxCollider = false;
		m_bCreateTrigger = false;
	}

	m_bCreateEnemy = bCreateEnemy;
}

void MouseControlSystem::MouseBox( const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer,
								   SDL_Rect& mouseBox, SDL_Rect& camera, bool collider, bool trigger )
{
	SDL_GetMouseState( &m_MousePosX, &m_MousePosY );

	m_MousePosX += camera.x;
	m_MousePosY += camera.y;
	m_MousePosScreen.x = m_MousePosX;
	m_MousePosScreen.y = m_MousePosY;

	// If Grid Snap is enabled, snap the tile/sprite to the next grid location
	if ( m_bGridSnap )
	{
		m_MousePosGrid.x = static_cast<int>( m_MousePosScreen.x ) * m_GridSize;
		m_MousePosGrid.y = static_cast<int>( m_MousePosScreen.y ) * m_GridSize;

		if ( m_MousePosScreen.x >= 0 )
			m_MousePosGrid.x = ( static_cast<int>( m_MousePosScreen.x ) ) / m_GridSize;
		if ( m_MousePosScreen.y >= 0 )
			m_MousePosGrid.y = ( static_cast<int>( m_MousePosScreen.y ) ) / m_GridSize;

		mouseBox.x = ( m_MousePosGrid.x * m_GridSize ) - camera.x;
		mouseBox.y = ( m_MousePosGrid.y * m_GridSize ) - camera.y;
	}
	else // Float the tile/sprite at the center of the mouse
	{
		mouseBox.x = m_MousePosScreen.x - camera.x - ( m_MouseRectX * m_TransformComponent.scale.x ) / 2;
		mouseBox.y = m_MousePosScreen.y - camera.y - ( m_MouseRectY * m_TransformComponent.scale.y ) / 2;
	}

	SDL_Rect srcRect = { m_SpriteComponent.srcRect.x, m_SpriteComponent.srcRect.y, m_MouseRectX, m_MouseRectY };

	// Set the destination Rectangle with the x, y position to be rendererd;
	SDL_Rect dstRect = { mouseBox.x,
						 mouseBox.y,
						 mouseBox.w * m_MouseRectX * m_TransformComponent.scale.x,
						 mouseBox.h * m_MouseRectY * m_TransformComponent.scale.y };

	if ( !collider )
	{
		SDL_RenderCopyEx(
			renderer,
			assetManager->GetTexture( m_SpriteComponent.assetID ), // Should this not have a texture associated?
			&srcRect,
			&dstRect,
			0, // Passed as an angle in degrees
			NULL,
			SDL_FLIP_NONE );
	}
	else
	{
		if ( !trigger )
			SDL_SetRenderDrawColor( renderer, 255, 0, 0, 100 );
		else
			SDL_SetRenderDrawColor( renderer, 0, 255, 0, 125 );

		SDL_RenderFillRect( renderer, &dstRect );
		SDL_RenderDrawRect( renderer, &dstRect );
	}
}
