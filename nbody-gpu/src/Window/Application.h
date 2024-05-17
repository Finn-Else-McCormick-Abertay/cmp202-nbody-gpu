#pragma once

#include <Window/WindowingApi.h>

#include "Console.h"
#include <Simulation/Simulation.h>
#include <Rendering/CameraController.h>
#include <Rendering/DrawQueue.h>

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
	float DrawGenerateWindow(float vOffset);
	float DrawTimeWindow(float vOffset);
	float DrawInspectWindow(float vOffset);
	float DrawDebugWindow(float vOffset);

	static float DrawWindowGeneric(std::string name, ImGuiWindowFlags windowFlags, bool* p_show, Rendering::float2 offset, Rendering::float2 size, const std::function<void()>&);

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
	bool m_showGenerateWindow = true;
	bool m_showTimeWindow = true; int m_stepsPerFrame = 1; bool m_simPaused = true; float m_stepLengthNextSim = 1.f;
	bool m_showInspectWindow = false; int m_inspectSelectedIndex = -1;
	bool m_showDebugWindow = false;

public:
	static void __windowRefreshCallback(GLFWwindow*);
	static void __windowPositionCallback(GLFWwindow*, int x, int y);
	static void __framebufferSizeCallback(GLFWwindow*, int width, int height);
	static void __windowCloseCallback(GLFWwindow*);

	static void __mousePositionCallback(GLFWwindow*, double xPos, double yPos);
	static void __scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void __keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};