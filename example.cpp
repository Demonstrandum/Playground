#include <playground/common.h>
#include <playground/native.h>
#include <playground/mesh.h>
#include <playground/text.h>

#include "assets/inter.ttf.h"
#include "assets/vs_texture.bin.h"
#include "assets/fs_texture.bin.h"

#include <imgui/imgui.h>

#include <playground/theme.inl>
#include <playground/imguikey.inl>

#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

#include <GLFW/glfw3.h>

#include <iostream>

static u0 errorCallback(ierr err, const char *msg)
{
    std::cerr << "glfw(" << err << "): " << msg << std::endl;
}

const u32 CLEAR_COLOR = 0xA8'AD'B5'FF;

const u32 RESETS = BGFX_RESET_VSYNC
    | BGFX_RESET_MSAA_X8
    | BGFX_RESET_HIDPI
    | BGFX_RESET_HDR10;

static const bgfx::EmbeddedShader embeddedShaders[] = {
    BGFX_EMBEDDED_SHADER(vs_texture),
    BGFX_EMBEDDED_SHADER(fs_texture),
    BGFX_EMBEDDED_SHADER_END()
};

const ImGuiWindowFlags IMGUI_WINFLAGS = ImGuiWindowFlags_AlwaysAutoResize;

struct WindowState {
    GLFWwindow* window;
    struct {
        f64 x, y;
        u16 button;
        struct { f64 dx, dy; } scroll;
    } mouse = { 0 };
    u32 width = 0, height = 0;
    u32 frameWidth = 0, frameHeight = 0;
    f64 aspect = 1;
    f64 scale = 1;

    bool shouldClose() { return glfwWindowShouldClose(window); }
    u0 tick() { }
    u0 tock() { }
    u0 reset() {
        bgfx::reset(frameWidth, frameHeight, RESETS);
        bgfx::setViewRect(0, 0, 0, frameWidth, frameHeight);
    }
    u0 update()
    {
        ifast w, h, fw, fh;
        glfwGetWindowSize(window, &w, &h);
        glfwGetFramebufferSize(window, &fw, &fh);
        width = w; height = h;
        frameWidth = fw; frameHeight = fh;
        scale = frameWidth / width;
        aspect = frameWidth / frameHeight;
    }
    u0 registerCallbacks()
    {
        static WindowState *const that = this;
        glfwSetWindowRefreshCallback(window, [](GLFWwindow*) {
            that->update();
            that->reset();
            bgfx::frame();
        });
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, ifast, ifast) {
            that->update();
            that->reset();
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow*, ifast button, ifast action, ifast mods) {
            u32 l = IMGUI_MBUT_LEFT;
            u32 m = IMGUI_MBUT_MIDDLE;
            u32 r = IMGUI_MBUT_RIGHT;
            u32 bstate = 0;
            if (action == GLFW_RELEASE) switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:   { bstate &= ~l; } break;
                case GLFW_MOUSE_BUTTON_MIDDLE: { bstate &= ~m; } break;
                case GLFW_MOUSE_BUTTON_RIGHT:  { bstate &= ~r; } break;
            } else switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:   { bstate |= l; } break;
                case GLFW_MOUSE_BUTTON_MIDDLE: { bstate |= m; } break;
                case GLFW_MOUSE_BUTTON_RIGHT:  { bstate |= r;  } break;
            }
            that->mouse.button = bstate;
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow*, f64 xpos, f64 ypos) {
            that->mouse.x = that->scale * xpos;
            that->mouse.y = that->scale * ypos;
        });
        glfwSetScrollCallback(window, [](GLFWwindow*, f64 xoff, f64 yoff) {
            that->mouse.scroll.dx += xoff;
            that->mouse.scroll.dy += yoff;
        });
        glfwSetKeyCallback(window, [](GLFWwindow* win, ifast keycode, ifast scancode, ifast state, ifast) {
            updateKeyModifiers(win);
            keycode = translateUntranslatedKey(keycode, scancode);
            ImGuiIO& io = ImGui::GetIO();
            ImGuiKey key = toImGuiKey(keycode);
            io.AddKeyEvent(key, state == GLFW_PRESS);
        });
        glfwSetCharCallback(window, [](GLFWwindow*, rune codepoint) {
            ImGuiIO& io = ImGui::GetIO();
            io.AddInputCharacter(codepoint);
        });
    }
};

