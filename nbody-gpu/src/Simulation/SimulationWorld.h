#pragma once

#include <Simulation/Body.h>
#include <vector>
#include <Simulation/RandomGen.h>
#include <random>
#include <sycl/sycl.hpp>

namespace Simulation {

	class World
	{
	public:
		World(World&) = default;
		World(const World&) = default;

		static World RandomWorld(size_t size, const Random::GeneratorCallable& BodyGen, int seed = -1);

		static World CentralStar(float mass);

		static World Combine(const World&, const World&);

		Body& operator[](size_t index);
		const Body& at(size_t index) const;
		size_t size() const;

		std::vector<Body>::iterator begin();
		std::vector<Body>::iterator end();

		std::vector<Body>::const_iterator cbegin() const;
		std::vector<Body>::const_iterator cend() const;

		sycl::buffer<Body, 1> buffer();

	private:
		std::vector<Body> m_bodies;

		World(size_t size = 0);
	};
}