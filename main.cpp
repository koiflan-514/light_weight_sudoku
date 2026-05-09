#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <filesystem>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "sudoku_core.h"
#include "ui_renderer.h"
#include "color_rgba.h"
#include "my_style.h"

class Process {
private:
    GLFWwindow* window;
    SudokuCore sudoku_core;
    UIRenderer* ui_renderer = nullptr;
public:
    Process() : window(nullptr) {}
    ~Process() { delete ui_renderer; }
    bool initAll();
    void running();
    void cleanUp();
};

int main() {
    Process mainProcess;
    if (!mainProcess.initAll()) {
        std::cerr << "Failed to initialize all" << std::endl;
        return -1;
    }
    mainProcess.running();
    mainProcess.cleanUp();
    return 0;
}

bool Process::initAll() {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Win11 Sudoku", nullptr, nullptr);
    if (!window) { glfwTerminate(); return false; }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // 优化：安全加载字体，避免崩溃
    const char* fontPath = "C:/Windows/Fonts/msyh.ttc";
    if (std::filesystem::exists(fontPath)) {
        ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, 16.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
        if (font) io.FontDefault = font;
    }

    UI::apply_win11_dark_theme();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ui_renderer = new UIRenderer(&sudoku_core);
    sudoku_core.loadPuzzle(Difficulty::NORMAL);
    return true;
}

void Process::running() {
    while (!glfwWindowShouldClose(window)) {
        glfwWaitEventsTimeout(0.016);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        ImGui::Begin("Win11 Sudoku", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ui_renderer->handleInput(); // 优先处理输入
        ui_renderer->render();

        ImGui::End();

        ImGui::Render();
        glClearColor(rgba::to_float(31), rgba::to_float(31), rgba::to_float(31), rgba::to_float(255));
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

void Process::cleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}