#include "GenerationWindow.h"

GenerationWindow::GenerationWindow(std::unique_ptr<Simulation::Instance>* simulation)
	: Window("World Generation", true, ImGuiWindowFlags_NoSavedSettings, ImVec2(410.f, 480.f)), p_simulationPtr(simulation) {

	m_currentGenerator = SOLAR_SYSTEM;

	m_generatorNames = {
		{ SOLAR_SYSTEM, "Solar System" },
		{ ELLIPTICAL_GALAXY, "Elliptical Galaxy" },
		{ SPIRAL_GALAXY, "Spiral Galaxy" },
	};

	m_massDist.Set<Random::normal>(Random::normal::param_type{ 1'000.f, 500.f });
	m_speedDist.Set<Random::normal>(Random::normal::param_type{ 100.f, 10.f });
}

void GenerationWindow::DrawWindowContents() {
	if (!p_simulationPtr) { return; }
	auto& simulation = *p_simulationPtr;

	if (simulation) {
		if (ImGui::BeginCombo("Generator Type", m_generatorNames.at(m_currentGenerator).c_str())) {
			for (auto& [id, name] : m_generatorNames) {
				bool isSelected = (id == m_currentGenerator);
				if (ImGui::Selectable(name.c_str(), isSelected)) {
					m_currentGenerator = id;
				}
				if (isSelected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}

		switch (m_currentGenerator) {
		case SOLAR_SYSTEM: DrawGeneratorSolarSystem(); break;
		case ELLIPTICAL_GALAXY: DrawGeneratorEllipticalGalaxy(); break;
		case SPIRAL_GALAXY: DrawGeneratorSpiralGalaxy(); break;
		}

		if (ImGui::Button("Generate")) { Generate(); }
	}
}

void GenerationWindow::DrawGeneratorSolarSystem() {

	DrawNumBodiesOptions();

	ImGui::InputFloat("Size", &m_solarSystemSizeH, 0.f, 0.f, "%.3fm");
	ImGui::InputFloat("Height", &m_solarSystemSizeV, 0.f, 0.f, "%.3fm");

	DrawSpeedOptions();

	DrawClockwiseCombo();

	DrawMassOptions();

	ImGui::InputFloat("Star Mass", &m_solarSystemStarMass, 0.f, 0.f, "%.3fkg");
}

void GenerationWindow::DrawGeneratorEllipticalGalaxy() {

	DrawNumBodiesOptions();

	float dim[3] = { m_galaxySizeX, m_galaxySizeY, m_galaxySizeZ };
	if (ImGui::InputFloat3("Dimensions", dim, "%.3fm")) {
		m_galaxySizeX = dim[0];
		m_galaxySizeY = dim[1];
		m_galaxySizeZ = dim[2];
	}

	DrawSpeedOptions();

	DrawClockwiseCombo();

	DrawMassOptions();
}

void GenerationWindow::DrawGeneratorSpiralGalaxy() {

	DrawNumBodiesOptions();

	DrawSpeedOptions();

	DrawClockwiseCombo();

	DrawMassOptions();
}

void GenerationWindow::DrawMassOptions() {
	ImGui::Text("Mass Distribution");
	ImGui::Indent();
	m_massDist.Draw();
	ImGui::Unindent();
}

void GenerationWindow::DrawSpeedOptions() {
	ImGui::Text("Speed Distribution");
	ImGui::Indent();
	m_speedDist.Draw();
	ImGui::Unindent();
}

void GenerationWindow::DrawClockwiseCombo() {
	if (ImGui::BeginCombo("Rotation direction", m_clockwise ? "Clockwise" : "Anticlockwise")) {
		auto option = [&](bool val, const char* name) {
			if (ImGui::Selectable(name, m_clockwise == val)) {
				m_clockwise = val;
			}
			if (m_clockwise == val) { ImGui::SetItemDefaultFocus(); }
			};

		option(true, "Clockwise");
		option(false, "Anticlockwise");

		ImGui::EndCombo();
	}
}

void GenerationWindow::DrawNumBodiesOptions() {
	if (ImGui::InputInt("Number of Bodies", &m_numBodies)) {
		m_numBodies = std::max(m_numBodies, 0);
	}
}


void GenerationWindow::Generate() {
	if (!p_simulationPtr) { return; }

	switch (m_currentGenerator) {
	case SOLAR_SYSTEM: {

		auto hDist = Random::normal(0.f, m_solarSystemSizeH);
		auto vDist = Random::normal(0.f, m_solarSystemSizeV);

		std::visit([&](auto massDist, auto speedDist) {
			*p_simulationPtr = std::make_unique<Simulation::Instance>(
				std::make_unique<Simulation::World>(
					Simulation::World::Combine(
						Simulation::World::CentralStar(m_solarSystemStarMass),
						Simulation::World::RandomWorld(m_numBodies,
							Random::MakeDiskGenerator(massDist, hDist, hDist, vDist, speedDist, m_clockwise)
						)
					)
				),
				(*p_simulationPtr)->StepLength()
			);
			}, m_massDist.GetVariant(), m_speedDist.GetVariant());
		
	} break;
	case ELLIPTICAL_GALAXY: {

	} break;
	case SPIRAL_GALAXY: {

	} break;
	}
}