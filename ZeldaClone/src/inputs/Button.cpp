#include "Button.h"

void Button::Update(bool pressed)
{
	justPressed = !isDown && pressed;
	justReleased = isDown && !pressed;
	isDown = pressed;
}
