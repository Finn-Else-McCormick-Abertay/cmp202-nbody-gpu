#pragma once

#include <Simulation/SimulationWorld.h>
#include <Rendering/DrawQueue.h>

void DrawAxes(DrawQueue&);

void DrawGimbal(DrawQueue&);

void DrawGrid(DrawQueue&);

void DrawSimulation(const SimulationWorld&, DrawQueue&);