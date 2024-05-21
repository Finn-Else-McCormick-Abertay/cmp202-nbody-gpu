#include "Application.h"

#include <Rendering/DrawFunctions.h>
#include <imgui_internal.h>

using namespace Rendering;

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

	int windowWidth, windowHeight;
	glfwGetWindowSize(inst.p_window, &windowWidth, &windowHeight);
	inst.m_cameraController.SetViewport(ImVec2(windowWidth, windowHeight));

	inst.m_simulationSettingsWindow = SimulationSettingsWindow(&inst.m_simulation);
	inst.m_generationWindow = GenerationWindow(&inst.m_simulation);
	inst.m_timeWindow = TimeWindow(&inst.m_simulation);
	inst.m_inspectWindow = InspectWindow(&inst.m_simulation);
}

void Application::Enter() {
	auto& inst = Singleton();

	while (!glfwWindowShouldClose(inst.p_window)) {
		inst.Update();
		glfwPollEvents();
	}
}

void Application::Update() {
	WindowingApi::NewFrame(); m_imguiMidFrame = true;
	{
		DrawMenuBar();
		DrawMainWindow();

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		float2 workPos = to_float2(viewport->WorkPos);

		float currentHeight = 140.f;
		const float margin = 10.f;

		auto propogateHeight = [&](auto& window) {
			window.Draw(to_imvec(workPos + float2(25.f, currentHeight)));
			if (window.show) { currentHeight += window.size.y + margin; }
		};

		propogateHeight(m_generationWindow);
		propogateHeight(m_timeWindow);
		propogateHeight(m_inspectWindow);
		propogateHeight(m_viewSettingsWindow);
		propogateHeight(m_simulationSettingsWindow);
	}
	WindowingApi::Render(p_window); m_imguiMidFrame = false;
}

void Application::DrawMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Menu Item");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Generate Window", "G", &m_generationWindow.show);
			ImGui::MenuItem("Time Control Panel", "T", &m_timeWindow.show);
			ImGui::MenuItem("Inspect Window", "I", &m_inspectWindow.show);
			ImGui::MenuItem("View Settings Window", "V", &m_viewSettingsWindow.show);
			ImGui::MenuItem("Simulation Settings Window", "S", &m_simulationSettingsWindow.show);
			ImGui::EndMenu();
		}
		ImGui::PopItemFlag();
		ImGui::EndMainMenuBar();
	}
}

void Application::DrawMainWindow() {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;
	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoMouseInputs;
	windowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::Begin("Main Window", NULL, windowFlags);

	m_drawQueue.SetCamera(m_cameraController.Camera());
	m_drawQueue.SetWindowOffset(to_float2(ImGui::GetWindowPos()));

	if (m_showAxes) DrawAxes(m_drawQueue);
	if (m_showGrid) DrawGrid(m_drawQueue, m_showAxes);

	if (Simulation() != nullptr) {
		Simulation()->Progress(m_timeWindow.isPaused ? 0 : m_timeWindow.stepsPerFrame);
		SimulationDrawParams params;
		params.massScaleFactor = m_cameraController.GetScale() * m_viewSettingsWindow.massScaleFactor;
		params.mapSpeedToColor = m_viewSettingsWindow.showSpeedColors;
		params.drawVelocityArrows = m_viewSettingsWindow.showVelocityArrows;
		params.unitScale = 1.f / m_viewSettingsWindow.unitToMetres;
		DrawSimulation(Simulation()->World(), m_drawQueue, { m_inspectWindow.selectedIndex }, params);
	}

	m_drawQueue.ImGuiRender(ImGui::GetWindowDrawList());

	m_cameraController.DisplayInfoChildWindow();

	ImGui::End();
}

Simulation::Instance* Application::Simulation() { return Singleton().m_simulation.get(); }
void Application::SetSimulation(std::unique_ptr<Simulation::Instance>&& ptr) { Singleton().m_simulation = std::move(ptr); }

Console& Application::Output() { return m_console; }

bool Application::ImGuiMidFrame() { return Singleton().m_imguiMidFrame; }

void Application::__windowRefreshCallback(GLFWwindow*) {
	if (ImGuiMidFrame()) { return; }
	Singleton().Update();
}

void Application::__windowPositionCallback(GLFWwindow*, int x, int y) {
	if (ImGuiMidFrame()) { return; }
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

	auto& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard) { return; }

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