#pragma once



class RupeeTypeComponent
{
public:
	enum class RupeeType { YELLOW = 0, BLUE };
	RupeeType type;

	RupeeTypeComponent(RupeeType type = RupeeType::YELLOW)
	{
		this->type = type;
	}
};