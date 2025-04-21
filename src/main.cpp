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
#include "mesh.hpp"

#include "bx/math.h"
#include "bx/timer.h"

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

	// Load model.
	Model testModel;
	testModel.loadFromFile("spider.obj");

	// Create vb and ib for each individual mesh in model.
	std::vector<bgfx::VertexBufferHandle> vbs;
	std::vector<bgfx::IndexBufferHandle> ibs;
	for (int i = 0; i < testModel.meshes.size(); i++) {
		vbs.push_back(
			bgfx::createVertexBuffer(
				bgfx::makeRef(testModel.meshes[i].vertexList.data(), 16*testModel.meshes[i].vertexList.size()),
				layout
			)
		);

		ibs.push_back(
			bgfx::createIndexBuffer(
				bgfx::makeRef(testModel.meshes[i].indexList.data(), 2*testModel.meshes[i].indexList.size())
			)
		);
	}

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
		const bx::Vec3 eye = { 0.0f, 0.0f, -500.0f };

		{
			float view[16];
			bx::mtxLookAt(view, eye, at);

			float proj[16];
			bx::mtxProj(proj, 60.0f, static_cast<float>(width)/static_cast<float>(height), 0.1f, 10000.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(kClearView, view, proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(kClearView, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));
		}

		bx::mtxRotateXYZ(testModel.modelMatrix.data(),  time + 0.21f, time + 0.37f, time + 0.50f);

		for (int i = 0; i < vbs.size(); i++) {
			bgfx::setTransform(testModel.modelMatrix.data());

			bgfx::setVertexBuffer(0, vbs[i]);
			bgfx::setIndexBuffer(ibs[i]);

			bgfx::setState(BGFX_STATE_DEFAULT);
			bgfx::submit(kClearView, shaderProgramHandle, 0);
		}

		// Advance to next frame. Process submitted rendering primitives.
		bgfx::frame();
	}

	// Clean up.
	for (auto vb: vbs) {
		bgfx::destroy(vb);
	}

	for (auto ib: ibs) {
		bgfx::destroy(ib);
	}

	bgfx::destroy(shaderProgramHandle);

	bgfx::shutdown();
	glfwTerminate();
	return 0;
}