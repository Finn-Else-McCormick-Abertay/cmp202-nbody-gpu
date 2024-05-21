#include "TimeWindow.h"

TimeWindow::TimeWindow(std::unique_ptr<Simulation::Instance>* simulation) : Window("Time Control", true, ImGuiWindowFlags_NoSavedSettings, ImVec2(280.f, 130.f)), p_simulationPtr(simulation) {}

void TimeWindow::DrawWindowContents() {
	if (!p_simulationPtr) { return; }
	auto& simulation = *p_simulationPtr;

	if (simulation) {
		if (!simulation->Started()) {
			if (ImGui::Button("Begin")) {
				//simulation = std::unique_ptr<Simulation::Instance>(new Simulation::Instance(*simulation, Duration(stepLengthNextSim)));
				simulation->SetStepLength(Duration(stepLengthNextSim));
				isPaused = false;
			}
			if (ImGui::InputFloat("Step length", &stepLengthNextSim, 1.f, 100.f, "%.3f seconds")) {
				stepLengthNextSim = std::max(stepLengthNextSim, 0.f);
			}
			auto stepDur = Duration(stepLengthNextSim);
			ImGui::Text("Step Length: %s", stepDur.AsFormattedString().c_str());
		}
		else {
			if (ImGui::Button(isPaused ? "Play" : "Pause")) { isPaused = !isPaused; }
			ImGui::Text("Step Length: %s", simulation->StepLength().AsFormattedString().c_str());
			float stepsPerFrameAsFloat = stepsPerFrame;
			if (ImGui::InputFloat("Multiplier", &stepsPerFrameAsFloat, 1.f, 5.f, "%gx")) {
				stepsPerFrame = static_cast<int>(std::max(stepsPerFrameAsFloat, 1.f));
			}
			Duration elapsedTime = simulation->StepsTaken() * simulation->StepLength();
			ImGui::Text("Elapsed Time: %s", elapsedTime.AsFormattedString(TimeUnit::SECOND, true).c_str());
		}
	}
}