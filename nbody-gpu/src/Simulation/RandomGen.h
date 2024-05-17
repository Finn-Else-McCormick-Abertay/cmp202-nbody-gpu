#pragma once

#include <Simulation/Body.h>
#include <random>
#include <functional>
#include <glm/gtx/vector_angle.hpp>

namespace Random {
	using RandomEngine = std::mt19937;

	using GeneratorCallable = std::function<Simulation::Body(RandomEngine&)>;

	template<
		typename MassDist = std::normal_distribution<float>,
		typename HDist = std::normal_distribution<float>,
		typename VDist = std::normal_distribution<float>,
		typename SpeedDist = std::normal_distribution<float>
	>
	class PlanetaryDiskGenerator
	{
	public:
		PlanetaryDiskGenerator(
			const MassDist::param_type& massParams,
			const HDist::param_type& hParams,
			const VDist::param_type& vParams,
			const SpeedDist::param_type& speedParams,
			bool clockwise = true
		) : m_massDist(massParams), m_hDist(hParams), m_vDist(vParams), m_speedDist(speedParams), m_clockwise(clockwise) {}

		Simulation::Body operator()(RandomEngine& gen) {
			float mass = m_massDist(gen);
			Simulation::float3 pos = Simulation::float3(m_hDist(gen), m_vDist(gen), m_hDist(gen));
			float speed = m_speedDist(gen);

			Simulation::float3 zeroBearingVec = Simulation::float3(1.f, 0.f, 0.f);
			Simulation::float3 dirFromOrigin = glm::normalize(pos);
			Simulation::float3 upVec = Simulation::float3(0.f, 1.f, 0.f);

			float bearingAngle = glm::orientedAngle(dirFromOrigin, zeroBearingVec, upVec);
			Simulation::float3 moveDir = glm::angleAxis(bearingAngle, upVec) * Simulation::float3(0.f, 0.f, (m_clockwise ? 1.f : -1.f));

			Simulation::float3 vel = moveDir * speed;

			return { mass, pos, vel };
		}

	private:
		MassDist m_massDist;
		HDist m_hDist;
		VDist m_vDist;
		SpeedDist m_speedDist;
		bool m_clockwise;
	};
}