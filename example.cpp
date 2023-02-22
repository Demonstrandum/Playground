//#include <playground/common.h>
#include <playground/native.h>

#include <imgui/imgui.h>
#include <playground/theme.h>

#include <bgfx/bgfx.h>
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

struct WindowState {
	GLFWwindow* window;
	struct {
		f64 x, y;
		u16 button;
		i32 scroll;
	} mouse = { 0 };
	u32 width = 0, height = 0;
	u32 frameWidth = 0, frameHeight = 0;
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
				case GLFW_MOUSE_BUTTON_LEFT:   { bstate &= !l; } break;
				case GLFW_MOUSE_BUTTON_MIDDLE: { bstate &= !m; } break;
				case GLFW_MOUSE_BUTTON_RIGHT:  { bstate &= !r; } break;
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
	}
};

static u0 frameLoop(WindowState& state)
{
	state.registerCallbacks();
	state.update();
	state.reset();

	f32 rgba[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	u32 frame = 0;
	while (!state.shouldClose()) {
		state.tick();
		never {
			const u32 r = (u32)(rgba[0] * 255.0f);
			const u32 g = (u32)(rgba[1] * 255.0f);
			const u32 b = (u32)(rgba[2] * 255.0f);
			const u32 a = (u32)(rgba[3] * 255.0f);
			const u32 clearColor = (r << 24) | (g << 16) | (b << 8) | a;
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, clearColor, 1.0f, 0);
		}
		/* drawing */
		imguiBeginFrame((i32)state.mouse.x, (i32)state.mouse.y,
			state.mouse.button, state.mouse.scroll,
			state.frameWidth, state.frameHeight);
		{
			ImGui::Begin("Little Guy");
			ImGui::Button("Hello");
			ImGui::Text("Hello, World!");
			ImGui::End();
		}
		{
			ImGui::Begin("Sliders");
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
}

ierr main(i32 argc, char** argv)
{
	const u32 win_w = 1000;
	const u32 win_h = 700;

	bool ok = glfwInit();
	if (!ok) {
		std::cerr << "failed to init glfw" << std::endl;
		return FAIL;
	}
	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *window = glfwCreateWindow(
		win_w, win_h,
		"BGFX Window",
		nullptr, nullptr
	);
	if (window == nullptr) {
		std::cerr << "failed to create window" << std::endl;
		return FAIL;
	}

	WindowState state{ .window = window };
	state.update();

	bgfx::PlatformData pd;
	pd.ndt = nullptr;
	pd.nwh = native::windowHandle(window);

	bgfx::Init init;
	init.type = bgfx::RendererType::Count;
	init.resolution.width = state.frameWidth;
	init.resolution.height = state.frameHeight;
	init.resolution.reset = RESETS;
	init.platformData = pd;

	bgfx::init(init);

	/* bgfx setup */
	bgfx::setDebug(BGFX_DEBUG_TEXT | 0 & BGFX_DEBUG_STATS);
	bgfx::setViewRect(0, 0, 0, state.frameWidth, state.frameHeight);
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, CLEAR_COLOR, 1.0f, 0);
	bgfx::touch(0);

	/* setup imgui */
	const f32 dpi = 1.5f;
	imguiCreate(20.0f * dpi);
    ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	setStyle();
	style.ScaleAllSizes(dpi);

	/* run main loop */
	frameLoop(state);

	imguiDestroy();
	bgfx::shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();

	return OK;
}
