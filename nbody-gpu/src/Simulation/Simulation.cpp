#include "Simulation.h"

#include "GravityInteraction.h"
#include "Integration.h"
#include <iostream>

using namespace Simulation;
using namespace sycl;

Simulation::Instance::Instance(std::unique_ptr<SimWorld>&& world, Duration timeStep) : m_world(std::move(world)), m_timeStep(timeStep), m_queue(SyclSetup()) {}

Simulation::Instance::Instance(Instance& other)
	: m_world(std::move(other.m_world)), m_timeStep(std::move(other.m_timeStep)), m_softening(other.m_softening),
	m_runMode(other.m_runMode), m_gravityStepGroupSize(other.m_gravityStepGroupSize), m_integrationStepGroupSize(other.m_integrationStepGroupSize),
	m_profiler(std::move(other.m_profiler)),
	m_queue(other.m_queue) {}

queue Simulation::Instance::SyclSetup() {
	static auto exception_handler = [](sycl::exception_list e_list) {
		for (std::exception_ptr const& e : e_list) {
			try {
				std::rethrow_exception(e);
			}
			catch (std::exception const& e) {
				std::terminate();
			}
		}
		};

	device device;
	// Try to get a gpu, fallback to whatever's available
	try {
		device = gpu_selector().select_device();
	}
	catch (sycl::_V1::runtime_error e) {
		device = default_selector().select_device();
		std::cout << "GPU unavailable.\n";
	}

	std::cout << "SYCL Device: " << device.get_info<info::device::name>() << std::endl;

	return queue(device, exception_handler);
}

const World& Simulation::Instance::World() const { return *m_world; }
Duration Simulation::Instance::StepLength() const { return m_timeStep; }
void Simulation::Instance::SetStepLength(Duration step) { m_timeStep = step; }
int Simulation::Instance::StepsTaken() const { return m_stepsTaken; }
bool Simulation::Instance::Started() const { return m_started; }

float Simulation::Instance::Softening() const { return m_softening; }
void Simulation::Instance::SetSoftening(float softening) { m_softening = softening; }

Simulation::Instance::RunMode Simulation::Instance::Mode() const { return m_runMode; }
void Simulation::Instance::SetMode(RunMode mode) { m_runMode = mode; }

std::pair<int, int> Simulation::Instance::GetGroupSize() const { return std::make_pair(m_gravityStepGroupSize, m_integrationStepGroupSize); }
void Simulation::Instance::SetGroupSize(int gravStep, int intStep) {
	m_gravityStepGroupSize = gravStep; m_integrationStepGroupSize = intStep;
}

const Profiler& Simulation::Instance::GetProfiler() const { return m_profiler; }
bool Simulation::Instance::IsProfiling() const { return m_isProfiling; }
void Simulation::Instance::SetIsProfiling(bool val) { m_isProfiling = val; }

void Simulation::Instance::ResetProfiler() { m_profiler = Profiler(); }

void Simulation::Instance::Progress(int steps) {
	for (int i = 0; i < steps; ++i) {
		if (m_isProfiling) { m_profiler.StartTimer(); }
		switch (m_runMode) {
		case RunMode::SEQUENTIAL:	PerformStepSequential(); break;
		case RunMode::PARALLEL:		PerformStepParallel(); break;
		}
		if (m_isProfiling) { m_profiler.StopTimer(); }
	}
}

void Simulation::Instance::PerformStepSequential() {
	++m_stepsTaken; m_started = true;
	std::vector<float3> bodyAccelerations; bodyAccelerations.reserve(m_world->size());

	// Gravity Step (calculate new accelerations)
	for (int i = 0; i < m_world->size(); ++i) {
		float3 rootAccel = float3();
		auto& rootBody = m_world->at(i);
		for (int j = 0; j < m_world->size(); ++j) {
			if (i == j) { continue; }
			auto& interactingBody = m_world->at(j);
			rootAccel += BodyBodyInteractionPlummer(rootBody, interactingBody, m_softening);
		}
		bodyAccelerations.emplace_back(rootAccel);
	}

	// Integration Step (update velocity and position based on accel)
	for (int i = 0; i < m_world->size(); ++i) {
		Body& body = (*m_world)[i];

		body = LeapfrogIntegrate(body, bodyAccelerations.at(i), m_timeStep.AsSeconds());
	}
}


void Simulation::Instance::PerformStepParallel() {
	++m_stepsTaken; m_started = true;

	size_t numBodies = m_world->size();

	auto bodyBuf = m_world->buffer();

	std::vector<float3> bodyAccelerations = std::vector<float3>(numBodies);
	buffer<float3, 1> accelBuf = buffer<float3, 1>(bodyAccelerations.data(), range<1>(bodyAccelerations.size()));


	// Gravity Step (calculate new accelerations)
	auto gravityStep = m_queue.submit([&](handler& handler) {
		auto bodyAcc = bodyBuf.get_access<access::mode::read>(handler);
		auto accelAcc = accelBuf.get_access<access::mode::read_write>(handler);

		float softening = m_softening;
		size_t groupSize = m_gravityStepGroupSize;

		auto ndr = nd_range<2>(range<2>(numBodies, numBodies), range<2>(groupSize, groupSize));

		handler.parallel_for(ndr, [=](nd_item<2> item) {

			size_t rootId = item.get_global_id(0);
			size_t interactId = item.get_global_id(1);

			if (rootId == interactId) { return; }

			float3 newAccel = BodyBodyInteractionPlummer(bodyAcc[rootId], bodyAcc[interactId], softening);
			accelAcc[rootId] += newAccel;
		});
	});

	m_queue.wait();


	// Integration Step (update velocity and position based on accel)
	m_queue.submit([&](handler& handler) {
		//handler.depends_on(gravityStep);

		auto bodyAcc = bodyBuf.get_access<access::mode::read_write>(handler);
		auto accelAcc = accelBuf.get_access<access::mode::read>(handler);

		float timeStep = m_timeStep.AsSeconds();
		size_t groupSize = m_integrationStepGroupSize;

		handler.parallel_for(nd_range<1>(range<1>(numBodies), range<1>(groupSize)), [=](nd_item<1> item) {
			size_t id = item.get_global_id(0);
			bodyAcc[id] = LeapfrogIntegrate(bodyAcc[id], accelAcc[id], timeStep);
		});
	});

	m_queue.wait();
}