#pragma once
#ifdef PLATFORM_WIN32

#include <Application/WindowingApi.h>

class WindowingApiWin32Impl : public WindowingApi
{
public:
	virtual void EnableDarkModeImpl(GLFWwindow*) override;
};

#endif