#include "SimulationWorld.h"

using namespace Simulation;

Simulation::World::World(size_t size) { m_bodies.reserve(size); }

Body& Simulation::World::operator[](size_t index) { return m_bodies[index]; }
const Body& Simulation::World::at(size_t index) const { return m_bodies.at(index); }
size_t Simulation::World::size() const { return m_bodies.size(); }

std::vector<Body>::iterator Simulation::World::begin() { return m_bodies.begin(); }
std::vector<Body>::iterator Simulation::World::end() { return m_bodies.end(); }

std::vector<Body>::const_iterator Simulation::World::cbegin() const { return m_bodies.cbegin(); }
std::vector<Body>::const_iterator Simulation::World::cend() const { return m_bodies.cend(); }

sycl::buffer<Body, 1> Simulation::World::buffer() { return sycl::buffer<Body, 1>(m_bodies.data(), sycl::range<1>(m_bodies.size())); }

World Simulation::World::RandomWorld(size_t size, const Random::GeneratorCallable& BodyGen, int seed) {
	World world = World(size);
	unsigned int generationSeed;
	if (seed > 0) { generationSeed = seed; } else { std::random_device randomDevice; generationSeed = randomDevice(); }
	Random::RandomEngine gen(generationSeed);
	for (size_t i = 0; i < size; ++i) { world.m_bodies.emplace_back(BodyGen(gen)); }
	return world;
}

World Simulation::World::CentralStar(float mass) {
	World world = World(1);
	world.m_bodies.emplace_back(mass, float3(), float3());
	return world;
}


World Simulation::World::Combine(const World& lhs, const World& rhs) {
	World resultantWorld = World(lhs.size() + rhs.size());

	for (auto it = lhs.cbegin(); it != lhs.cend(); ++it) { resultantWorld.m_bodies.emplace_back(*it); }
	for (auto it = rhs.cbegin(); it != rhs.cend(); ++it) { resultantWorld.m_bodies.emplace_back(*it); }

	return resultantWorld;
}