#pragma once

#include <Application/Window/Window.h>

struct ViewSettingsWindow : public Window
{
	ViewSettingsWindow();

	float massScaleFactor = 0.01f;

	float unitToMetres = 1'000.f;

	bool showSpeedColors = true;
	bool showVelocityArrows = true;

protected:
	virtual void DrawWindowContents() override;
};