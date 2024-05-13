#include "Application.h"

#include <Rendering/DrawFunctions.h>

Application& Application::Singleton() {
	static Application inst;
	return inst;
}

Application::~Application() {
}

void Application::Init() {
	auto& inst = Singleton();

	auto [window, context] = WindowingApi::Init("CMP202 GPU Project",
		[](GLFWwindow* window) {
			glfwSetWindowRefreshCallback(window, __windowRefreshCallback);
			glfwSetWindowPosCallback(window, __windowPositionCallback);
			glfwSetFramebufferSizeCallback(window, __framebufferSizeCallback);
			glfwSetWindowCloseCallback(window, __windowCloseCallback);

			glfwSetCursorPosCallback(window, __mousePositionCallback);
			glfwSetScrollCallback(window, __scrollCallback);
			glfwSetKeyCallback(window, __keyCallback);
		});

	inst.p_window = window;
	inst.p_imguiContext = context;

	WindowingApi::SetWindowIcon(window, { "assets/icon/badicon.png", "assets/icon/badicon.png", "assets/icon/badicon.png" });

	//WindowingApi::RefreshTitleBarThemeColor(inst.p_window);

	/*
	// System color callback
	WindowingApi::SetSystemColorCallback([](bool isDarkMode, const std::map<WindowingApi::UIColorType, ImColor>& colorMap)->void {
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = colorMap.at(WindowingApi::UIColorType::FOREGROUND);
		style.Colors[ImGuiCol_WindowBg] = colorMap.at(WindowingApi::UIColorType::BACKGROUND);
		style.Colors[ImGuiCol_MenuBarBg] = colorMap.at(WindowingApi::UIColorType::BACKGROUND);
	});
	*/


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
	WindowingApi::NewFrame();
	{
		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();

		DrawMenuBar();
		DrawMainWindow(mainViewport);
	}
	WindowingApi::Render(p_window);
}

void Application::DrawMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Menu Item");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Application::DrawMainWindow(const ImGuiViewport* viewport) {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;
	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoMouseInputs;
	windowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::Begin("Main Window", NULL, windowFlags);

	m_drawQueue.SetCamera(m_cameraController.Camera());
	m_drawQueue.SetWindowOffset(to_float2(ImGui::GetWindowPos()));

	DrawAxes(m_drawQueue);
	DrawGrid(m_drawQueue);

	if (Simulation() != nullptr) {
		//Simulation()->Progress();
		DrawSimulation(Simulation()->World(), m_drawQueue);
	}

	m_drawQueue.ImGuiRender(ImGui::GetWindowDrawList());

	m_cameraController.DisplayInfoChildWindow();

	ImGui::End();
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
	if (!io.WantCaptureMouse) {
		bool middleMouse = io.MouseDown[2];
		bool shiftHeld = io.KeyShift;

		if (middleMouse) {
			if (shiftHeld) { inst.m_cameraController.Pan(mouseMove); }
			else { inst.m_cameraController.Rotate(mouseMove); }
		}
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