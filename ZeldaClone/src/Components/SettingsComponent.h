#pragma once

class SettingsComponent
{
public:
	enum class Input
	{
		KEY = 0,
		BUTTON
	};

	int index;
	SettingsComponent::Input input;

	SettingsComponent(int index = -1, SettingsComponent::Input input = SettingsComponent::Input::KEY)
	{
		this->index = index;
		this->input = input;
	}
};