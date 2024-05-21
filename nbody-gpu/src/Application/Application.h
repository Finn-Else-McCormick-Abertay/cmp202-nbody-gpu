#pragma once

#include <Application/WindowingApi.h>

#include <Application/Console.h>
#include <Simulation/Simulation.h>
#include <Rendering/CameraController.h>
#include <Rendering/DrawQueue.h>

#include <Application/Window/TimeWindow.h>
#include <Application/Window/InspectWindow.h>
#include <Application/Window/ViewSettingsWindow.h>
#include <Application/Window/GenerationWindow.h>
#include <Application/Window/SimulationSettingsWindow.h>

class Application
{
public:
	static void Init();
	static void Enter();

	void Update();

	Console& Output();

	static void SetSimulation(std::unique_ptr<Simulation::Instance>&&);

	static Simulation::Instance* Simulation();

private:
	Application() = default;
	Application(Application&) = delete;
	~Application();

	static Application& Singleton();

	void DrawMenuBar();
	void DrawMainWindow();

	SimulationSettingsWindow m_simulationSettingsWindow;
	GenerationWindow m_generationWindow;
	TimeWindow m_timeWindow;
	InspectWindow m_inspectWindow;
	ViewSettingsWindow m_viewSettingsWindow;

	Console m_console;

	GLFWwindow* p_window = nullptr;
	ImGuiContext* p_imguiContext = nullptr;
	bool m_imguiMidFrame = false;
	static bool ImGuiMidFrame();

	std::unique_ptr<Simulation::Instance> m_simulation = nullptr;
	Rendering::CameraController m_cameraController;
	Rendering::DrawQueue m_drawQueue;

	double m_cursorX = 0.0, m_cursorY = 0.0;

	bool m_showAxes = true, m_showGrid = true;

public:
	static void __windowRefreshCallback(GLFWwindow*);
	static void __windowPositionCallback(GLFWwindow*, int x, int y);
	static void __framebufferSizeCallback(GLFWwindow*, int width, int height);
	static void __windowCloseCallback(GLFWwindow*);

	static void __mousePositionCallback(GLFWwindow*, double xPos, double yPos);
	static void __scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void __keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};