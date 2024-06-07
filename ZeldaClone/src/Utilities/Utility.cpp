#include "Utilities/Utility.h"
#include "ECS/ECS.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/HUDComponent.h"
#include "Components/GameComponent.h"
#include "Systems/SoundFXSystem.h"
#include "Game/Game.h"
#include "Components/MenuComponent.h"
#include "Utilities/Timer.h"
#include "States/State.h"
#include "States/GameStateMachine.h"

#include <cmath>

int GetValue( int num, int digit )
{
	auto n = powf( 10, digit );
	auto n1 = powf( 10, digit - 1 );

	return static_cast<int>( fmod( num, n ) / n1 );
}

void ConvertNumberParser( std::string group, int num, int power )
{
	// check for a value of zero
	if ( num <= 0 )
	{
		// Create a variable entity and have it equal the given tag
		auto entity = Registry::Instance().GetEntityByTag( group );
		entity.GetComponent<SpriteComponent>().srcRect.x = 0;
		entity.GetComponent<SpriteComponent>().srcRect.y = 0;
		return;
	}

	int digit{ ++power };
	int temp_num = GetValue( num, digit );

	// Create a variable entity and have it equal the given tag
	auto entity = Registry::Instance().GetEntityByTag( group );
	if ( temp_num >= 0 && temp_num <= 9 )
	{
		entity.GetComponent<SpriteComponent>().srcRect.x = 8 * temp_num;
		entity.GetComponent<SpriteComponent>().srcRect.y = 0;
	}
}

void ConvertNumber( int number, int x, int y )
{
	size_t digits = std::to_string( number ).size();

	int temp_num = number;
	int temp_num2 = number;
	int temp_num3 = number;

	// check for a value of zero
	if ( number <= 0 )
	{
		// Create a variable entity and have it equal the given tag
		auto entity = Registry::Instance().CreateEntity();
		entity.Group( "caption" );
		entity.AddComponent<TransformComponent>( glm::vec2( x, y ), glm::vec2( 4, 4 ) );
		entity.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "numbers",
															   .width = 8,
															   .height = 8,
															   .layer = 5,
															   .bIsFixed = false,
															   .srcRect = SDL_Rect{ 0, 0, 8, 8 } } );

		entity.AddComponent<GameComponent>();
	}
	else
	{
		if ( digits == 3 )
		{
			temp_num /= pow( 10, digits - 1 );

			auto entity = Registry::Instance().CreateEntity();
			entity.Group( "caption" );
			entity.AddComponent<TransformComponent>( glm::vec2( x, y ), glm::vec2( 4, 4 ) );
			entity.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "numbers",
																   .width = 8,
																   .height = 8,
																   .layer = 5,
																   .bIsFixed = false,
																   .srcRect = SDL_Rect{ 0, 0, 8, 8 } } );

			entity.AddComponent<GameComponent>();
			if ( temp_num >= 0 && temp_num <= 9 )
			{
				entity.GetComponent<SpriteComponent>().srcRect.x = 8 * temp_num;
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
			}

			temp_num2 /= pow( 10, digits - 2 );

			auto entity2 = Registry::Instance().CreateEntity();
			entity2.Group( "caption" );
			entity2.AddComponent<TransformComponent>( glm::vec2( x + ( 32 * digits - 1 ), y ), glm::vec2( 4, 4 ) );
			entity2.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "numbers",
																	.width = 8,
																	.height = 8,
																	.layer = 5,
																	.bIsFixed = false,
																	.srcRect = SDL_Rect{ 0, 0, 8, 8 } } );

			entity2.AddComponent<GameComponent>();
			if ( temp_num2 >= 0 && temp_num2 <= 9 )
			{
				entity2.GetComponent<SpriteComponent>().srcRect.x = 8 * temp_num2;
				entity2.GetComponent<SpriteComponent>().srcRect.y = 0;
			}

			temp_num3 /= pow( 10, digits - 0 );

			auto entity3 = Registry::Instance().CreateEntity();
			entity3.Group( "caption" );
			entity3.AddComponent<TransformComponent>( glm::vec2( x + ( 32 * digits ), y ), glm::vec2( 4, 4 ) );
			entity3.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "numbers",
																	.width = 8,
																	.height = 8,
																	.layer = 5,
																	.bIsFixed = false,
																	.srcRect = SDL_Rect{ 0, 0, 8, 8 } } );

			entity3.AddComponent<GameComponent>();
			if ( temp_num3 >= 0 && temp_num3 <= 9 )
			{
				entity3.GetComponent<SpriteComponent>().srcRect.x = 8 * temp_num3;
				entity3.GetComponent<SpriteComponent>().srcRect.y = 0;
			}
		}
	}
}

void ConvertNumber( const std::string& number, int x, int y )
{
	size_t digits = number.size();
	int num = atoi( number.c_str() );

	// check for a value of zero
	if ( num <= 0 )
	{
		// Create a variable entity and have it equal the given tag
		auto entity = Registry::Instance().CreateEntity();
		entity.Group( "caption" );
		entity.AddComponent<TransformComponent>( glm::vec2( x, y ), glm::vec2( 4, 4 ) );
		entity.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "numbers",
															   .width = 8,
															   .height = 8,
															   .layer = 5,
															   .bIsFixed = false,
															   .srcRect = SDL_Rect{ 0, 0, 8, 8 } } );

		entity.AddComponent<GameComponent>();
	}
	else
	{
		int scale = Game::Instance().GetGameScale();
		int space = ( 8 * scale );

		for ( int i = 0; i < digits; i++ )
		{
			int temp_num = (int)number[ i ];

			auto entity = Registry::Instance().CreateEntity();
			entity.Group( "caption" );
			entity.AddComponent<TransformComponent>( glm::vec2( x + ( space * i ), y ), glm::vec2( scale, scale ) );
			entity.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "numbers",
																   .width = 8,
																   .height = 8,
																   .layer = 5,
																   .bIsFixed = false,
																   .srcRect = SDL_Rect{ 0, 0, 8, 8 } } );
			entity.AddComponent<GameComponent>();

			if ( temp_num >= 48 && temp_num <= 57 )
			{
				entity.GetComponent<SpriteComponent>().srcRect.x = 8 * ( temp_num - 48 );
				entity.GetComponent<SpriteComponent>().srcRect.y = 0;
			}
		}
	}
}

