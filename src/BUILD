package(default_visibility = ["//visibility:public"])
load("@//:configuration.bzl", "COPT_CXX")

objc_library(
    name = "objc-src",
    srcs = ["metal.mm"],
    hdrs = [],
    includes = [],
    copts = ["-fno-objc-arc"],
)

cc_library(
    name = "playground",
    srcs = [
        "native.cpp"
    ],
    hdrs = ["@//include:headers"] + glob([
        '*.hpp', '*.h',
    ]),
    strip_include_prefix = "/include/playground",
    includes = ["@//include:folder"],
    defines = ["PLAYGROUND_SRC"] + select({
        "@bazel_tools//src/conditions:windows": ["KERNEL_WIN32"],
        "@bazel_tools//src/conditions:linux":   ["KERNEL_LINUX"],
        "@bazel_tools//src/conditions:darwin":  ["KERNEL_DARWIN"],
        "//conditions:default": [],
    }),
    deps = [
        "@glfw//:glfw",
        "@bgfx//:bgfx",
    ] + select({
        "@bazel_tools//src/conditions:darwin": [":objc-src"],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@bazel_tools//src/conditions:linux":   ['-lwayland-egl', '-lwayland-client'],
        "//conditions:default": [],
    }),
    copts = COPT_CXX,
)
