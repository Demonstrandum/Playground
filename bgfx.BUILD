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
    name = "imgui-headers",
    hdrs = glob([
        "3rdparty/dear-imgui/**/*.h",
    ]),
    includes = ["3rdparty"],
    strip_include_prefix = "3rdparty/dear-imgui",
)

cc_library(
    name = "stb-headers",
    hdrs = glob([
        "3rdparty/stb/*.h",
    ]),
    includes = ["3rdparty"],
    strip_include_prefix = "3rdparty",
)

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
    defines = ["IMGUI_ENABLE_FREETYPE"] + select({
        "@bazel_tools//src/conditions:darwin": [
            "BGFX_CONFIG_RENDERER_VULKAN=0",
            "BGFX_CONFIG_RENDERER_METAL=1",
        ],
        "@bazel_tools//src/conditions:linux": [
            "BGFX_CONFIG_RENDERER_OPENGL=0",
            "BGFX_CONFIG_RENDERER_OPENGLES=20",
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
        "@bazel_tools//src/conditions:linux": [
            "3rdparty/directx-headers/include/directx",
            "3rdparty/directx-headers/include/wsl/stubs",
        ],
        "//conditions:default": [],
    }),
    deps = [
        "@imgui//:freetype",
        "@bimg//:bimg",
    ],
    linkopts = select({
        "@bazel_tools//src/conditions:linux": ['-lX11', '-lEGL', '-lGL', '-pthread'],
        "//conditions:default": [],
    }),
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
        "//:bgfx_src",
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
        "@bazel_tools//src/conditions:darwin": ["//:bgfx_objc"],
        "//conditions:default": ["//:bgfx_src"],
    }),
)

cc_library(
    name = "fcpp",
    srcs = glob(["3rdparty/fcpp/*.c"],
        exclude = ["**/usecpp.c"],
    ),
    hdrs = glob(["3rdparty/fcpp/*.h"]),
    includes = ["3rdparty/fcpp"],
)

cc_library(
    name = "glsl-optimizer",
    srcs = glob([
        "3rdparty/glsl-optimizer/src/util/**/*.c*",
        "3rdparty/glsl-optimizer/src/mesa/**/*.c*",
        "3rdparty/glsl-optimizer/src/glsl/**/*.c*",
    ], exclude = [
        "**/src/glsl/glcpp/glcpp.c",
        "**/src/glsl/glcpp/tests/**",
        "**/src/glsl/glcpp/**/*.l",
        "**/src/glsl/glcpp/**/*.y",
        "**/src/glsl/ir_set_program_inouts.cpp",
        "**/src/glsl/main.cpp",
        "**/src/glsl/builtin_stubs.cpp",
    ]),
    hdrs = glob([
        "3rdparty/glsl-optimizer/src/util/**/*.h*",
        "3rdparty/glsl-optimizer/src/mesa/**/*.h*",
        "3rdparty/glsl-optimizer/src/glsl/**/*.h*",
        "3rdparty/glsl-optimizer/include/**/*.h",
    ]),
    includes = [
        "3rdparty/glsl-optimizer/include",
        "3rdparty/glsl-optimizer/src/glsl",
        "3rdparty/glsl-optimizer/src/mesa",
        "3rdparty/glsl-optimizer/src",
    ],
    defines = select({
        "@bazel_tools//src/conditions:windows_msvc": [
            "__STDC__",
            "__STDC_VERSION__=199901L",
            "strdup=_strdup",
            "alloca=_alloca",
            "isascii=__isascii",
        ],
        "//conditions:default": [],
    }),
    copts = select({
        "@bazel_tools//src/conditions:windows": [],
        "//conditions:default": ['-fno-strict-aliasing'],  # unix: has bugs if strict aliasing is used
    })
)

cc_library(
    name = "spirv-headers",
    hdrs = glob([
        "3rdparty/spirv-headers/include/**/*.h*",  # .h, .hpp, .hpp11, &c.
    ]),
    includes = ["3rdparty/spirv-headers/include"],
)

