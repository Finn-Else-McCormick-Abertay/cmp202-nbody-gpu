#include "WindowingApi.h"

#include <memory>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stb_image.h>

#ifdef PLATFORM_WIN32
#include <Window/WindowingApiWin32Impl.h>
#define MAKE_WINDOWING_API() std::unique_ptr<WindowingApi>(new WindowingApiWin32Impl);
#endif

WindowingApi& WindowingApi::Singleton() {
	static std::unique_ptr<WindowingApi> ptr = MAKE_WINDOWING_API();
	return *ptr;
}

std::pair<GLFWwindow*, ImGuiContext*> WindowingApi::Init(const char* windowName, const std::function<void(GLFWwindow*)>& installCallbacks) {
	auto& inst = Singleton();

	glfwInit();
	inst.m_glfwInitialised = true;

	// Create GLFW window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1280, 960, windowName, NULL, NULL);

	EnableDarkMode(window);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Vsync

	// Setup OpenGL
	glewInit();

	// Install user callbacks before initialising imgui (which installs its own)
	installCallbacks(window);

	// Setup Dear ImGui
	IMGUI_CHECKVERSION();
	ImGuiContext* context = ImGui::CreateContext();

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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	inst.m_imguiInitialised = true;

	return std::make_pair(window, context);
}

void WindowingApi::Shutdown() {
	auto& inst = Singleton();

	if (inst.m_imguiInitialised) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	if (inst.m_glfwInitialised) {
		glfwTerminate();
	}
}

void WindowingApi::NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void WindowingApi::Render(GLFWwindow* window) {
	// End imgui frame, finalise draw data
	ImGui::Render();

	// Setup OpenGL viewport
	glfwMakeContextCurrent(window);
	int display_w, display_h; glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render imgui draw data to OpenGL viewport
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(window);
	}

	glfwSwapBuffers(window);
}

void WindowingApi::SetWindowIcon(GLFWwindow* window, const std::vector<std::string>& imagePaths) {
	int numImages = imagePaths.size();
	GLFWimage* images = new GLFWimage[numImages];
	for (int i = 0; i < numImages; ++i) {
		std::string path = imagePaths.at(i);
		GLFWimage& image = images[i];
		image.pixels = stbi_load(path.c_str(), &image.width, &image.height, NULL, 4);
	}

	glfwSetWindowIcon(window, numImages, images);

	for (int i = 0; i < numImages; ++i) {
		stbi_image_free(images[i].pixels);
	}
	delete[] images;
}

void WindowingApi::EnableDarkMode(GLFWwindow* window) {
	Singleton().EnableDarkModeImpl(window);
}