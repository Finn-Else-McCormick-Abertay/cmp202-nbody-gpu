#pragma once

#include <Simulation/Body.h>
#include <vector>
#include <random>

class SimulationWorld
{
public:
	SimulationWorld(SimulationWorld&) = default;
	SimulationWorld(const SimulationWorld&) = default;

	template<typename PositionDistribution, typename MassDistribution,
		typename = std::enable_if_t<std::is_invocable_r_v<float, PositionDistribution, std::mt19937&>>,
		typename = std::enable_if_t<std::is_invocable_r_v<float, MassDistribution, std::mt19937&>>>
	static SimulationWorld RandomWorld(size_t size, PositionDistribution& posDist, MassDistribution& massDist, int seed = -1);


	const Body& at(size_t index) const;
	size_t size() const;

	std::vector<Body>::const_iterator cbegin() const;
	std::vector<Body>::const_iterator cend() const;

private:
	std::vector<Body> m_bodies;

	SimulationWorld(size_t size = 0);
};


template<typename PositionDistribution, typename MassDistribution, typename, typename>
SimulationWorld SimulationWorld::RandomWorld(size_t size, PositionDistribution& posDist, MassDistribution& massDist, int inputSeed) {
	SimulationWorld world = SimulationWorld(size);

	unsigned int generationSeed;
	if (inputSeed > 0) { generationSeed = inputSeed; }
	else {
		std::random_device randomDevice;
		generationSeed = randomDevice();
	}
	std::mt19937 gen(generationSeed);

	for (size_t i = 0; i < size; ++i) {
		glm::vec3 pos = glm::vec3(posDist(gen), posDist(gen), posDist(gen));
		float mass = massDist(gen);
		world.m_bodies.emplace_back(pos, mass);
	}

	return world;
}