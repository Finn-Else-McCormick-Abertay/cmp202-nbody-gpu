#include "SimulationSettingsWindow.h"

SimulationSettingsWindow::SimulationSettingsWindow(std::unique_ptr<Simulation::Instance>* simulation) : Window("Simulation Settings", false, 0, ImVec2(350.f, 240.f)), p_simulationPtr(simulation) {}

void SimulationSettingsWindow::DrawWindowContents() {
	if (!p_simulationPtr) { return; }

	auto& simulation = *p_simulationPtr;

	auto currentMode = simulation->Mode();
	if (ImGui::BeginCombo("Run Mode", currentMode == Simulation::Instance::RunMode::SEQUENTIAL ? "Sequential" : "Parallel")) {
		auto option = [&](Simulation::Instance::RunMode mode, const char* name) {
			if (ImGui::Selectable(name, currentMode == mode)) {
				simulation->SetMode(mode);
			}
			if (currentMode == mode) { ImGui::SetItemDefaultFocus(); }
		};

		option(Simulation::Instance::RunMode::SEQUENTIAL, "Sequential");
		option(Simulation::Instance::RunMode::PARALLEL, "Parallel");

		ImGui::EndCombo();
	}

	float softening = simulation->Softening();
	if (ImGui::InputFloat("Softening", &softening)) { simulation->SetSoftening(softening); }

	if (currentMode == Simulation::Instance::RunMode::PARALLEL) {
		ImGui::Text("Group Size");
		ImGui::Indent();

		auto [gravStep, intStep] = simulation->GetGroupSize();
		bool shouldSetSteps = false;
		shouldSetSteps |= ImGui::InputInt("Gravity Step", &gravStep);
		shouldSetSteps |= ImGui::InputInt("Integration Step", &intStep);

		if (shouldSetSteps) { simulation->SetGroupSize(gravStep, intStep); }

		ImGui::Unindent();
	}

	bool isProfiling = simulation->IsProfiling();
	if (ImGui::Checkbox("Profile", &isProfiling)) { simulation->SetIsProfiling(isProfiling); }

	if (ImGui::Button("Reset Profiler")) { simulation->ResetProfiler(); }

	if (ImGui::Button("Update Averages")) {
		auto& profiler = simulation->GetProfiler();
		m_timesCount = profiler.Count();
		m_medianTime = profiler.Median();
		m_meanTime = profiler.Mean();
	}

	if (m_timesCount > 0) {
		ImGui::Text("Tick duration averaged over %i steps:", m_timesCount);
		ImGui::Text("Median : %s", Duration(m_medianTime).AsFormattedString(TimeUnit::NANOSECOND, true).c_str());
		ImGui::Text("Mean : %s", Duration(m_meanTime).AsFormattedString(TimeUnit::NANOSECOND, true).c_str());
	}
}