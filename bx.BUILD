load("@rules_cc//cc:defs.bzl", "cc_library")
load("@//:configuration.bzl", "COPT_CXX")

package(default_visibility = ["//visibility:public"])

bx_srcs = [
    "src/allocator.cpp",
    "src/bx.cpp",
    "src/commandline.cpp",
    "src/crtnone.cpp",
    "src/debug.cpp",
    "src/dtoa.cpp",
    "src/easing.cpp",
    "src/file.cpp",
    "src/filepath.cpp",
    "src/hash.cpp",
    "src/math.cpp",
    "src/mutex.cpp",
    "src/os.cpp",
    "src/process.cpp",
    "src/semaphore.cpp",
    "src/settings.cpp",
    "src/sort.cpp",
    "src/string.cpp",
    "src/thread.cpp",
    "src/timer.cpp",
    "src/url.cpp",
]

cc_library(
    name = "bx",
    srcs = bx_srcs,
    hdrs = glob([
        "**",
    ]),
    includes = [
        "3rdparty",
        "include",
    ] + select({
        "@bazel_tools//src/conditions:darwin":  ["include/compat/osx"],
        "@bazel_tools//src/conditions:windows": ["include/compat/msvc"],
        "@bazel_tools//src/conditions:linux":   ["include/compat/linux"],
        "@bazel_tools//src/conditions:freebsd": ["include/compat/freebsd"],
        "//conditions:default": [],
    }),
    defines = ["BX_CONFIG_DEBUG", "__STDC_FORMAT_MACROS"],
    deps = [],
    copts = COPT_CXX,
)

