#pragma once

#include <Application/Window/Window.h>
#include <memory>
#include <Simulation/Simulation.h>

struct SimulationSettingsWindow : public Window
{
	SimulationSettingsWindow(std::unique_ptr<Simulation::Instance>* simulation = nullptr);

	std::unique_ptr<Simulation::Instance>* p_simulationPtr;

protected:
	virtual void DrawWindowContents() override;
};