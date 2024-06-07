#include "inputs/Button.h"

void Button::Update( bool pressed )
{
	bJustPressed = !bIsDown && pressed;
	bJustReleased = bIsDown && !pressed;
	bIsDown = pressed;
}
