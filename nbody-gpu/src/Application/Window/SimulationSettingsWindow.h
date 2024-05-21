#pragma once

#include <Application/Window/Window.h>
#include <memory>
#include <Simulation/Simulation.h>
#include <chrono>

struct SimulationSettingsWindow : public Window
{
	SimulationSettingsWindow(std::unique_ptr<Simulation::Instance>* simulation = nullptr);

	std::unique_ptr<Simulation::Instance>* p_simulationPtr;

protected:
	virtual void DrawWindowContents() override;

	int m_timesCount = 0;
	std::chrono::nanoseconds m_medianTime, m_meanTime;
};