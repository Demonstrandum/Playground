load("@//:configuration.bzl", "COPT_CXX")

cc_binary(
	name = "playground-example",
	srcs = ["example.cpp"],
	includes = ["include"],
	deps = ["//src:playground"],
	copts = COPT_CXX,
)
