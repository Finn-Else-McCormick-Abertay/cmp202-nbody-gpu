#include "SimulationSettingsWindow.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <imgui_stdlib.h>
#include <filesystem>

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

	if (ImGui::Button("Output Averages to File")) {
		auto& profiler = simulation->GetProfiler();

		nlohmann::json j;

#ifdef _DEBUG
		j["mode"] = "debug";
#endif
#ifdef NDEBUG
		j["mode"] = "release";
#endif

		nlohmann::json timesJson;

		for (auto& time : profiler.Times()) {
			timesJson.push_back(time.count());
		}

		j["median"] = profiler.Median().count();
		j["mean"] = profiler.Mean().count();
		j["times"] = timesJson;

		std::filesystem::path path = std::filesystem::path(m_fileName).replace_extension("json");

		std::ofstream file(path);
		if (file.is_open()) {
			file << j;
		}
	}

	ImGui::InputText("File Name", &m_fileName);
}