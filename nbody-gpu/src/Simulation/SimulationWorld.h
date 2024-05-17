#pragma once

#include <Simulation/Body.h>
#include <vector>
#include <Simulation/RandomGen.h>
#include <random>

namespace Simulation {

	class World
	{
	public:
		World(World&) = default;
		World(const World&) = default;

		static World RandomWorld(size_t size, const Random::GeneratorCallable& BodyGen, int seed = -1) {
			World world = World(size);
			unsigned int generationSeed; if (seed > 0) { generationSeed = seed; } else { std::random_device randomDevice; generationSeed = randomDevice(); }
			Random::RandomEngine gen(generationSeed);
			for (size_t i = 0; i < size; ++i) { world.m_bodies.emplace_back(BodyGen(gen)); }
			return world;
		}

		Body& operator[](size_t index);
		const Body& at(size_t index) const;
		size_t size() const;

		std::vector<Body>::iterator begin();
		std::vector<Body>::iterator end();

		std::vector<Body>::const_iterator cbegin() const;
		std::vector<Body>::const_iterator cend() const;

	private:
		std::vector<Body> m_bodies;

		World(size_t size = 0);
	};
}