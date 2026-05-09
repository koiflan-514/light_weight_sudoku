#pragma once
#include "sudoku_core.h"
#include <GLFW/glfw3.h>

class UIRenderer {
private:
    SudokuCore* core;
    GLFWwindow* window;
    float cell_size = 50.0f;
    float padding = 8.0f;
    
    void drawCell(int8_t row, int8_t col);
    void drawGrid();
    void drawNumberPad();
    void drawControls();
    
public:
    UIRenderer(SudokuCore* core_ptr, GLFWwindow* win) : core(core_ptr), window(win) {}
    ~UIRenderer() = default;
    
    void render();
    void handleInput();
    void setCellSize(float size);
};
