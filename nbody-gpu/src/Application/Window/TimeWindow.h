#pragma once

#include <Application/Window/Window.h>
#include <memory>
#include <Simulation/Simulation.h>

struct TimeWindow : public Window
{
	TimeWindow(std::unique_ptr<Simulation::Instance>* simulation = nullptr);

	std::unique_ptr<Simulation::Instance>* p_simulationPtr;
	bool isPaused = true;
	int stepsPerFrame = 1;
	float stepLengthNextSim = 1.f;

protected:
	virtual void DrawWindowContents() override;
};