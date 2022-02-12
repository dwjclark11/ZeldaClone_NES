#pragma once

#include "../Utilities/Timer.h"
#include <string>
class CaptionComponent
{
public:
	std::string caption;
	bool scrollable;
	bool started;
	bool finished;
	int xPos;
	int yPos;
	bool numbers;
	int numFrames;
	int currentFrame;
	int ones;
	int tens;
	int hundreds;

	Timer scrollTimer;

	CaptionComponent(std::string caption = "no_caption", bool scrollable = true, bool numbers = false, int xPos = 0, int yPos = 0, int numFrames = 1, int ones = 0, int tens = 0, int hundreds = 0)
	{
		this->caption = caption;
		this->scrollable = scrollable;
		this->finished = false;
		this->started = false;
		this->xPos = xPos;
		this->yPos = yPos;
		this->numbers = numbers;
		this->numFrames = numFrames;
		this->currentFrame = 0;
		this->ones = ones;
		this->tens = tens;
		this->hundreds = hundreds;
	}
};