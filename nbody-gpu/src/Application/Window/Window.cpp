#include "Window.h"

Window::Window(std::string name, bool show, ImGuiWindowFlags flags, ImVec2 size) : show(show), name(name), flags(flags), size(size) {}

void Window::Draw(ImVec2 position, ImGuiCond cond) {
	if (show) {
		ImGui::SetNextWindowPos(position, cond);
		ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);
		ImGui::Begin(name.c_str(), &show, flags);

		DrawWindowContents();

		size = ImGui::GetWindowSize();

		ImGui::End();
	}
}