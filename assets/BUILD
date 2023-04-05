load("@//:configuration.bzl", "COPT_CXX")
load("@//:shaderc.bzl", 'shaderc_bin')

package(default_visibility = ["//visibility:public"])

# TODO: call `xxd -i in > out` from Bazel.
cc_library(
    name = "font",
    hdrs = ["inter.ttf.h"],
)

shaderc_bin(
    name = "texture_shaders",
    srcs = ["//assets:vs_texture.sc", "//assets:fs_texture.sc"],
    varying_def = "//assets:varying.def.sc"
)

