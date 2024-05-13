#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Rendering/DrawFunctions.h>

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
	glfwSetWindowRefreshCallback(inst.p_window, __windowRefreshCallback);
	glfwSetWindowPosCallback(inst.p_window, __windowPositionCallback);
	glfwSetFramebufferSizeCallback(inst.p_window, __framebufferSizeCallback);
	glfwSetWindowCloseCallback(inst.p_window, __windowCloseCallback);

	glfwSetCursorPosCallback(inst.p_window, __mousePositionCallback);
	glfwSetScrollCallback(inst.p_window, __scrollCallback);
	glfwSetKeyCallback(inst.p_window, __keyCallback);

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

	int windowWidth, windowHeight;
	glfwGetWindowSize(inst.p_window, &windowWidth, &windowHeight);
	inst.m_cameraController.SetViewport(ImVec2(windowWidth, windowHeight));
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

	// Create windows
	const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	//ImGui::DockSpaceOverViewport(mainViewport);

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;
	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	//windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	windowFlags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoMouseInputs;
	windowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(mainViewport->WorkPos);
	ImGui::SetNextWindowSize(mainViewport->WorkSize);
	ImGui::SetNextWindowViewport(mainViewport->ID);
	ImGui::Begin("Main Window", NULL, windowFlags);

	m_drawQueue.SetCamera(m_cameraController.Camera());
	m_drawQueue.SetWindowOffset(to_float2(ImGui::GetWindowPos()));

	DrawAxes(m_drawQueue);
	DrawGrid(m_drawQueue);

	if (Simulation() != nullptr) {
		Simulation()->Progress();
		DrawSimulation(Simulation()->World(), m_drawQueue);
	}

	m_drawQueue.ImGuiRender(ImGui::GetWindowDrawList());

	m_cameraController.DisplayInfoChildWindow();

	ImGui::End();

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



Simulation* Application::Simulation() { return Singleton().m_simulation.get(); }
void Application::SetSimulation(SimulationPtr&& ptr) { Singleton().m_simulation = std::move(ptr); }

Console& Application::Output() { return m_console; }


void Application::__windowRefreshCallback(GLFWwindow*) {
	Singleton().Update();
}

void Application::__windowPositionCallback(GLFWwindow*, int x, int y) {
	Singleton().Update();
}

void Application::__framebufferSizeCallback(GLFWwindow*, int width, int height) {
	Singleton().m_cameraController.SetViewport(ImVec2(width, height));
}

void Application::__windowCloseCallback(GLFWwindow*) {}


void Application::__mousePositionCallback(GLFWwindow*, double xPos, double yPos) {
	auto& inst = Singleton();

	float2 mouseMove = float2(inst.m_cursorX - xPos, inst.m_cursorY - yPos);

	auto& io = ImGui::GetIO();
	bool middleMouse = io.MouseDown[2];
	bool shiftHeld = io.KeyShift;

	if (middleMouse) {
		if (shiftHeld) { inst.m_cameraController.Pan(mouseMove); }
		else		   { inst.m_cameraController.Rotate(mouseMove); }
	}

	inst.m_cursorX = xPos;
	inst.m_cursorY = yPos;
}

void Application::__scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	auto& inst = Singleton();

	inst.m_cameraController.Scale(yOffset);
}

void Application::__keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto& inst = Singleton();

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_KP_0: inst.m_cameraController.Camera().SetPosition(float3()); break;
		case GLFW_KEY_KP_1: inst.m_cameraController.SetView(CameraController::View::FRONT); break;
		case GLFW_KEY_KP_3: inst.m_cameraController.SetView(CameraController::View::RIGHT); break;
		case GLFW_KEY_KP_7: inst.m_cameraController.SetView(CameraController::View::TOP); break;
		case GLFW_KEY_KP_9: inst.m_cameraController.ReverseView(); break;
		case GLFW_KEY_KP_5:
			inst.m_cameraController.SetType(
				(inst.m_cameraController.CurrentType() == CameraController::ORTHOGRAPHIC) ? CameraController::PERSPECTIVE : CameraController::ORTHOGRAPHIC);
			break;
		}
	}
}