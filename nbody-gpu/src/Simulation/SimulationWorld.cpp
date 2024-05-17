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