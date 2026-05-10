#pragma once
#include <GLFW/glfw3.h>
#include "sudoku_core.h"
#include "imgui.h"
#include "my_style.h"

enum class AppState {
    START_MENU,
    PLAYING
};

struct CellAnim {
    float pop_scale = 1.0f;
    float hover_fade = 0.0f;
    float select_fade = 0.0f;
    float highlight_fade = 0.0f;
    float ripple_delay = 0.0f;
    float ripple_fade = 0.0f;
};

class UIRenderer {
private:
    SudokuCore* core;
    GLFWwindow* window;

    // 转场状态机变量
    AppState current_state = AppState::START_MENU;
    AppState next_state = AppState::START_MENU;
    float transition_t = 1.0f; // 0.0 (透明/滑出) -> 1.0 (完全显示)

    float cell_size = 50.0f;
    float padding = 8.0f;
    ImVec2 grid_start_pos;
    ImVec2 right_panel_pos;

    CellAnim anims[9][9];
    float win_timer = 0.0f;
    bool was_complete = false;

    void updateLayout();
    void updateAnimations(float dt);
    ImU32 lerpColor(ImU32 col_a, ImU32 col_b, float t);

    void triggerRipple(int target_r, int target_c);
    void inputNumber(int val);

    void drawStartMenu(float alpha, float y_offset);
    void drawGrid(ImDrawList* draw_list, float alpha);
    void drawNumberPad(float alpha);
    void drawControls(float alpha);
    void handleInput();
    void handleWindowDrag();

public:
    UIRenderer(SudokuCore* core_ptr, GLFWwindow* window_ptr) : core(core_ptr), window(window_ptr) {}
    void render();
};