package(default_visibility = ["//visibility:public"])

cc_library(
    name = "linalg",
    hdrs = ["linalg.h"],
    linkopts = select({
        "@bazel_tools//src/conditions:windows_msvc": [],
        "//conditions:default": ["-lm"],
    })
)
