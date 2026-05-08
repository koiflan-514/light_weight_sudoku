#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <gl/GL.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "sudoku_core.h"
int main() {
	// 初始化 GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// 配置 OpenGL 版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Sudoku", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// 将窗口的 OpenGL 上下文设为当前
	glfwMakeContextCurrent(window);

	// 设置垂直同步
	glfwSwapInterval(1);

	// 初始化 Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// 设置样式
	ImGui::StyleColorsDark();

	// 初始化平台和渲染器后台
	const char* glsl_version = "#version 130";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// 主循环
	while (!glfwWindowShouldClose(window)) {
		// 处理事件
		glfwPollEvents();
		
		// 开始 ImGui 新帧
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();


		// 渲染 ImGui 内容
		ImGui::Render();

		// 设置清空颜色
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// 提交 ImGui 渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 交换前后缓冲区
		glfwSwapBuffers(window);
	}

	// ImGui 资源清理
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// GLFW 资源清理
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
