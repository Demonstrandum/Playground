load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

new_git_repository(
    name = "glfw",
    remote = "https://github.com/glfw/glfw.git",
    commit = "8f470597d625ae28758c16b4293dd42d63e8a83a",
    build_file = "@//:glfw.BUILD",
)

new_git_repository(
    name = "bx",
    remote = "https://github.com/bkaradzic/bx.git",
    commit = "fa1411e4aa111c8b004c97660ab31ba1a5287835",
    build_file = "@//:bx.BUILD",
)

new_git_repository(
    name = "bimg",
    remote = "https://github.com/bkaradzic/bimg.git",
    commit = "7afa2419254fd466c013a51bdeb0bee3022619c4",
    build_file = "@//:bimg.BUILD",
)

new_git_repository(
    name = "bgfx",
    remote = "https://github.com/bkaradzic/bgfx.git",
    commit = "b0bb4fc578cf1e9e6f446ee0c718a2305846b207", # Mar 18
    build_file = "@//:bgfx.BUILD",
)
