#include "Systems/MenuSystems/RenderMainMenuSystem.h"
#include "Components/TransformComponent.h"
#include "Components/MenuComponent.h"
#include "Components/SpriteComponent.h"
#include "AssetManager/AssetManager.h"

#include "States/MenuState.h"
#include "Utilities/GameData.h"

#include <SDL.h>

RenderMainMenuSystem::RenderMainMenuSystem()
	: m_GameData( GameData::GetInstance() )
{
	RequiredComponent<TransformComponent>();
	RequiredComponent<SpriteComponent>();
	RequiredComponent<MenuComponent>();
}

void RenderMainMenuSystem::Update( SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager )
{
	// Loop all entities that the system is interested in
	for ( const auto& entity : GetSystemEntities() )
	{
		// Do not draw the link avatar if there is no save file associated with it
		if ( entity.HasTag( "first_menu_slot" ) && m_GameData.GetPlayer1Name().size() == 0 )
			continue;
		if ( entity.HasTag( "second_menu_slot" ) && m_GameData.GetPlayer2Name().size() == 0 )
			continue;
		if ( entity.HasTag( "third_menu_slot" ) && m_GameData.GetPlayer3Name().size() == 0 )
			continue;

		const auto& transform = entity.GetComponent<TransformComponent>();
		const auto& sprite = entity.GetComponent<SpriteComponent>();

		// Set the src Rect of our original sprite texture
		SDL_Rect srcRect = sprite.srcRect;

		// Set the destination rect with the x, y position to be rendered
		SDL_Rect dstRect = { static_cast<int>( transform.position.x ),
							 static_cast<int>( transform.position.y ),
							 static_cast<int>( sprite.width * transform.scale.x ),
							 static_cast<int>( sprite.height * transform.scale.y ) };

		SDL_RenderCopyEx( renderer,
						  assetManager->GetTexture( sprite.assetID ),
						  &srcRect,
						  &dstRect,
						  transform.rotation,
						  NULL,		  // The rotation is done on the screnter of the sprite, width / 2, height / 2
						  sprite.flip // This is if we want to flip a sprite
		);
	}
}

void RenderMainMenuSystem::OnExit()
{
	for ( auto& entity : GetSystemEntities() )
	{
		entity.Kill();
	}
}
