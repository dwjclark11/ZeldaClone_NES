#pragma once
#include "Utilities/Timer.h"
#include <string>

struct CaptionComponent
{
	std::string caption{ "no_caption" };
	bool scrollable{ true }, started{ false }, finished{ false }, is_number{ false };
	int xPos{ 0 }, yPos{ 0 }, currentFrame{ 0 };
	Timer scrollTimer{};
};
