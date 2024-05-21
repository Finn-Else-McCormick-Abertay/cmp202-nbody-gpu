#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <functional>
#include <vector>
#include <string>

#define TOKEN_APPEND_SUFFIX(TOKEN, SUFFIX) TOKEN##SUFFIX
#define DECL_STATIC_WITH_ABSTRACT_IMPL(RET_TYPE, METHOD_NAME, ...) \
public: static RET_TYPE METHOD_NAME(__VA_ARGS__); protected: virtual RET_TYPE TOKEN_APPEND_SUFFIX(METHOD_NAME, Impl)(__VA_ARGS__) = 0; public:

class WindowingApi
{
public:
	virtual ~WindowingApi() = default;

	static std::pair<GLFWwindow*, ImGuiContext*> Init(const char* windowName, const std::function<void(GLFWwindow*)>& installCallbacks);
	static void Shutdown();

	static void NewFrame();
	static void Render(GLFWwindow*);

	static void SetWindowIcon(GLFWwindow*, const std::vector<std::string>& paths);

	DECL_STATIC_WITH_ABSTRACT_IMPL(void, EnableDarkMode, GLFWwindow*)

protected:
	WindowingApi() = default;

private:
	static WindowingApi& Singleton();

	bool m_glfwInitialised = false, m_imguiInitialised = false;
};