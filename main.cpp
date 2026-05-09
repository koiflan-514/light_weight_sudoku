// GLAD
#include <glad/glad.h>

// Standard
#include <iostream>

// GLFW
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <gl/GL.h>

// ImGui
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Project Heads
#include "sudoku_core.h"
#include "color_rgba.h"

class process {
private:
	GLFWwindow* window;
public:
	// 初始化
	bool init_all();
	// 运行时
	void running();
	// 资源清理
	void clean_up();
};

int main() {
	
	process main_process{};
	if (!main_process.init_all()) {
		std::cerr << "Failed to initialize all" << std::endl;
		return -1;
	}

	main_process.running();
	
	main_process.clean_up();
	return 0;
}

bool process::init_all() {
	// 初始化 GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	// 配置 OpenGL 版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
	window = glfwCreateWindow(1280, 720, "Sudoku", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// 将窗口的 OpenGL 上下文设为当前
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// 设置垂直同步
	glfwSwapInterval(1);

	// 初始化 Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// 加载中文字体（例如微软雅黑）
	const char* fontPath = "C:/Windows/Fonts/msyh.ttc";  // 请将此文件放在 exe 所在目录
	ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

	if (font) {
		io.FontDefault = font;  // 设置为默认字体
		std::cout << "中文已加载" << std::endl;
	}
	else {
		std::cerr << "警告：无法加载中文字体，中文将显示为问号" << std::endl;
	}

	// 设置样式
	ImGui::StyleColorsDark();

	// 初始化平台和渲染器后台
	const char* glsl_version = "#version 130";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	return true;
}

void process::running() {
	// 主循环
	while (!glfwWindowShouldClose(window)) {
		// 处理事件
		glfwPollEvents();

		// 开始 ImGui 新帧
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		// 1. 创建一个窗口 (如果还没创建)
		ImGui::Begin("控制面板");

		// ----- 常规按钮组 -----
		ImGui::Text("常规按钮：");
		if (ImGui::Button("按钮 A")) {
			// 点击后执行的动作
			std::cout << "按钮 A 被点击" << std::endl;
		}
		ImGui::SameLine();          // 将下一个按钮放在同一行
		if (ImGui::Button("按钮 B")) {
			std::cout << "按钮 B 被点击" << std::endl;
		}
		ImGui::SameLine();
		if (ImGui::Button("按钮 C")) {
			std::cout << "按钮 C 被点击" << std::endl;
		}

		// ----- 禁用按钮（灰色不可点）-----
		ImGui::Text("禁用按钮：");
		ImGui::BeginDisabled();     // 开始禁用区域
		ImGui::Button("不可点击的按钮");
		ImGui::EndDisabled();

		// ----- 自定义大小的按钮 -----
		ImGui::Text("不同尺寸：");
		ImGui::Button("小", ImVec2(40, 0));
		ImGui::SameLine();
		ImGui::Button("标准", ImVec2(80, 0));
		ImGui::SameLine();
		ImGui::Button("很长很长的按钮", ImVec2(150, 0));

		// ----- 带颜色的按钮（使用 PushStyleColor）-----
		ImGui::Text("彩色按钮：");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));   // 绿色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		if (ImGui::Button("保存")) {
			std::cout << "保存操作" << std::endl;
		}
		ImGui::PopStyleColor(2);

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));   // 红色
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
		if (ImGui::Button("删除")) {
			std::cout << "删除操作" << std::endl;
		}
		ImGui::PopStyleColor(2);

		// ----- 大按钮占满宽度 -----
		ImGui::Text("占满宽度：");
		if (ImGui::Button("全宽按钮", ImVec2(-1.0f, 0))) {   // 宽度 -1 表示占满可用宽度
			std::cout << "全宽按钮被点击" << std::endl;
		}

		ImGui::End();   // 窗口结束

		// 渲染 ImGui 内容
		ImGui::Render();

		// 设置清空颜色
		glClearColor(rgba::to_float(32), rgba::to_float(32), rgba::to_float(32), rgba::to_float(255));
		glClear(GL_COLOR_BUFFER_BIT);

		// 提交 ImGui 渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 交换前后缓冲区
		glfwSwapBuffers(window);
	}
}

void process::clean_up() {
	// ImGui 资源清理
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// GLFW 资源清理
	glfwDestroyWindow(window);
	glfwTerminate();
}