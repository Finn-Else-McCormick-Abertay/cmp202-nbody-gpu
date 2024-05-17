#include "Simulation.h"

#include "GravityInteraction.h"
#include "Integration.h"

using namespace Simulation;

Simulation::Instance::Instance(std::unique_ptr<SimWorld>&& world, Duration timeStep) : m_world(std::move(world)), m_timeStep(timeStep) {}

Simulation::Instance::Instance(Instance& other) : m_world(std::move(other.m_world)), m_timeStep(std::move(other.m_timeStep)) {}
Simulation::Instance::Instance(Instance& other, Duration timeStep) : Instance(other) { m_timeStep = timeStep; }

const World& Simulation::Instance::World() const { return *m_world; }
Duration Simulation::Instance::StepLength() const { return m_timeStep; }
int Simulation::Instance::StepsTaken() const { return m_stepsTaken; }
bool Simulation::Instance::Started() const { return m_started; }


void Simulation::Instance::Progress(int steps) {
	for (int i = 0; i < steps; ++i) {
		PerformStep();
	}
}

void Simulation::Instance::PerformStep() {
	++m_stepsTaken; m_started = true;
	std::vector<float3> bodyAccelerations; bodyAccelerations.reserve(m_world->size());
	for (int i = 0; i < m_world->size(); ++i) {
		float3 rootAccel = float3();
		auto& rootBody = m_world->at(i);
		for (int j = 0; j < m_world->size(); ++j) {
			if (i == j) { continue; }
			auto& interactingBody = m_world->at(j);
			rootAccel += BodyBodyInteractionPlummer(rootBody, interactingBody, 0.f);
		}
		bodyAccelerations.emplace_back(rootAccel);
	}

	for (int i = 0; i < m_world->size(); ++i) {
		Body& body = (*m_world)[i];

		body = LeapfrogIntegrate(body, bodyAccelerations.at(i), m_timeStep);
	}
}