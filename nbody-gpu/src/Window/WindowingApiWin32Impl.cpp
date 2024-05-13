#ifdef PLATFORM_WIN32
#include "WindowingApiWin32Impl.h"

#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

//#include <dwmapi.h>

void WindowingApiWin32Impl::EnableDarkModeImpl(GLFWwindow* glfwWindow) {
	/*
	HWND win32Window = glfwGetWin32Window(glfwWindow);

	BOOL useDarkMode = true;
	BOOL success = SUCCEEDED(DwmSetWindowAttribute(win32Window, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode)));
	*/
}

#endif