#include "Utilities/GameData.h"
#include "Utilities/Utility.h"
#include "Logger/Logger.h"
#include "Game/Game.h"
#include "Systems/SoundFXSystem.h"

#include <SDL_mixer.h>

constexpr int MAX_RUPEES = 255;
constexpr int MAX_BOMBS = 15;
constexpr int MAX_KEYS = 15;
constexpr int MAX_TRIFORCE_PIECES = 9;

std::unique_ptr<GameData> GameData::m_pInstance = nullptr;

void GameData::ConvertHUDNumbers()
{
	if ( m_TotalRupees != m_TotalPrevRupees /*|| !game.GetPlayer()->GetPlayerCreated()*/ )
	{
		ConvertNumberParser( "rupee_hundreds", m_TotalRupees, 2 );
		ConvertNumberParser( "rupee_tens", m_TotalRupees, 1 );
		ConvertNumberParser( "rupee_ones", m_TotalRupees, 0 );
		m_TotalPrevRupees = m_TotalRupees;
	}

	if ( m_TotalKeys != m_TotalPrevKeys /* || !game.GetPlayer().GetPlayerCreated()*/ )
	{
		ConvertNumberParser( "keys_tens", m_TotalKeys, 1 );
		ConvertNumberParser( "keys_ones", m_TotalKeys, 0 );
		m_TotalPrevKeys = m_TotalKeys;
	}

	if ( m_TotalBombs != m_TotalPrevBombs /* || !game.GetPlayer().GetPlayerCreated()*/ )
	{
		ConvertNumberParser( "bombs_tens", m_TotalBombs, 1 );
		ConvertNumberParser( "bombs_ones", m_TotalBombs, 0 );
		m_TotalPrevBombs = m_TotalBombs;
	}
}

void GameData::RupeeScroll()
{
	if ( m_ScrollRupees <= 0 )
		return;

	// Check to see if the rupee scoll timer has started
	if ( !m_RupeeTimer.isStarted() )
		m_RupeeTimer.Start();

	if ( m_RupeeTimer.GetTicks() >= 50 * m_Index )
	{
		m_bRupeeScrolling = true;
		// if we are buying an item, decrement from total Rupees
		if ( m_bBuyItem )
		{
			if ( m_ScrollRupees > 0 && m_TotalRupees >= m_ScrollRupees )
				m_TotalRupees--;
		}
		else // If we collect rupees, increment total rupees the desired amount!
		{
			// Do not go over the max amount of rupees
			if ( m_ScrollRupees > 0 && m_TotalRupees < MAX_RUPEES )
				m_TotalRupees++;
		}
		// Play the collect rupee sound
		Mix_Volume( SoundChannel::RUPEE, 10 );
		m_Game.GetSoundPlayer().PlaySoundFX( "get_rupee", 0, SoundChannel::RUPEE );
		m_ScrollRupees--;

		// Check for money scroll completion
		if ( m_ScrollRupees <= 0 )
		{
			m_RupeeTimer.Stop();
			m_bBuyItem = false;
			m_Index = 0;
			m_ScrollRupees = 0;
			m_bRupeeScrolling = false;
		}
		else
		{
			m_Index++;
		}
	}
}

