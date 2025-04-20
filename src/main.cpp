#include <stdio.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include "util.hpp"
#include "bx/math.h"
#include "bx/timer.h"

struct Vertex {
	float x, y, z;
	uint32_t color;
};

static Vertex cubeVertices[] = {
	{-1.0f,  1.0f,  1.0f, 0x00000000, },
	{ 1.0f,  1.0f,  1.0f, 0xff000000, },
	{-1.0f, -1.0f,  1.0f, 0x00ff0000, },
	{ 1.0f, -1.0f,  1.0f, 0x0000ff00, },
	{-1.0f,  1.0f, -1.0f, 0x000000ff, },
	{ 1.0f,  1.0f, -1.0f, 0xffff0000, },
	{-1.0f, -1.0f, -1.0f, 0x0000ffff, },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff, },
};

static const uint16_t cubeTriList[] = {
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

static void glfw_errorCallback(int error, const char *description) {
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {  }

int main(int argc, char **argv)
{
	// Create a GLFW window without an OpenGL context.
	glfwSetErrorCallback(glfw_errorCallback);
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *window = glfwCreateWindow(1024, 768, "Engine", nullptr, nullptr);
	if (!window)
		return 1;
	glfwSetKeyCallback(window, glfw_keyCallback);

	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
	bgfx::renderFrame();
	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	init.platformData.ndt = glfwGetX11Display();
	init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
	init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(window);
#endif

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	init.resolution.width = static_cast<uint32_t>(width);
	init.resolution.height = static_cast<uint32_t>(height);
	init.resolution.reset = BGFX_RESET_VSYNC;

	if (!bgfx::init(init))
		return 1;

	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	constexpr bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xff00ff00);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

	// Create vertex layout.
	bgfx::VertexLayout layout;
	layout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	const bgfx::VertexBufferHandle vb = bgfx::createVertexBuffer(
		bgfx::makeRef(cubeVertices, sizeof(cubeVertices)),
		layout
	);

	const bgfx::IndexBufferHandle ib = bgfx::createIndexBuffer(
		bgfx::makeRef(cubeTriList, sizeof(cubeTriList))
	);

	// Create Shader Program here...
	const auto shaderProgramHandle = loadProgram("vs.bin", "fs.bin");

	const auto timeOffset = bx::getHPCounter();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Handle window resize.
		int oldWidth = width, oldHeight = height;
		glfwGetWindowSize(window, &width, &height);
		if (width != oldWidth || height != oldHeight) {
			bgfx::reset(static_cast<uint32_t>(width), static_cast<uint32_t>(height), BGFX_RESET_VSYNC);
			bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
		}

		const auto time = static_cast<float>((bx::getHPCounter()-timeOffset)/static_cast<double>(bx::getHPFrequency()));

		const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

		{
			float view[16];
			bx::mtxLookAt(view, eye, at);

			float proj[16];
			bx::mtxProj(proj, 60.0f, static_cast<float>(width)/static_cast<float>(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(kClearView, view, proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(kClearView, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));
		}

		float mtx[16];
		bx::mtxRotateXY(mtx,  time + 0.21f, time + 0.37f);

		bgfx::setTransform(mtx);

		bgfx::setVertexBuffer(0, vb);
		bgfx::setIndexBuffer(ib);

		bgfx::setState(BGFX_STATE_DEFAULT);

		bgfx::submit(kClearView, shaderProgramHandle, 0);

		// Advance to next frame. Process submitted rendering primitives.
		bgfx::frame();
	}

	bgfx::destroy(vb);
	bgfx::destroy(ib);
	bgfx::destroy(shaderProgramHandle);

	bgfx::shutdown();
	glfwTerminate();
	return 0;
}