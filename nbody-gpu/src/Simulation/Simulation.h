#pragma once

#include <Simulation/SimulationWorld.h>
#include <memory>

class Simulation
{
public:
	Simulation(std::unique_ptr<SimulationWorld>&&, float timeStep);

	void Progress(int steps = 1);

	const SimulationWorld& World() const;
	float StepLength() const;

private:
	std::unique_ptr<SimulationWorld> m_world;
	float m_timeStep;
};