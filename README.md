# Playground

Getting something drawing on the screen is a pain.

This is a simple starting point for a windowed cross-platform graphical application.

- Windowing: GLFW
- Graphics: BGFX (API over OpenGL, DirectX, Vulkan, Metal, &c.)
- Debug GUI: ImGui

See `example.cpp` for short example usage.

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

- On Windows you will need to Visual Studio (yup.) or at least the Windows {10,11} SDK to use the MSVC compiler. After Bazel is installed (recommend through Chocolatey, `choco install bazel`), you need to modify your users environment variables to have `BAZEL_VC` point to your `VC` folder containing your compiler tools.
- macOS and *nix environments are more straightforward. Just install Bazel with `brew install bazel` or through your native package manager. Make sure Java version ≥11 is available/default.

![macos-screenshot](https://user-images.githubusercontent.com/26842759/220960536-9875f36d-09df-491a-bb5c-ed5e35119799.png)
<!-- ![windows-screenshot](https://user-images.githubusercontent.com/26842759/220960544-1271bf7a-1d1c-45ac-9f2e-14993e81219d.png) -->

## TODO

- [x] macOS working (checked on `arm/aarch64`)
- [x] Windows working (checked on `arm/aarch64`)
- [ ] Linux working (not checked)
- [ ] *BSD working (not checked)
- [x] macOS native window customisation
- [ ] Windows native window customisation
- [ ] Linux Qt/Gtk native window customisation
