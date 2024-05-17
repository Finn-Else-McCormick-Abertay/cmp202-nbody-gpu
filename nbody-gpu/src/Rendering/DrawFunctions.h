#pragma once

#include <Simulation/SimulationWorld.h>
#include <Rendering/DrawQueue.h>

#include <set>

namespace Rendering {

	void DrawAxes(DrawQueue&);

	void DrawGimbal(DrawQueue&);

	void DrawGrid(DrawQueue&, bool skipLinesOnAxis = true);

	void DrawSimulation(const Simulation::World&, DrawQueue&, const std::set<int>& highlighted = {});

}