#pragma once

#include <imgui.h>
#include <string>

struct Window
{
	bool show;
	std::string name;
	ImGuiWindowFlags flags;
	ImVec2 size;

	void Draw(ImVec2 position, ImGuiCond = ImGuiCond_Appearing);

protected:
	Window(std::string name, bool show = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None, ImVec2 size = ImVec2());
	virtual void DrawWindowContents() = 0;
};