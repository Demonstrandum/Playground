#pragma once

#include "common.h"
#include <GLFW/glfw3.h>

namespace native {
	u0 *windowHandle(GLFWwindow *window);
#ifdef KERNEL_DARWIN
	u0 *metalLayer(u0 *cocoaWindow);
#endif
}
