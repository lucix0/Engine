#include <iostream>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "window.hpp"

int main(int argc, char **argv) {


	Shader objectShader("resources/shaders/ObjectVertex.glsl", "resources/shaders/ObjectFragment.glsl");

	stbi_set_flip_vertically_on_load(true);

	Model testModel("resources/models/backpack.obj");

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(win.GetWindowHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	while (!glfwWindowShouldClose(win.GetWindowHandle())) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objectShader.use();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.setMat4("model", model);

		objectShader.setVec3("viewPos", camera.Position);
		objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		objectShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		objectShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		objectShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		testModel.Draw(objectShader);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool demo = true;
		ImGui::ShowDemoWindow(&demo);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(win.GetWindowHandle());
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}