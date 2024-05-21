#pragma once

#include <Simulation/Body.h>
#include <random>
#include <functional>
#include <glm/gtx/vector_angle.hpp>

#include <RandomDistributions.h>

namespace Random {
	using GeneratorCallable = std::function<Simulation::Body(RandomEngine&)>;

	template<typename MassDist = normal, typename XDist = normal, typename ZDist = normal, typename YDist = normal, typename SpeedDist = normal>
	class DiskGenerator
	{
	public:
		DiskGenerator(
			const MassDist::param_type& massParams,
			const XDist::param_type& xParams,
			const ZDist::param_type& zParams,
			const YDist::param_type& yParams,
			const SpeedDist::param_type& speedParams,
			bool clockwise = true
		) : m_massDist(massParams), m_xDist(xParams), m_zDist(zParams), m_yDist(yParams), m_speedDist(speedParams), m_clockwise(clockwise) {}

		Simulation::Body operator()(RandomEngine& gen) {
			float mass = m_massDist(gen);
			Simulation::float3 pos = Simulation::float3(m_xDist(gen), m_yDist(gen), m_zDist(gen));

			float speed = m_speedDist(gen);

			Simulation::float3 dirFromOrigin = glm::normalize(pos);
			Simulation::float3 upVec = Simulation::float3(0.f, 1.f, 0.f);

			Simulation::float3 moveDir = glm::cross(dirFromOrigin, upVec);

			Simulation::float3 vel = moveDir * speed;

			return { mass, pos, vel };
		}

	private:
		MassDist m_massDist;
		XDist m_xDist;
		ZDist m_zDist;
		YDist m_yDist;
		SpeedDist m_speedDist;
		bool m_clockwise;
	};

	template<typename MassDist, typename XDist, typename ZDist, typename YDist, typename SpeedDist>
	static auto MakeDiskGenerator(const MassDist& massDist, const XDist& xDist, const ZDist& zDist, const YDist& yDist, const SpeedDist& speedDist, bool clockwise) {
		return DiskGenerator<MassDist, XDist, ZDist, YDist, SpeedDist>(massDist.param(), xDist.param(), zDist.param(), yDist.param(), speedDist.param(), clockwise);
	}
}