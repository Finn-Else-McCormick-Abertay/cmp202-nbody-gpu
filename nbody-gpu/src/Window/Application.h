#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "Console.h"
#include <Simulation/SimulationWorld.h>
#include <Rendering/CameraController.h>
#include <Rendering/DrawQueue.h>

class Application
{
public:
	static void Init();
	static void Enter();

	void Update();

	Console& Output();

	static SimulationWorld* World();
	static void SetWorld(std::unique_ptr<SimulationWorld>&&);

private:
	Application();
	Application(Application&) = delete;
	~Application();

	static Application& Singleton();

	Console m_console;

	GLFWwindow* p_window = nullptr;
	ImGuiContext* p_imguiContext = nullptr;

	std::unique_ptr<SimulationWorld> m_simulation = nullptr;
	CameraController m_cameraController;
	DrawQueue m_drawQueue;

	double m_cursorX = 0.0, m_cursorY = 0.0;

public:
	static void __windowRefreshCallback(GLFWwindow*);
	static void __windowPositionCallback(GLFWwindow*, int x, int y);
	static void __framebufferSizeCallback(GLFWwindow*, int width, int height);
	static void __windowCloseCallback(GLFWwindow*);

	static void __mousePositionCallback(GLFWwindow*, double xPos, double yPos);
	static void __scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void __keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};