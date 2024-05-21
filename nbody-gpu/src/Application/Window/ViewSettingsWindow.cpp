#include "ViewSettingsWindow.h"

ViewSettingsWindow::ViewSettingsWindow() : Window("View Settings", false, ImGuiWindowFlags_NoSavedSettings, ImVec2(340.f, 130.f)) {}


void ViewSettingsWindow::DrawWindowContents() {

	const char* format = (massScaleFactor == 0.f) ? "Constant Size" : "%.3f";
	if (ImGui::DragFloat("Mass Scale", &massScaleFactor, 0.001f, 0.f, 1.f, format)) { massScaleFactor = std::max(massScaleFactor, 0.f); }

	ImGui::InputFloat("Unit in Metres", &unitToMetres);

	ImGui::Checkbox("Display Speed Colors", &showSpeedColors);

	ImGui::Checkbox("Display Velocity Arrows", &showVelocityArrows);

}