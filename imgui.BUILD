# We actually just use the ImGui already provided by bgfx.
# We only use this repository to build in the freetype support.

package(default_visibility = ["//visibility:public"])

cc_library(
	name = "freetype",
	srcs = ["misc/freetype/imgui_freetype.cpp"],
	hdrs = ["misc/freetype/imgui_freetype.h"],
	defines = ["IMGUI_ENABLE_FREETYPE"],
	deps = [
		"@freetype2//:freetype",
		"@bgfx//:imgui-headers",
		"@bgfx//:stb-headers",
	],
)
