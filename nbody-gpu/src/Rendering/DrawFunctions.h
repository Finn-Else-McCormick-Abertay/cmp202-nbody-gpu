#pragma once

#include <Simulation/SimulationWorld.h>
#include <Rendering/DrawQueue.h>

#include <set>

namespace Rendering {

	void DrawAxes(DrawQueue&);

	void DrawGimbal(DrawQueue&);

	void DrawGrid(DrawQueue&, bool skipLinesOnAxis = true);

	struct SimulationDrawParams {
		float massScaleFactor = 0.f;
		bool mapSpeedToColor = false;
		ImU32 colorMin = Color::BLUE;	float minThreshold = 0.1f;
		ImU32 colorMid = Color::RED;	float midThreshold = 20.f;
		ImU32 colorMax = Color::WHITE;	float maxThreshold = 40.f;

		bool drawVelocityArrows = false;
		float velocityArrowScaleFactor = 1.f;

		float unitScale = 1.f;

		SimulationDrawParams() = default;
	};

	void DrawSimulation(const Simulation::World&, DrawQueue&, const std::set<int>& highlighted = {}, const SimulationDrawParams& params = SimulationDrawParams());

}