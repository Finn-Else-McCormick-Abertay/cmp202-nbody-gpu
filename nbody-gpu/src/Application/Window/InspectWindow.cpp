#include "InspectWindow.h"

InspectWindow::InspectWindow(std::unique_ptr<Simulation::Instance>* simulation) : Window("Inspect", false, 0, ImVec2(250.f, 130.f)), p_simulationPtr(simulation) {}

void InspectWindow::DrawWindowContents() {
	if (!p_simulationPtr) { return; }

	auto& simulation = *p_simulationPtr;
	if (simulation) {
		auto& world = simulation->World();
		ImGui::Text("World size: %i", (int)world.size());
		ImGui::InputInt("Body index", &selectedIndex);
		if (selectedIndex >= 0 && selectedIndex < world.size()) {
			auto& body = world.at(selectedIndex);
			ImGui::Text("Pos: %g, %g, %g", body.position.x, body.position.y, body.position.z);
			ImGui::Text("Vel: %g, %g, %g", body.velocity.x, body.velocity.y, body.velocity.z);
			ImGui::Text("Mass: %g", body.mass);
		}
	}
}