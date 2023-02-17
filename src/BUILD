objc_library(
	name = "objc-src",
	srcs = ["metal.mm"],
	hdrs = [],
	copts = ["-fno-objc-arc"],
)

cc_library(
	name = "playground-src",
	srcs = [
		"native.cpp"
	],
	hdrs = [
		"common.hpp",
		"native.hpp",
	],
	defines = ["PLAYGROUND_SRC"] + select({
		"@bazel_tools//src/conditions:windows": ["KERNEL_WIN32"],
		"@bazel_tools//src/conditions:linux":   ["KERNEL_LINUX"],
		"@bazel_tools//src/conditions:darwin":  ["KERNEL_DARWIN"],
	}),
	deps = [
		"@glfw//:glfw",
		"@bgfx//:bgfx",
	] + select({
		"@bazel_tools//src/conditions:darwin": [":objc-src"],
		"//conditions:default": [],
	}),
	copts = ["-std=c++20"],
)

cc_binary(
	name = "playground",
	srcs = ["main.cpp"],
	deps = [
		":playground-src",
	],
	copts = ["-std=c++20"],
)
