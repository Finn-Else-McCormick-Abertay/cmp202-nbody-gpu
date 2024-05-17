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

		float currentHeight = 140.f;
		const float margin = 10.f;

		auto propogateHeight = [&](auto DrawFunc) {
			float windowHeight = ((*this).*DrawFunc)(currentHeight);
			if (windowHeight > 0.f) { currentHeight += windowHeight + margin; }
		};

		propogateHeight(&Application::DrawGenerateWindow);
		propogateHeight(&Application::DrawTimeWindow);
		propogateHeight(&Application::DrawInspectWindow);
		propogateHeight(&Application::DrawDebugWindow);
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
			ImGui::MenuItem("Generate Window", "G", &m_showGenerateWindow);
			ImGui::MenuItem("Time Control Panel", "T", &m_showTimeWindow);
			ImGui::MenuItem("Inspect Window", "I", &m_showInspectWindow);
			ImGui::MenuItem("Debug Window", "D", &m_showDebugWindow);
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
		Simulation()->Progress(m_simPaused ? 0 : m_stepsPerFrame);
		DrawSimulation(Simulation()->World(), m_drawQueue, { m_inspectSelectedIndex });
	}

	m_drawQueue.ImGuiRender(ImGui::GetWindowDrawList());

	m_cameraController.DisplayInfoChildWindow();

	ImGui::End();
}

float Application::DrawGenerateWindow(float vOffset) {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;

	return DrawWindowGeneric("Generate", windowFlags, &m_showGenerateWindow, float2(25.f, vOffset), float2(300.f, 130.f),
		[&]() {
			auto simulation = Simulation();

			if (simulation) {
			}
		});
}

float Application::DrawTimeWindow(float vOffset) {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;

	return DrawWindowGeneric("Time Control", windowFlags, &m_showTimeWindow, float2(25.f, vOffset), float2(300.f, 130.f),
		[&]() {
			auto simulation = Simulation();
			if (simulation) {
				if (!simulation->Started()) {
					if (ImGui::Button("Begin")) {
						m_simulation = std::unique_ptr<Simulation::Instance>(new Simulation::Instance(*m_simulation, Duration(m_stepLengthNextSim)));
						m_simPaused = false;
					}
					if (ImGui::InputFloat("Step length", &m_stepLengthNextSim, 1.f, 100.f, "%.3f seconds")) {
						m_stepLengthNextSim = std::max(m_stepLengthNextSim, 0.f);
					}
					auto stepDur = Duration(m_stepLengthNextSim);
					ImGui::Text("Step Length: %s", stepDur.AsFormattedString().c_str());
				}
				else {
					if (ImGui::Button(m_simPaused ? "Play" : "Pause")) { m_simPaused = !m_simPaused; }
					ImGui::Text("Step Length: %s", simulation->StepLength().AsFormattedString().c_str());
					float stepsPerFrameAsFloat = m_stepsPerFrame;
					if (ImGui::InputFloat("Multiplier", &stepsPerFrameAsFloat, 1.f, 5.f, "%gx")) {
						m_stepsPerFrame = static_cast<int>(stepsPerFrameAsFloat);
					}
					Duration elapsedTime = simulation->StepsTaken() * simulation->StepLength();
					ImGui::Text("Elapsed Time: %s", elapsedTime.AsFormattedString().c_str());
				}
			}
		});
}

float Application::DrawInspectWindow(float vOffset) {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;

	return DrawWindowGeneric("Inspect", windowFlags, &m_showInspectWindow, float2(25.f, vOffset), float2(),
		[&]() {
			auto simulation = Simulation();
			if (simulation) {
				auto& world = simulation->World();
				ImGui::Text("World size: %i", world.size());
				ImGui::InputInt("Body index", &m_inspectSelectedIndex);
				if (m_inspectSelectedIndex >= 0 && m_inspectSelectedIndex < world.size()) {
					auto& body = world.at(m_inspectSelectedIndex);
					ImGui::Text("Pos: %g, %g, %g", body.position.x, body.position.y, body.position.z);
					ImGui::Text("Vel: %g, %g, %g", body.velocity.x, body.velocity.y, body.velocity.z);
					ImGui::Text("Mass: %g", body.mass);
				}
			}
		});
}

float Application::DrawDebugWindow(float vOffset) {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;

	return DrawWindowGeneric("Debug", windowFlags, &m_showDebugWindow, float2(25.f, vOffset), float2(),
		[&]() {
		});
}

float Application::DrawWindowGeneric(std::string name, ImGuiWindowFlags windowFlags, bool* p_show, float2 offset, float2 size, const std::function<void()>& Func) {
	float windowHeight = size.y;

	if ((p_show == NULL) || (*p_show)) {
		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		float2 windowPos = to_float2(mainViewport->WorkPos) + offset;

		ImGui::SetNextWindowPos(to_imvec(windowPos), ImGuiCond_Appearing);
		if (windowHeight == 0.f) { windowFlags |= ImGuiWindowFlags_AlwaysAutoResize; } else { ImGui::SetNextWindowSize(to_imvec(size), ImGuiCond_Appearing); }
		ImGui::Begin(name.c_str(), p_show, windowFlags);

		Func();

		if (windowHeight == 0.f) { windowHeight = ImGui::GetWindowHeight(); }
		ImGui::End();
	}

	return windowHeight;
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