static u0 frameLoop(WindowState& state)
{
    state.registerCallbacks();
    state.update();
    state.reset();

    f32 rgba[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    ImVec4* color = (ImVec4 *)&rgba;
    char inputText[255] = { 0 };

    /* render some text */
    std::vector<umin> font(inter_ttf, std::end(inter_ttf));
    text::FontShaper shaper;
    shaper.loadFontFace(font, 136);
    auto texture = shaper.renderText("You are running Vulkan.");

    /* create shader program */
    bgfx::RendererType::Enum type = bgfx::getRendererType();
    auto program = bgfx::createProgram(
        bgfx::createEmbeddedShader(embeddedShaders, type, "vs_texture"),
        bgfx::createEmbeddedShader(embeddedShaders, type, "fs_texture"),
        true);

    /* mesh test */
    mesh::Mesh q = mesh::quad(vec2(0.0f, 0.0f), texture.rectWidth, texture.rectHeight);
    q.vertices[0].color = mesh::RGBA32(0xff, 0x00, 0x00);
    q.vertices[1].color = mesh::RGBA32(0x00, 0xff, 0x00);
    q.vertices[2].color = mesh::RGBA32(0x00, 0x00, 0xff);
    q.vertices[3].color = mesh::RGBA32(0x00, 0x00, 0x00);
    mesh::Vertex::init();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(q.vertexBuffer(), mesh::Vertex::layout);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(q.indexBuffer());
    auto s_texture = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);

    /* main loop */
    f32 view[16];
    f32 proj[16];
    u32 frame = 0;
    until (state.shouldClose()) {
        state.tick();
        /* camera */
        const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
        const bx::Vec3 eye = { 0.0f, 0.0f, -1.0f };
        bx::mtxLookAt(view, eye, at);
        //bx::mtxProj(proj, 60.0f, state.aspect, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bx::mtxOrtho(proj,
            0.0f, state.frameWidth,
            0.0f, state.frameHeight,
            0.0f, 100.0f, 0.0f,
            bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
        bgfx::setViewRect(0, 0, 0, state.frameWidth, state.frameHeight);

        bgfx::setState(0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_MSAA
            | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
            );
        bgfx::setTexture(0, s_texture, texture.handle);
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);
        bgfx::submit(0, program);

        /* imgui */
        imguiBeginFrame((i32)state.mouse.x, (i32)state.mouse.y,
            state.mouse.button, (i32)state.mouse.scroll.dy,
            state.frameWidth, state.frameHeight);
        {
            ImGui::Begin("What's your name?", nullptr, IMGUI_WINFLAGS);
            if (ImGui::Button("Clear"))
                memset(inputText, '\0', sizeof(inputText));
            if (inputText[0] == '\0')
                ImGui::Text("Write your name below!");
            else
                ImGui::TextColored(*color, "Hello, %s!", inputText);
            ImGui::InputText("Name", inputText, sizeof(inputText));
            ImGui::End();
        }
        //ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
        {
            ImGui::Begin("Sliders", nullptr, IMGUI_WINFLAGS);
            ImGui::Text("Framerate (Hz): %.3f", ImGui::GetIO().Framerate);
            ImGui::SliderFloat("Red",   &rgba[0], 0.0f, 1.0f);
            ImGui::SliderFloat("Green", &rgba[1], 0.0f, 1.0f);
            ImGui::SliderFloat("Blue",  &rgba[2], 0.0f, 1.0f);
            ImGui::ColorEdit3("Colour Pick", rgba);
            ImGui::End();
        }
        imguiEndFrame();
        /* swap */
        bgfx::touch(0);
        bgfx::frame();
        glfwWaitEvents();
        ++frame;
        state.tock();
    }

    bgfx::destroy(program);
    bgfx::destroy(s_texture);
    bgfx::destroy(texture.handle);
    bgfx::destroy(vbh);
    bgfx::destroy(ibh);
}

ierr main(i32 argc, char** argv)
{
    const u32 win_w = 1000;
    const u32 win_h = 700;

    bool ok;
    if (glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
        ok = glfwInit();
        if (!ok) {
            glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);
            ok = glfwInit();
        }
    } else {
        ok = glfwInit();
    }
    if (!ok) {
        std::cerr << "failed to init glfw" << std::endl;
        return FAIL;
    }
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(
        win_w, win_h,
        "Playground Example",
        nullptr, nullptr
    );
    if (window == nullptr) {
        std::cerr << "failed to create window" << std::endl;
        return FAIL;
    }

    WindowState state{ .window = window };
    state.update();

    bgfx::PlatformData pd;
    pd.ndt = native::displayHandle();
    pd.nwh = native::windowHandle(window);
    assert(pd.nwh != nullptr);

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = state.frameWidth;
    init.resolution.height = state.frameHeight;
    init.resolution.reset = RESETS;
    init.platformData = pd;

    bgfx::init(init);

    /* bgfx setup */
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewRect(0, 0, 0, state.frameWidth, state.frameHeight);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, CLEAR_COLOR, 1.0f, 0);
    bgfx::touch(0);

    /* setup imgui */
    const f32 dpi = 1.5f;
    imguiCreate(20.0f * dpi);
    ImGui::StyleColorsDark();

    setStyle();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(dpi);

    /* run main loop */
    frameLoop(state);

    /* clean-up */
    imguiDestroy();
    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return OK;
}
