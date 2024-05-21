#pragma once

#include <Application/Window/Window.h>
#include <memory>
#include <Simulation/Simulation.h>

struct InspectWindow : public Window
{
	InspectWindow(std::unique_ptr<Simulation::Instance>* simulation = nullptr);

	std::unique_ptr<Simulation::Instance>* p_simulationPtr;

	int selectedIndex = -1;

protected:
	virtual void DrawWindowContents() override;
};