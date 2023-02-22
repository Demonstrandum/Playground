package(default_visibility = ["//visibility:public"])

objc_library(
	name = "objc-src",
	srcs = ["metal.mm"],
	hdrs = [],
	includes = [],
	copts = ["-fno-objc-arc"],
)

print("//include:headers")

cc_library(
	name = "playground",
	srcs = [
		"native.cpp"
	],
	hdrs = ["//include:headers"] + glob([
		'*.hpp', '*.h',
	]),
	strip_include_prefix = "/include/playground",
	#includes = ["//:include/folder"],
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
