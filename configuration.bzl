COPT_CXX = select({
	"@bazel_tools//src/conditions:windows": ["/std:c++20", "/Zc:__cplusplus"],
	"//conditions:default": ["-std=c++20"],
})