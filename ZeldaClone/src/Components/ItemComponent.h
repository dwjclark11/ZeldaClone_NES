#pragma once

enum ItemCollectType
{
	BOMBS = 0,
	ARROWS,
	HEARTS
};

struct ItemComponent
{
	ItemCollectType type;

	ItemComponent(ItemCollectType type = BOMBS)
	{
		this->type = type;
	}
};