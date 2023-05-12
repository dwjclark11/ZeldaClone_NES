#pragma once
struct Button
{
	bool isDown;
	bool justReleased;
	bool justPressed;
	Button() : isDown(false), justReleased(false), justPressed(false)
	{}

	// @pressed - is the button pressed this frame
	void Update(bool pressed);
};