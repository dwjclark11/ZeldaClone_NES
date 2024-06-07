#pragma once
struct Button
{
	bool bIsDown{ false }, bJustReleased{ false }, bJustPressed{ false };
	void Update( bool pressed );
};