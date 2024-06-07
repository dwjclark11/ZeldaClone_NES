#pragma once
#include <memory>
#include <unordered_map>
#include "Utilities/Timer.h"
#include "Components/TriggerBoxComponent.h"
#include "Components/SceneChangeComponent.h"

enum SoundChannel
{
	ANY = -1,
	LOW_HEALTH = 0,
	RUPEE = 1,
	TEXT = 2,
	COLLECT = 3,
};

class GameData
{
  public:
	enum class GameItems
	{
		EMPTY = 0,
		BOOMERANG,
		MAGIC_BOOMERANG,
		BOMB,
		CANDLE,
		MAGIC_CANDLE,
		BOW,
		WOOD_ARROWS,
		MAGIC_ARROWS,
		FOOD,
		WHISTLE,
		MAGIC_ROD,
		FLUTE,
		RAFT,
		LADDER,
		BLUE_RING,
		RED_RING,
		SHIELD,
		MAGIC_SHIELD,
		MAP,
		POWER_BRACLET,
		MASTER_KEY,
		BLUE_POTION,
		RED_POTION,
		WOOD_SWORD,
		SILVER_SWORD,
		MAGIC_SWORD
	};
	enum class ItemType
	{
		EMPTY = 0,
		BOOMERANG,
		BOMB,
		POTION_BLUE,
		POTION_RED,
		CANDLE,
		BOW,
		FOOD,
		WHISTLE,
		MAGIC_ROD
	};

  private:
	std::unordered_map<GameItems, bool> m_mapGameItems;
	std::unordered_map<std::string, bool> m_mapGameSecrets;
	ItemType m_eSelectedItem;
	int m_GamePlayerNum, mLevelWidth, mLevelHeight;
	int m_ScrollRupees, m_TotalRupees, m_TotalPrevRupees, m_TotalBombs;
	int m_TotalPrevBombs, m_TotalKeys, m_TotalPrevKeys, m_TotalTriforcePieces, m_Index;
	int m_NumHearts, m_LoadedHearts;
	class Game& m_Game;
	Timer m_RupeeTimer;
	bool m_bRupeeScrolling, m_bBuyItem, m_bGameLoaded;
	TriggerBoxComponent m_TriggerBoxComponent;
	SceneChangeComponent m_SceneChangeComponent;

	std::string m_sPlayer1Name, m_sPlayer2Name, m_sPlayer3Name;

	GameData();
	static std::unique_ptr<GameData> m_pInstance;

	void ConvertHUDNumbers();
	void RupeeScroll();

  public:
	static GameData& GetInstance();
	const bool HasItem( GameItems item );
	bool AddItem( GameItems item );
	bool RemoveItem( GameItems item );
	const bool HasSword();
	const bool RupeeScrollActive() const { return m_bRupeeScrolling; }
	void AddGameSecrets( const std::string& locationID, bool found );
	bool HasSecret( const std::string& locationID );
	bool IsSecretFound( const std::string& locationID );
	void SetSecretFound( const std::string& locationID, bool found );
	void AddRupees( int num ) { m_ScrollRupees += num; }
	void AddBombs( int num );
	void AddKeys( int num );
	void AddTriforcePieces( int num );
	void UpdateGameData();
	bool BuyItem( int price );
	const int PlayerNum() const { return m_GamePlayerNum; }
	void SetPlayerNum( int num ) { m_GamePlayerNum = num; }
	void SetSelectedItem( ItemType item ) { m_eSelectedItem = item; }
	const ItemType GetSelectedItem() const { return m_eSelectedItem; }
	std::unordered_map<std::string, bool>& GetGameSecrets() { return m_mapGameSecrets; }

	inline void SetLevelSize( int level_width, int level_height )
	{
		mLevelWidth = level_width;
		mLevelHeight = level_height;
	}

	inline const int GetLevelWidth() const { return mLevelWidth; }
	inline const int GetLevelHeight() const { return mLevelHeight; }
	const int GetTotalRupees() const { return m_TotalRupees; }
	const int GetTotalKeys() const { return m_TotalKeys; }
	const int GetTotalBombs() const { return m_TotalBombs; }
	const int GetTotalTriforce() const { return m_TotalTriforcePieces; }
	void UseKey()
	{
		if ( m_TotalKeys > 0 )
			m_TotalKeys--;
	}
	void UseBomb()
	{
		if ( m_TotalBombs > 0 )
			m_TotalBombs--;
	}
	void UseArrow()
	{
		if ( m_TotalRupees > 0 )
			m_TotalRupees--;
	}

	void SetPlayer1Name( const std::string& name ) { m_sPlayer1Name = name; }
	void SetPlayer2Name( const std::string& name ) { m_sPlayer2Name = name; }
	void SetPlayer3Name( const std::string& name ) { m_sPlayer3Name = name; }

	const std::string& GetPlayer1Name() { return m_sPlayer1Name; }
	const std::string& GetPlayer2Name() { return m_sPlayer2Name; }
	const std::string& GetPlayer3Name() { return m_sPlayer3Name; }

	inline void SetScengeChangeValues( const SceneChangeComponent& scene ) { m_SceneChangeComponent = scene; }
	inline void SetTriggerBoxValues( const TriggerBoxComponent& trigger_box ) { m_TriggerBoxComponent = trigger_box; }
	inline const SceneChangeComponent& GetScene() { return m_SceneChangeComponent; }
	inline const TriggerBoxComponent& GetTrigger() { return m_TriggerBoxComponent; }

	inline const bool IsGameLoaded() const { return m_bGameLoaded; }
	inline void SetGameLoaded( bool bGameLoaded ) { m_bGameLoaded = bGameLoaded; }

	inline const int GetNumHearts() const { return m_NumHearts; }
	inline void SetNumHearts( int numHearts ) { m_NumHearts = numHearts; }
	inline void IncrementNumHearts() { m_NumHearts++; }

	inline const int GetLoadedHearts() const { return m_LoadedHearts; }
	inline void SetLoadedHearts( int loadedHearts ) { m_LoadedHearts = loadedHearts; }
	inline void IncrementLoadedHearts() { m_LoadedHearts++; }
	inline void DecrementLoadedHearts() { m_LoadedHearts--; }
};