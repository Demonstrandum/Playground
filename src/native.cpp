#include "native.h"

#ifdef KERNEL_WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define _GET_WINDOW glfwGetWin32Window
#endif

#ifdef KERNEL_LINUX
	#define GLFW_EXPOSE_NATIVE_X11
	#define GLFW_EXPOSE_NATIVE_WAYLAND
	#define _GET_WINDOW glfwGetWaylandWindow
#endif

#ifdef KERNEL_DARWIN
	#define GLFW_EXPOSE_NATIVE_COCOA
	#define _GET_WINDOW glfwGetCocoaWindow
#endif

#ifndef _GET_WINDOW
	#error "Unknown platform"
#endif

#include <GLFW/glfw3native.h>

namespace native {
	u0 *windowHandle(GLFWwindow *window)
	{
		u0 *handle = nullptr;
#ifdef KENREL_LINUX
		int platform = glfwGetPlatform();
		if (platform == GLFW_PLATFORM_X11)
			handle = glfwGetX11Window(window);
		else
			handle = _GET_WINDOW(window);
#else
		handle = _GET_WINDOW(window);
#endif
#ifdef KERNEL_DARWIN
		handle = metalLayer(handle);
#endif
		return handle;
	}
}
