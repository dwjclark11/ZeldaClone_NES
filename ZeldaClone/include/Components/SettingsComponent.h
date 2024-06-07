#pragma once

enum class SettingsInput
{
	KEY = 0,
	BUTTON
};

struct SettingsComponent
{
	int index{ -1 };
	SettingsInput input{ SettingsInput::KEY };
};