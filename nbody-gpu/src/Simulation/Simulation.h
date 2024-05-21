#pragma once

#include <Simulation/SimulationWorld.h>
#include <Simulation/Time.h>
#include <memory>
#include <sycl/sycl.hpp>
#include <Application/Profiler.h>

namespace Simulation {

	class Instance
	{
		using SimWorld = Simulation::World;
	public:
		enum class RunMode { SEQUENTIAL, PARALLEL };

		Instance(std::unique_ptr<World>&&, Duration timeStep);
		Instance(Instance& other);
		//Instance(Instance& other, Duration timeStep);

		void Progress(int steps = 1);

		const World& World() const;

		float Softening() const;
		void SetSoftening(float);

		Duration StepLength() const;
		void SetStepLength(Duration);
		int StepsTaken() const;

		bool Started() const;

		RunMode Mode() const;
		void SetMode(RunMode);

		std::pair<int, int> GetGroupSize() const;
		void SetGroupSize(int gravityStep, int integrationStep);

		const Profiler& GetProfiler() const;
		bool IsProfiling() const;
		void SetIsProfiling(bool);

		void ResetProfiler();

	private:
		void PerformStepSequential();

		void PerformStepParallel();

		std::unique_ptr<SimWorld> m_world;

		Duration m_timeStep;
		int m_stepsTaken = 0; bool m_started = false;

		float m_softening = 0.01f;

		RunMode m_runMode = RunMode::SEQUENTIAL;
		size_t m_gravityStepGroupSize = 10;
		size_t m_integrationStepGroupSize = 10;

		bool m_isProfiling = false;
		Profiler m_profiler;

		sycl::queue m_queue;
		sycl::queue SyclSetup();
	};
}