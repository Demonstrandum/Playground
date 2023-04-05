load("@//:configuration.bzl", "COPT_CXX")

cc_binary(
    name = "playground-example",
    srcs = ["example.cpp"],
    includes = ["include"],
    deps = ["//src:playground", "//assets:font", "//assets:texture_shaders"],
    copts = COPT_CXX,
)
