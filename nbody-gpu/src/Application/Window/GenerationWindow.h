#pragma once

#include <Application/Window/Window.h>
#include <Simulation/Simulation.h>
#include <Simulation/RandomGen.h>

#include <Application/Window/Components/DistributionComponent.h>

#include <memory>
#include <map>
#include <string>
#include <functional>

struct GenerationWindow : public Window
{
	GenerationWindow(std::unique_ptr<Simulation::Instance>* simulation = nullptr);

	std::unique_ptr<Simulation::Instance>* p_simulationPtr;

	enum GeneratorType { SOLAR_SYSTEM, ELLIPTICAL_GALAXY, SPIRAL_GALAXY };

protected:
	virtual void DrawWindowContents() override;

	GeneratorType m_currentGenerator;

	std::map<GeneratorType, std::string> m_generatorNames;

	float m_solarSystemSizeH = 100'000.f;
	float m_solarSystemSizeV = 10.f;

	float m_solarSystemStarMass = 10'000.f;

	float m_galaxySizeX = 1'000'000.f, m_galaxySizeZ = 1'000'000.f, m_galaxySizeY = 1'000.f;

	DistributionComponent m_massDist;
	DistributionComponent m_speedDist;

	bool m_clockwise = true;

	int m_numBodies = 150;

	void DrawGeneratorSolarSystem();
	void DrawGeneratorEllipticalGalaxy();
	void DrawGeneratorSpiralGalaxy();

	void DrawMassOptions();
	void DrawSpeedOptions();
	void DrawClockwiseCombo();

	void DrawNumBodiesOptions();

	void Generate();
};