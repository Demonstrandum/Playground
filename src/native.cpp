#include "native.h"

#ifdef KERNEL_WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define _GET_WINDOW glfwGetWin32Window
#endif

#ifdef KERNEL_LINUX
    #include <wayland-egl.h>
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

#include <iostream>

namespace native {
    u0* windowHandle(GLFWwindow *window)
    {
        u0* handle = nullptr;
#ifdef KERNEL_LINUX
        ifast platform = glfwGetPlatform();
        if (platform == GLFW_PLATFORM_X11) {
            std::cerr << "platform: X11 linux window\n";
            handle = (u0*)(uptr)glfwGetX11Window(window);
        } else {
            std::cerr << "platform: non-X11 linux window\n";
            handle = glfwGetWindowUserPointer(window);
        }
        if (handle == nullptr && platform == GLFW_PLATFORM_WAYLAND) {
            std::cerr << "platform: manually creating wayland window surface\n";
            ifast w, h;
            glfwGetFramebufferSize(window, &w, &h);
            wl_surface* surface = (wl_surface*)glfwGetWaylandWindow(window);
            if (surface == nullptr) {
                std::cerr << "unable to get wl_surface";
                return nullptr;
            }
            handle = wl_egl_window_create(surface, w, h);
            if (handle == nullptr) {
                std::cerr << "unable to create wl_egl_window\n";
                return nullptr;
            }
            glfwSetWindowUserPointer(window, handle);
        }
#else
        handle = _GET_WINDOW(window);
#endif
#ifdef KERNEL_DARWIN
        handle = metalLayer(handle);
#endif
        return handle;
    }

    u0* displayHandle()
    {
        ifast platform = glfwGetPlatform();
        if (platform == GLFW_PLATFORM_X11)
            return glfwGetX11Display();
        if (platform == GLFW_PLATFORM_WAYLAND)
            return glfwGetWaylandDisplay();
        return nullptr;
    }
}
