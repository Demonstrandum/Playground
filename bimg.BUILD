load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

srcs = [
    "src/**/*.cpp",
    "3rdparty/libsquish/*.cpp",
    "3rdparty/libsquish/*.h",
    "3rdparty/libsquish/*.inl",
    "3rdparty/edtaa3/*.cpp",
    "3rdparty/edtaa3/*.h",
    "3rdparty/etc1/*.cpp",
    "3rdparty/etc1/*.h",
    "3rdparty/etc2/*.cpp",
    "3rdparty/etc2/*.hpp",
    "3rdparty/nvtt/**/*.cpp",
    "3rdparty/nvtt/**/*.h",
    "3rdparty/nvtt/**/*.inl",
    "3rdparty/pvrtc/*.cpp",
    "3rdparty/pvrtc/*.h",
]

cc_library(
	name = "iqa",
	srcs = glob([
		"3rdparty/iqa/include/*.h",
		"3rdparty/iqa/source/*.c",
	]),
	hdrs = glob([
		"3rdparty/iqa/include/*.h",
	]),
	includes = ["3rdparty/iqa/include"],
)

cc_library(
	name = "tinyexr",
	srcs = glob([
		"3rdparty/tinyexr/deps/**/*.c",
		"3rdparty/tinyexr/*.h",
	]),
	hdrs = glob([
		"3rdparty/tinyexr/*.h",
		"3rdparty/tinyexr/deps/**/*.h",
	]),
)

cc_library(
	name = "astc-encoder",
	hdrs = glob([
		"3rdparty/astc-encoder/source/*.h",
		"3rdparty/astc-encoder/include/*.h",
	]),
	srcs = glob(["3rdparty/astc-encoder/source/*.cpp"]),
	includes = ["3rdparty/astc-encoder/include"],
	copts = ["-std=c++20"],
)

cc_library(
    name = "astc-codec",
    hdrs = [
		"include/bimg/bimg.h",
		"include/bimg/decode.h",
		"include/bimg/encode.h",
	],
    includes = ["include"],
	deps = [":astc-encoder"],
	copts = ["-std=c++20"],
)

cc_library(
    name = "bimg",
    srcs = glob(srcs),
    hdrs = glob([
        "**/*.h",
        "src/bimg_p.*",
        "3rdparty/lodepng/*.cpp",
		"3rdparty/tinyexr/deps/miniz/*.h",
    ]),
    includes = [
        "3rdparty",
        "3rdparty/iqa/include",
        "3rdparty/nvtt",
        "3rdparty/tinyexr/deps/miniz",
        "include",
    ],
    deps = [
		":iqa",
		":tinyexr",
        ":astc-codec",
        "@bx//:bx",
    ],
    visibility = ["//visibility:public"],
	copts = ["-std=c++20"],
)
