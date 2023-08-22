#pragma once
struct Button
{
	bool isDown{ false }, justReleased{ false }, justPressed{ false };
	void Update(bool pressed);
};