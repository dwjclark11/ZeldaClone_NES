#pragma once
enum class ItemCollectType
{
	BOMBS = 0,
	HEARTS,
	YELLOW_RUPEE,
	BLUE_RUPEE,
	KEYS,
	DEFAULT,
};

enum class SpecialItemType
{
	NOT_SPECIAL = 0,
	WOOD_SWORD,
	STEEL_SWORD,
	MAGIC_SWORD,
	FULL_HEART,
	RAFT,
	POWER_BRACLET,
	RED_CANDLE,
	WOOD_BOOMERANG,
	MAGIC_BOOMERANG,
	LADDER,
	ARROWS,
	MAGIC_ARROWS,
	WOOD_BOW,
	TRIFORCE_PIECE,
};

struct ItemComponent
{
	ItemCollectType type{ ItemCollectType::DEFAULT };
	SpecialItemType special{ SpecialItemType::NOT_SPECIAL };
	int price{ 0 };
	bool bCollected{ false };
};