void ConvertName( const std::string& name, int x, int y )
{
	int num = name.length();
	// Clamp Num at 7
	if ( num > 7 )
		num = 7;
	// Variable for spaces in between letters
	int space = 0;

	// Parse the string and set the srcRect position
	for ( int i = 0; i < num; i++ )
	{
		auto& GSM = Game::Instance().GetStateMachine();

		char letter = std::toupper( name[ i ] );

		Entity nameEnt = Registry::Instance().CreateEntity();

		nameEnt.Group( "caption" );
		nameEnt.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "caption_letters",
																.width = 16,
																.height = 16,
																.layer = 0,
																.bIsFixed = false,
																.srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

		nameEnt.AddComponent<TransformComponent>( glm::vec2( x, y ), glm::vec2( 1.5, 1.5 ), 0 );

		if ( GSM->GetCurrentState() == "MENU_STATE" )
			nameEnt.AddComponent<MenuComponent>();
		else if ( GSM->GetCurrentState() == "GAMESTATE" )
			nameEnt.AddComponent<GameComponent>();

		/*
			Created a new sprite that allows me to make the code cleaner
			Code may have to be adjusted if the sprite changes
		*/

		// check to see if the char is part of the upper alphabet
		if ( letter >= 65 && letter <= 97 )
		{
			nameEnt.GetComponent<SpriteComponent>().srcRect.x = 16 * ( letter - 65 );
			nameEnt.GetComponent<SpriteComponent>().srcRect.y = 0;
			nameEnt.GetComponent<TransformComponent>().position.x += space;
		}
		// Create a space between the letters
		space += 24;
	}
}

void ConvertLetter( const char letter, int x, int y )
{

	// TODO: Change the sprite sheet to accomodate !?*+-. characters -- Leave for now!
	if ( letter < 65 || letter > 90 )
		return;

	char upperCase = std::toupper( letter );
	Entity nameEnt = Registry::Instance().CreateEntity();

	nameEnt.Group( "caption" );
	nameEnt.AddComponent<SpriteComponent>( SpriteComponent{ .assetID = "caption_letters",
															.width = 16,
															.height = 16,
															.layer = 2,
															.bIsFixed = false,
															.srcRect = SDL_Rect{ 0, 0, 16, 16 } } );

	nameEnt.AddComponent<TransformComponent>( glm::vec2( x, y ), glm::vec2( 1.5, 1.5 ), 0 );

	auto& GSM = Game::Instance().GetStateMachine();
	if ( GSM->GetCurrentState() == "MENU_STATE" )
		nameEnt.AddComponent<MenuComponent>();
	else if ( GSM->GetCurrentState() == "GAMESTATE" )
		nameEnt.AddComponent<GameComponent>();

	/*
		Created a new sprite that allows me to make the code cleaner
		Code may have to be adjusted if the sprite changes
	*/

	// check to see if the char is part of the upper alphabet
	if ( upperCase >= 65 && upperCase <= 90 )
	{
		nameEnt.GetComponent<SpriteComponent>().srcRect.x = 16 * ( upperCase - 65 );
		nameEnt.GetComponent<SpriteComponent>().srcRect.y = 0;
		nameEnt.GetComponent<TransformComponent>().position.x;
	}
}

void ChangePlayerAttrib( const glm::vec2& velocity, const glm::vec2& shieldOffset, const glm::vec2& swordOffset,
						 const int& shieldWidth, const int& shieldHeight, const int& spriteWidth, const int& swordWidth,
						 const int& swordHeight )
{
	const auto& player = Registry::Instance().GetEntityByTag( "player" );
	auto& playerTransform = player.GetComponent<TransformComponent>();
	auto& playerCollider = player.GetComponent<BoxColliderComponent>();
	auto& playerRigidbody = player.GetComponent<RigidBodyComponent>();
	auto& playerSprite = player.GetComponent<SpriteComponent>();

	const auto& shield = Registry::Instance().GetEntityByTag( "the_shield" );
	auto& shieldTransform = shield.GetComponent<TransformComponent>();
	auto& shieldCollider = shield.GetComponent<BoxColliderComponent>();
	auto& shieldRigidbody = shield.GetComponent<RigidBodyComponent>();

	const auto& sword = Registry::Instance().GetEntityByTag( "the_sword" );
	auto& swordTransform = sword.GetComponent<TransformComponent>();
	auto& swordCollider = sword.GetComponent<BoxColliderComponent>();
	auto& swordRigidbody = sword.GetComponent<RigidBodyComponent>();

	playerRigidbody.velocity = velocity;
	playerSprite.srcRect.x = playerSprite.width * spriteWidth;

	shieldTransform.position = playerTransform.position;
	shieldCollider.height = shieldHeight;
	shieldCollider.width = shieldWidth;
	shieldCollider.offset = shieldOffset;
	shieldRigidbody = playerRigidbody;

	swordTransform.position = playerTransform.position;
	swordCollider.height = swordHeight;
	swordCollider.width = swordWidth;
	swordCollider.offset = swordOffset;
	swordRigidbody = playerRigidbody;
}
