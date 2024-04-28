#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Application& Application::Singleton() {
	static Application inst;
	return inst;
}

Application::Application() {
	if (glfwInit()) {
		Output().Info("Initialised GLFW.");
	}
	else {
		Output().Error("GLFW failed to initialise.");
	}
}

Application::~Application() {
	if (p_imguiContext) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	glfwTerminate();
}

void Application::Init() {
	auto& inst = Singleton();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	inst.p_window = glfwCreateWindow(640, 480, "CMP202 GPU Project", NULL, NULL);

	glfwMakeContextCurrent(inst.p_window);
	glfwSwapInterval(1); // Vsync

	// Setup OpenGL
	glewInit();

	// Install callbacks
	glfwSetWindowContentScaleCallback(inst.p_window, __windowContentScaleCallback);
	glfwSetFramebufferSizeCallback(inst.p_window, __framebufferSizeCallback);
	glfwSetWindowSizeCallback(inst.p_window, __windowSizeCallback);
	glfwSetWindowRefreshCallback(inst.p_window, __windowRefreshCallback);
	glfwSetWindowMaximizeCallback(inst.p_window, __windowMaximizeCallback);
	glfwSetWindowFocusCallback(inst.p_window, __windowFocusCallback);
	glfwSetWindowCloseCallback(inst.p_window, __windowCloseCallback);

	// Setup Dear ImGui
	IMGUI_CHECKVERSION();
	inst.p_imguiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(inst.p_window, true);
	ImGui_ImplOpenGL3_Init();
}

void Application::Enter() {
	auto& inst = Singleton();

	while (!glfwWindowShouldClose(inst.p_window)) {
		inst.Update();
		glfwPollEvents();
	}
}

void Application::Update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(p_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(p_window);
	}

	glfwSwapBuffers(p_window);
}


Console& Application::Output() { return m_console; }


void Application::__windowContentScaleCallback(GLFWwindow*, float xScale, float yScale) {}

void Application::__framebufferSizeCallback(GLFWwindow*, int width, int height) {}

void Application::__windowSizeCallback(GLFWwindow*, int width, int height) {}

void Application::__windowRefreshCallback(GLFWwindow*) {}

void Application::__windowMaximizeCallback(GLFWwindow*, int maximized) {}

void Application::__windowFocusCallback(GLFWwindow*, int focused) {}

void Application::__windowCloseCallback(GLFWwindow* glfwWindow) {}
