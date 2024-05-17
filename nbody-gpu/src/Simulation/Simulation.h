#pragma once

#include <Simulation/SimulationWorld.h>
#include <Simulation/Time.h>
#include <memory>

namespace Simulation {

	class Instance
	{
		using SimWorld = Simulation::World;
	public:
		Instance(std::unique_ptr<World>&&, Duration timeStep);
		Instance(Instance& other);
		Instance(Instance& other, Duration timeStep);

		void Progress(int steps = 1);

		const World& World() const;
		Duration StepLength() const;
		int StepsTaken() const;

		bool Started() const;

	private:
		void PerformStep();

		std::unique_ptr<SimWorld> m_world;
		Duration m_timeStep;
		int m_stepsTaken = 0;
		bool m_started = false;
	};

}