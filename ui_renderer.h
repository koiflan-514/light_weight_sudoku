#pragma once
#include "sudoku_core.h"

class UIRenderer {
private:
    SudokuCore* core;
    float cell_size = 50.0f;
    float padding = 8.0f;

    void drawCell(int8_t row, int8_t col);
    void drawGrid();
    void drawNumberPad();
    void drawControls();

public:
    UIRenderer(SudokuCore* core_ptr) : core(core_ptr) {}

    void render();
    void handleInput();
    void setCellSize(float size) { cell_size = size; }
};