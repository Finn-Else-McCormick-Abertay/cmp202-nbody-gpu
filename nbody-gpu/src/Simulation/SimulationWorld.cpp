#include "SimulationWorld.h"

SimulationWorld::SimulationWorld(size_t size) { m_bodies.reserve(size); }

const Body& SimulationWorld::at(size_t index) const { return m_bodies.at(index); }
size_t SimulationWorld::size() const { return m_bodies.size(); }

std::vector<Body>::iterator SimulationWorld::begin() { return m_bodies.begin(); }
std::vector<Body>::iterator SimulationWorld::end() { return m_bodies.end(); }

std::vector<Body>::const_iterator SimulationWorld::cbegin() const { return m_bodies.cbegin(); }
std::vector<Body>::const_iterator SimulationWorld::cend() const { return m_bodies.cend(); }