cc_library(
    name = "spirv-tools",
    srcs = glob(["3rdparty/spirv-tools/source/**/*.cpp"]),
    hdrs = glob([
        "3rdparty/spirv-tools/include/**/*.hpp",
        "3rdparty/spirv-tools/include/**/*.inc",
        "3rdparty/spirv-tools/include/**/*.h",
        "3rdparty/spirv-tools/source/**/*.h",
    ]),
    includes = [
        "3rdparty/spirv-tools/include",
        "3rdparty/spirv-tools/include/generated",
        "3rdparty/spirv-tools/source",
        "3rdparty/spirv-tools",
    ],
    deps = ["//:spirv-headers"],
)

cc_library(
    name = "spirv-cross",
    srcs = glob(["3rdparty/spirv-cross/*.cpp"]),
    hdrs = glob([
        "3rdparty/spirv-cross/**/*.hpp",
        "3rdparty/spirv-cross/**/*.h",
    ]),
    includes = [
        "3rdparty/spirv-cross/include",
        "3rdparty/spirv-cross",
    ],
)

cc_library(
    name = "glslang",
    srcs = glob([
        "3rdparty/glslang/glslang/**/*.c",
        "3rdparty/glslang/glslang/**/*.cpp",
        "3rdparty/glslang/SPIRV/**/*.cpp",
        "3rdparty/glslang/OGLCompilersDLL/*.cpp",
        #"3rdparty/glslang/StandAlone/*.cpp",
    ], exclude = [
        "**/ossource.cpp",
    ]) + select({
        "@bazel_tools//src/conditions:windows": [
            "3rdparty/glslang/glslang/OSDependent/Windows/ossource.cpp"
        ],
        "//conditions:default": [
            "3rdparty/glslang/glslang/OSDependent/Unix/ossource.cpp"
        ],
    }),
    hdrs = glob([
        "3rdparty/glslang/*.h",
        "3rdparty/glslang/glslang/**/*.h",
        "3rdparty/glslang/glslang/**/*.hpp",
        "3rdparty/glslang/SPIRV/**/*.hpp",
        "3rdparty/glslang/SPIRV/**/*.h",
        "3rdparty/glslang/OGLCompilersDLL/*.h",
        "3rdparty/glslang/StandAlone/*.h",
    ]),
    includes = [
        "3rdparty/glslang/glslang/Include",
        "3rdparty/glslang/glslang/Public",
        "3rdparty/glslang/SPIRV",
        "3rdparty/glslang",
        "3rdparty",
    ],
    defines = [
        "ENABLE_OPT=1",
        "ENABLE_HLSL=1",
    ],
    deps = ["//:spirv-tools"],
)

cc_library(
    name = "webgpu",
    srcs = ["3rdparty/webgpu/webgpu_cpp.cpp"],
    hdrs = glob(["3rdparty/webgpu/include/webgpu/*.h"]),
    includes = ["3rdparty/webgpu/include"],
)

cc_library(
    name = "shaderc_src",
    srcs = glob(["tools/shaderc/*.cpp"]),
    hdrs = glob(["tools/shaderc/*.h"]),
    deps = [
        "//:bgfx",
        "//:fcpp",
        "//:glslang",
        "//:glsl-optimizer",
        "//:spirv-cross",
        "//:webgpu",
    ],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ['-DEFAULTLIB:psapi.lib'],
        "//conditions:default": ['-lpthread'],  # unix
    }),
    copts = COPT_CXX,
)

# compile bgfx shaders with:
#   shaderc -f <in.sc> -o <out.*> --type <[v]ertex/[f]ragment/[c]ompute> -i <include-dir>
# see `shaderc -h` for more options.
cc_binary(
    name = "shaderc",
    deps = ["//:shaderc_src"],
    copts = COPT_CXX,
)

filegroup(
    name = "shader-include",
    srcs = ["src"],
)
