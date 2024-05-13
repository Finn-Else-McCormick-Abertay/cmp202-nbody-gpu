#include "Simulation.h"

#include "GravityInteraction.h"
#include "Integration.h"

Simulation::Simulation(std::unique_ptr<SimulationWorld>&& world, float timeStep) : m_world(std::move(world)), m_timeStep(timeStep) {}

const SimulationWorld& Simulation::World() const { return *m_world; }
float Simulation::StepLength() const { return m_timeStep; }


void Simulation::Progress(int steps) {
	for (int i = 0; i < steps; ++i) {
		for (auto it = m_world->begin(); it != m_world->end(); ++it) {
			Body& body = *it;

			body = LeapfrogIntegrate(body, float3(0.1f, 0.f, 0.f), StepLength());
		}
	}
}