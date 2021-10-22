#pragma once

enum ItemCollectType
{
	BOMBS = 0,
	ARROWS,
	HEARTS,
	DEFAULT
};

enum SpecialItemType
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
};

struct ItemComponent
{
	ItemCollectType type;
	SpecialItemType special;

	ItemComponent(ItemCollectType type = DEFAULT, SpecialItemType special = NOT_SPECIAL)
	{
		this->type = type;
		this->special = special;
	}
};