GameData::GameData()
	: m_mapGameItems{
		{GameItems::BOOMERANG, false}, {GameItems::MAGIC_BOOMERANG, false},
		{GameItems::BOMB, false}, {GameItems::CANDLE, false},
		{GameItems::MAGIC_CANDLE, false}, {GameItems::BOW, false},
		{GameItems::WOOD_ARROWS, false}, {GameItems::MAGIC_ARROWS, false},
		{GameItems::FOOD, false}, {GameItems::WHISTLE, false},
		{GameItems::MAGIC_ROD, false}, {GameItems::FLUTE, false},
		{GameItems::RAFT, false}, {GameItems::LADDER, false},
		{GameItems::BLUE_RING, false}, {GameItems::RED_RING, false},
		{GameItems::SHIELD, false}, {GameItems::MAGIC_SHIELD, false},
		{GameItems::MAP, false}, {GameItems::POWER_BRACLET, false},
		{GameItems::MASTER_KEY, false}, {GameItems::BLUE_POTION, false},
		{GameItems::RED_POTION, false}, {GameItems::WOOD_SWORD, false},
		{GameItems::SILVER_SWORD, false}, {GameItems::MAGIC_SWORD, false},
	}
	, m_eSelectedItem{ItemType::EMPTY}
	, m_GamePlayerNum{-1}, mLevelWidth{16384}, mLevelHeight{5472}
	, m_ScrollRupees{0}, m_TotalRupees{0}, m_TotalPrevRupees{0}, m_TotalBombs{0}
	, m_TotalPrevBombs{0}, m_TotalKeys{0}, m_TotalPrevKeys{0}, m_TotalTriforcePieces{0}, m_Index{0}
	, m_NumHearts{ 3 }, m_LoadedHearts{ 0 }
	, m_Game(Game::Instance()), m_RupeeTimer{}
	, m_bRupeeScrolling{false}, m_bBuyItem{false}, m_bGameLoaded{ false }
	, m_sPlayer1Name{""}, m_sPlayer2Name{""}, m_sPlayer3Name{""}
{
}

GameData& GameData::GetInstance()
{
	if ( !m_pInstance )
		m_pInstance.reset( new GameData() );

	return *m_pInstance;
}

const bool GameData::HasItem( GameItems item )
{
	auto gameItemItr = m_mapGameItems.find( item );
	if ( gameItemItr == m_mapGameItems.end() )
	{
		Logger::Err( "GameData -- Trying to access items that do not exist!" );
		return false;
	}

	return gameItemItr->second;
}

bool GameData::AddItem( GameItems item )
{
	if ( HasItem( item ) )
	{
		Logger::Err( "GameData -- Item already in inventory!" );
		return false;
	}

	m_mapGameItems[ item ] = true;

	return true;
}

bool GameData::RemoveItem( GameItems item )
{
	if ( !HasItem( item ) )
	{
		Logger::Err( "GameData -- Item already removed from inventory!" );
		return false;
	}

	m_mapGameItems[ item ] = false;

	return true;
}

const bool GameData::HasSword()
{
	if ( HasItem( GameItems::WOOD_SWORD ) || HasItem( GameItems::SILVER_SWORD ) || HasItem( GameItems::MAGIC_SWORD ) )
		return true;

	return false;
}

void GameData::AddGameSecrets( const std::string& locationID, bool found )
{
	if ( m_mapGameSecrets.find( locationID ) == m_mapGameSecrets.end() )
	{
		m_mapGameSecrets.emplace( locationID, found );
	}
}

bool GameData::HasSecret( const std::string& locationID )
{
	if ( m_mapGameSecrets.find( locationID ) != m_mapGameSecrets.end() )
		return true;

	// If the location is not in the map return false
	return false;
}

bool GameData::IsSecretFound( const std::string& locationID )
{
	auto gameSecretsItr = m_mapGameSecrets.find( locationID );
	if ( gameSecretsItr != m_mapGameSecrets.end() )
	{
		return gameSecretsItr->second;
	}

	return false;
}

void GameData::SetSecretFound( const std::string& locationID, bool found )
{
	auto gameSecretsItr = m_mapGameSecrets.find( locationID );
	if ( gameSecretsItr != m_mapGameSecrets.end() )
	{
		gameSecretsItr->second = found;
	}
}

void GameData::AddBombs( int num )
{
	m_TotalBombs += num;
	if ( m_TotalBombs >= MAX_BOMBS )
		m_TotalBombs = MAX_BOMBS;
}

void GameData::AddKeys( int num )
{
	m_TotalKeys += num;
	if ( m_TotalKeys >= MAX_KEYS )
		m_TotalKeys = MAX_KEYS;
}

void GameData::AddTriforcePieces( int num )
{
	m_TotalTriforcePieces += num;

	if ( m_TotalTriforcePieces >= MAX_TRIFORCE_PIECES )
		m_TotalTriforcePieces = MAX_TRIFORCE_PIECES;
}

bool GameData::BuyItem( int price )
{
	if ( m_TotalRupees < price )
		return false;

	m_ScrollRupees = price;
	m_bBuyItem = true;
	return true;
}

void GameData::UpdateGameData()
{
	RupeeScroll();
	ConvertHUDNumbers();
}