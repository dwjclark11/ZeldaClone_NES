#pragma once

enum RupeeType {YELLOW = 0, BLUE};

struct RupeeTypeComponent
{
	RupeeType type;

	RupeeTypeComponent(RupeeType type = YELLOW)
	{
		this->type = type;
	}
};