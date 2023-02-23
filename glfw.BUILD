HDRS = [
	"include/GLFW/glfw3.h",
	"include/GLFW/glfw3native.h",
	"src/internal.h",
	"src/mappings.h",
	"src/xkb_unicode.h",
	"src/platform.h",
	"src/null_platform.h",
	"src/null_joystick.h",
]

SRCS = [
	"src/context.c",
	"src/egl_context.c",
	"src/init.c",
	"src/input.c",
	"src/osmesa_context.c",
	"src/platform.c",
	"src/monitor.c",
	"src/null_init.c",
	"src/null_joystick.c",
	"src/null_monitor.c",
	"src/null_window.c",
    "src/vulkan.c",
	"src/window.c",
	"src/xkb_unicode.c",
]
DEFINES = ["GLFW_INVALID_CODEPOINT"]

WIN32_DEFINES = [
    "_GLFW_WIN32",
]

WIN32_HDRS = [
    "src/win32_joystick.h",
    "src/win32_platform.h",
    "src/win32_time.h",
    "src/win32_thread.h",
]

WIN32_SRCS = [
    "src/win32_init.c",
    "src/win32_joystick.c",
    "src/win32_module.c",
    "src/win32_monitor.c",
    "src/win32_thread.c",
    "src/win32_time.c",
    "src/win32_window.c",
    "src/wgl_context.c",
]

WIN32_LINKOPTS = [
    "-DEFAULTLIB:user32.lib",
    "-DEFAULTLIB:gdi32.lib",
    "-DEFAULTLIB:shell32.lib",
]

LINUX_DEFINES = [
    "_GLFW_HAS_XF86VM",
    "_GLFW_X11",
	"_GLFW_WAYLAND",
]

LINUX_HDRS = [
    "src/glx_context.h",
    "src/linux_joystick.h",
    "src/posix_thread.h",
    "src/posix_time.h",
    "src/x11_platform.h",
    "src/wl_platform.h",
]

LINUX_SRCS = [
    "src/glx_context.c",
    "src/linux_joystick.c",
    "src/posix_module.c",
    "src/posix_thread.c",
    "src/posix_time.c",
    "src/x11_init.c",
    "src/x11_monitor.c",
    "src/x11_window.c",
	"src/wl_init.c",
    "src/wl_monitor.c",
    "src/wl_window.c",
]

LINUX_LINKOPTS = []

DARWIN_DEFINES = [
	"_GLFW_COCOA",
]
DARWIN_HDRS = [
	"src/cocoa_platform.h",
	"src/cocoa_joystick.h",
	"src/cocoa_time.h",
	"src/posix_thread.h",
	"src/posix_time.h",
]
DARWIN_SRCS = [
	"src/cocoa_time.c",
    "src/posix_module.c",
	"src/posix_thread.c",
	"src/posix_time.c",
]
DARWIN_OBJC_SRCS = [
	"src/cocoa_init.m",
	"src/cocoa_joystick.m",
	"src/cocoa_monitor.m",
	"src/cocoa_window.m",
	"src/nsgl_context.m",
]
DARWIN_LINKOPTS = []

objc_library(
	name = "glfw_objc",
	hdrs = HDRS + DARWIN_HDRS,
	srcs = DARWIN_OBJC_SRCS,
	defines = DARWIN_DEFINES,
	copts = ["-fno-objc-arc"],
)

cc_library(
    name = "glfw_src",
    hdrs = HDRS + select({
		"@bazel_tools//src/conditions:windows": WIN32_HDRS,
		"@bazel_tools//src/conditions:linux": LINUX_HDRS,
		"@bazel_tools//src/conditions:darwin": DARWIN_HDRS,
	}),
    srcs = SRCS + select({
		"@bazel_tools//src/conditions:windows": WIN32_SRCS,
		"@bazel_tools//src/conditions:linux": LINUX_SRCS,
		"@bazel_tools//src/conditions:darwin": DARWIN_SRCS,
	}),
	includes = ["include"],
    defines = DEFINES + select({
		"@bazel_tools//src/conditions:windows": WIN32_DEFINES,
		"@bazel_tools//src/conditions:linux": LINUX_DEFINES,
		"@bazel_tools//src/conditions:darwin": DARWIN_DEFINES,
	}),
	deps = select({
		"@bazel_tools//src/conditions:darwin": [":glfw_objc"],
		"//conditions:default": [],
	}),
	copts = [],
)

cc_library(
    name = "glfw",
    hdrs = [
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
    ],
	includes = ["include"],
    linkopts = select({
		"@bazel_tools//src/conditions:windows": WIN32_LINKOPTS,
		"@bazel_tools//src/conditions:linux": LINUX_LINKOPTS,
		"@bazel_tools//src/conditions:darwin": DARWIN_LINKOPTS,
	}),
    deps = [":glfw_src"],
	strip_include_prefix = "include",
    visibility = ["//visibility:public"],
	copts = [],
)
