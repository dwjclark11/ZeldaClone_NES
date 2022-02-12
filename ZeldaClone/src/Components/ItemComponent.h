#pragma once



class ItemComponent
{
public:
	enum class ItemCollectType
	{
		BOMBS = 0,
		HEARTS,
		DEFAULT,
		YELLOW_RUPEE,
		BLUE_RUPEE,
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
	};

	ItemCollectType type;
	SpecialItemType special;
	int price;

	ItemComponent(ItemCollectType type = ItemCollectType::DEFAULT, SpecialItemType special = SpecialItemType::NOT_SPECIAL, int price = 0)
	{
		this->type = type;
		this->special = special;
		this->price = price;
	}
};