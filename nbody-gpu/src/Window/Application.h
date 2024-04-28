#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "Console.h"

class Application
{
public:
	static void Init();
	static void Enter();

	void Update();

	Console& Output();

	static void __windowContentScaleCallback(GLFWwindow*, float xScale, float yScale);
	static void __framebufferSizeCallback(GLFWwindow*, int width, int height);
	static void __windowSizeCallback(GLFWwindow*, int width, int height);
	static void __windowRefreshCallback(GLFWwindow*);
	static void __windowMaximizeCallback(GLFWwindow*, int maximized);
	static void __windowFocusCallback(GLFWwindow*, int focused);
	static void __windowCloseCallback(GLFWwindow*);

private:
	Application();
	Application(Application&) = delete;
	~Application();

	static Application& Singleton();

	GLFWwindow* p_window = nullptr;
	ImGuiContext* p_imguiContext = nullptr;

	Console m_console;
};