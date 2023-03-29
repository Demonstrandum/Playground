load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

# Updated as of 2023-03-27
new_git_repository(
    name = "glfw",
    remote = "https://github.com/glfw/glfw.git",
    commit = "fbdb53b9ca457ab01675e20d9127cb62d8db88b8",
    build_file = "@//:glfw.BUILD",
)

# Updated as of 2023-02-21
new_git_repository(
    name = "freetype2",
    remote = "https://gitlab.freedesktop.org/freetype/freetype.git",
    commit = "3f01161ff22c84c371b6dc3b5e0351e0d6e8e771",
    build_file = "@//:freetype2.BUILD",
)

# Updated as of 2023-02-23
new_git_repository(
    name = "imgui",
    remote = "https://github.com/ocornut/imgui.git",
    commit = "2496b973f931186a43c723eda57baa092df6be61",
    build_file = "@//:imgui.BUILD",
)

# Updated as of 2023-02-27
new_git_repository(
    name = "bx",
    remote = "https://github.com/bkaradzic/bx.git",
    commit = "4e67e34c4b6fb1260459c57f93bc46f164ff22c3",
    build_file = "@//:bx.BUILD",
)

# Updated as of 2023-01-14
new_git_repository(
    name = "bimg",
    remote = "https://github.com/bkaradzic/bimg.git",
    commit = "7afa2419254fd466c013a51bdeb0bee3022619c4",
    build_file = "@//:bimg.BUILD",
)

# Updated as of 2023-03-18
new_git_repository(
    name = "bgfx",
    remote = "https://github.com/bkaradzic/bgfx.git",
    commit = "b0bb4fc578cf1e9e6f446ee0c718a2305846b207",
    patch_args = ["-p1"],
    patches = ["@//:bgfx-egl-support.patch"],
    build_file = "@//:bgfx.BUILD",
)

# Updated as of 2020-12-21
new_git_repository(
    name = "linalg",
    remote = "https://github.com/sgorsten/linalg.git",
    commit = "a3e87da35e32b781a4b6c01cdd5efbe7ae51c737",
    build_file = "@//:linalg.BUILD",
)
