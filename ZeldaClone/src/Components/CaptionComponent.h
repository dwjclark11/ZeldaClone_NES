#pragma once

#include "../Utilities/Timer.h"
#include <string>
struct CaptionComponent
{
	std::string caption;
	//bool scrollable;
	bool started;
	bool finished;
	int xPos;
	int yPos;
	//bool firstRow;
	int numFrames;
	int currentFrame;
	Timer scrollTimer;

	CaptionComponent(std::string caption = "no_caption", /*bool scrollable = false*/int xPos = 0, int yPos = 0, int numFrames = 1, bool firstRow = true)
	{
		this->caption = caption;
		//this->scrollable = scrollable;
		this->finished = false;
		this->started = false;
		this->xPos = xPos;
		this->yPos = yPos;
		//this->firstRow = firstRow;
		this->numFrames = numFrames;
		this->currentFrame = 0;
	}
};