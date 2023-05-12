#pragma once

#include "../Utilities/Timer.h"
#include <string>
class CaptionComponent
{
public:
	std::string caption;
	bool scrollable, started, finished, is_number;
	int xPos, yPos, currentFrame;

	Timer scrollTimer;

	CaptionComponent(std::string caption = "no_caption", bool scrollable = true, int xPos = 0, int yPos = 0, bool is_number = false)
		: caption{caption}, scrollable{scrollable}, started{false}, finished{false}, is_number{is_number}
		, xPos{xPos}, yPos{yPos}, currentFrame{0}, scrollTimer{}
	{
		
	}
};