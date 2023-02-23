load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "objc_library")
load("@//:configuration.bzl", "COPT_CXX")

package(default_visibility = ["//visibility:public"])

srcs = [
    "src/bgfx.cpp",
    "src/debug_renderdoc.cpp",
    "src/dxgi.cpp",
    "src/glcontext_egl.cpp",
    "src/glcontext_wgl.cpp",
    "src/glcontext_html5.cpp",
    "src/nvapi.cpp",
    "src/renderer_agc.cpp",
    "src/renderer_d3d11.cpp",
    "src/renderer_d3d12.cpp",
    "src/renderer_d3d9.cpp",
    "src/renderer_gl.cpp",
    "src/renderer_gnm.cpp",
    "src/renderer_noop.cpp",
    "src/renderer_nvn.cpp",
    "src/renderer_vk.cpp",
    "src/renderer_webgpu.cpp",
    "src/shader.cpp",
    "src/shader_dx9bc.cpp",
    "src/shader_dxbc.cpp",
    "src/shader_spirv.cpp",
    "src/topology.cpp",
    "src/vertexlayout.cpp",
]

cc_library(
    name = "bgfx_src",
    srcs = srcs
		+ select({
			"@bazel_tools//src/conditions:linux": ["src/glcontext_glx.cpp"],
			"//conditions:default": [],
		})
		+ glob([
			"examples/common/bgfx_utils.cpp",
			"examples/common/imgui/*.cpp",
			"3rdparty/dear-imgui/*.cpp",
			"3rdparty/meshoptimizer/src/*.cpp",
		]),
    hdrs = glob([
        "**/*.h",
        "**/*.inl",
    ]),
    defines = select({
		"@bazel_tools//src/conditions:darwin": [
			"BGFX_CONFIG_RENDERER_VULKAN=0",
			"BGFX_CONFIG_RENDERER_METAL=1",
		],
		"//conditions:default": [],
	}),
    includes = [
        "3rdparty",
        "3rdparty/khronos",
		"examples/common",
        "include",
    ] + select({
		"@bazel_tools//src/conditions:windows": ["3rdparty/directx-headers/include/directx"],
		"//conditions:default": [],
	}),
    deps = [
		"@bimg//:bimg",
    ],
	copts = COPT_CXX,
)

objc_library(
    name = "bgfx_objc",
    srcs = [
		"src/glcontext_eagl.mm",
		"src/glcontext_nsgl.mm",
		"src/renderer_mtl.mm",
    ],
    hdrs = glob([
        "**/*.h",
        "**/*.inl",
    ]),
    defines = [],
    includes = ["include"],
	enable_modules = True,
	sdk_frameworks = [
		"Foundation",
		"Metal",
		"AppKit",
		"IOKit",
		"QuartzCore",
	],
    copts = COPT_CXX + ["-fno-objc-arc"],
    deps = [
        ":bgfx_src",
    ],
)

cc_library(
	name = "bgfx",
	hdrs = glob([
		"**/*.h",
		"**/*.inl",
    ]),
	includes = [
        "3rdparty",
        "3rdparty/khronos",
        "include",
    ],
    visibility = ["//visibility:public"],
	copts = COPT_CXX,
	deps = select({
		"@bazel_tools//src/conditions:darwin": [":bgfx_objc"],
		"//conditions:default": [":bgfx_src"],
	}),
)
