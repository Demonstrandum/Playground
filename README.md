# Playground

Getting something drawing on the screen is a pain.

This is a simple starting point for a windowed cross-platform graphical application.

- Windowing: GLFW
- Graphics: BGFX (API over OpenGL, DirectX, Vulkan, Metal, &c.)
- Debug GUI: ImGui

See `src/main.cpp` for short example usage.

## Build

Bazel takes care of building everything on every platform.
You don't need to clone `glfw`, `bx`, `bimg`, `bgfx`, `imgui`, &c., and figure out how to compile them,
Bazel takes care of cloning and building (see `WORKSPACE` and `bgfx.BUILD` (&c.) files).

```sh
# build with
bazel build //src:playground
# or run example immediately
bazel run //:playground-example
```

### Notes

Install Bazel, see [bazel.build/install](https://bazel.build/install).

- On Windows you will need to Visual Studio or at least Windows {10,11} SDK to use the MSVC compiler. After Bazel is installed (recommend through Chocolatey), you need to modify your users environment variables to have `BAZEL_VC` point to your `VC` folder conatining your compiler.


![screenshot](https://user-images.githubusercontent.com/26842759/219747206-cf1154bc-2525-41c1-9764-06699d22cb84.png)

## TODO
- [x] macOS working
- [x] Windows working
- [ ] Linux working
- [ ] *BSD working
- [x] macOS native window customisation
- [ ] Windows native window customisation
- [ ] Linux Qt/Gtk native window customisation

