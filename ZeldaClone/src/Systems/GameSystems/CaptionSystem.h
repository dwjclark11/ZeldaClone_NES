#pragma once
#include "../../ECS/ECS.h"

class CaptionSystem : public System
{
private:
	Entity& AddCaption(const Entity& entity, int& spaceX, int& spaceY);
	void AddCaption(const Entity& entity, int& spaceX, int& spaceY, char letter);
	Entity& AddNumberCaption(const Entity& entity, int spaceX, int spaceY);
	int NextLine(std::string& str);

	const int LETTER_SPACE = 32;
	int mCurrentLetter;

public:
	CaptionSystem();
	void Update();
};