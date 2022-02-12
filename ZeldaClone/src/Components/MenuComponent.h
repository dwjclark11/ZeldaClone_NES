#pragma once

class MenuComponent
{
public:
	int slotPos;
	MenuComponent(int slotPos = 0) 
	{
		this->slotPos = slotPos;
	}
};