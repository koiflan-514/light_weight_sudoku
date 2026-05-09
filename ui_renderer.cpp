#include "ui_renderer.h"
#include "imgui.h"
#include "color_rgba.h"
#include "my_style.h"
#include <cstdio>

void UIRenderer::drawCell(int8_t row, int8_t col) {
    if (!core) return;
    
    float x = padding + col * cell_size;
    float y = padding + row * cell_size;
    
    bool is_selected = core->isSelected(row, col);
    bool is_highlighted = core->isHighlighted(row, col);
    bool is_given = core->isGiven(row, col);
    bool is_error = core->isError(row, col);
    uint8_t value = core->getValue(row, col);
    
    ImVec4 bg_color;
    if (is_selected) {
        bg_color = rgba::to_ImVec4(0, 120, 212, 255);
    } else if (is_highlighted) {
        bg_color = rgba::to_ImVec4(63, 63, 63, 255);
    } else {
        bg_color = rgba::to_ImVec4(45, 45, 46, 255);
    }
    
    ImGui::PushStyleColor(ImGuiCol_Button, bg_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rgba::to_ImVec4(63, 63, 63, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, rgba::to_ImVec4(72, 72, 72, 255));
    
    ImGui::SetCursorPos(ImVec2(x, y));
    char button_id[16];
    snprintf(button_id, sizeof(button_id), "##cell_%d_%d", row, col);
    if (ImGui::Button(button_id, ImVec2(cell_size - 1, cell_size - 1))) {
        core->selectCell(row, col);
    }
    
    ImGui::PopStyleColor(3);
    
    ImGui::SetCursorPos(ImVec2(x + 2, y + 2));
    ImGui::BeginChild("cell_content", ImVec2(cell_size - 5, cell_size - 5), false, ImGuiWindowFlags_NoDecoration);
    
    if (value != 0) {
        ImVec4 text_color = is_given ? rgba::to_ImVec4(255, 255, 255, 255) : rgba::to_ImVec4(0, 140, 255, 255);
        if (is_error) {
            text_color = rgba::to_ImVec4(232, 17, 35, 255);
        }
        
        ImGui::PushStyleColor(ImGuiCol_Text, text_color);
        ImGui::SetCursorPos(ImVec2((cell_size - 30) / 2, (cell_size - 36) / 2));
        ImGui::Text("%d", value);
        ImGui::PopStyleColor();
    } else {
        const uint8_t* notes = core->getNotes(row, col);
        if (!notes) {
            ImGui::EndChild();
            return;
        }
        float note_size = cell_size / 3;
        float note_pad = 2.0f;
        
        for (uint8_t i = 0; i < 9; ++i) {
            if (notes[i]) {
                uint8_t note_row = i / 3;
                uint8_t note_col = i % 3;
                float nx = note_col * note_size + note_pad;
                float ny = note_row * note_size + note_pad;
                
                ImGui::PushStyleColor(ImGuiCol_Text, rgba::to_ImVec4(110, 110, 110, 255));
                ImGui::SetCursorPos(ImVec2(nx, ny));
                ImGui::Text("%d", i + 1);
                ImGui::PopStyleColor();
            }
        }
    }
    
    ImGui::EndChild();
}

void UIRenderer::drawGrid() {
    if (!core) return;
    
    float grid_size = padding * 2 + cell_size * 9;
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, rgba::to_ImVec4(37, 37, 38, 255));
    ImGui::BeginChild("sudoku_grid", ImVec2(grid_size, grid_size), true, ImGuiWindowFlags_NoDecoration);
    
    for (uint8_t row = 0; row < 9; ++row) {
        for (uint8_t col = 0; col < 9; ++col) {
            drawCell(row, col);
        }
    }
    
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void UIRenderer::drawNumberPad() {
    if (!core) return;
    
    ImGui::BeginChild("number_pad", ImVec2(cell_size * 3 + padding * 2, cell_size * 4 + padding * 2), true, ImGuiWindowFlags_NoDecoration);
    
    static const char num_labels[9][2] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    
    for (uint8_t i = 0; i < 9; ++i) {
        uint8_t row = i / 3;
        uint8_t col = i % 3;
        float x = padding + col * cell_size;
        float y = padding + row * cell_size;
        
        UI::apply_win11_accent_button();
        ImGui::SetCursorPos(ImVec2(x, y));
        if (ImGui::Button(num_labels[i], ImVec2(cell_size - 1, cell_size - 1))) {
            int8_t sel_row = core->getSelectedRow();
            int8_t sel_col = core->getSelectedCol();
            if (sel_row >= 0 && sel_col >= 0) {
                core->setValue(sel_row, sel_col, i + 1);
            }
        }
        ImGui::PopStyleColor(3);
    }
    
    float x = padding;
    float y = padding + 3 * cell_size;
    
    UI::apply_win11_dark_button();
    ImGui::SetCursorPos(ImVec2(x, y));
    if (ImGui::Button("Erase", ImVec2(cell_size * 3 - 1, cell_size - 1))) {
        int8_t sel_row = core->getSelectedRow();
        int8_t sel_col = core->getSelectedCol();
        if (sel_row >= 0 && sel_col >= 0) {
            core->eraseValue(sel_row, sel_col);
        }
    }
    ImGui::PopStyleColor(3);
    
    ImGui::EndChild();
}

void UIRenderer::drawControls() {
    if (!core) return;
    
    ImGui::BeginChild("controls", ImVec2(cell_size * 3 + padding * 2, 120), true, ImGuiWindowFlags_NoDecoration);
    
    ImGui::SetCursorPos(ImVec2(padding, padding));
    UI::apply_win11_accent_button();
    if (ImGui::Button("New Game", ImVec2(-1, 32))) {
        core->loadPuzzle(Difficulty::NORMAL);
    }
    ImGui::PopStyleColor(3);
    
    ImGui::SetCursorPos(ImVec2(padding, padding + 40));
    UI::apply_win11_dark_button();
    if (ImGui::Button("Reset", ImVec2(-1, 32))) {
        core->reset();
    }
    ImGui::PopStyleColor(3);
    
    ImGui::SetCursorPos(ImVec2(padding, padding + 80));
    UI::apply_win11_dark_button();
    if (ImGui::Button("Check", ImVec2(-1, 32))) {
        bool complete = core->isComplete();
        if (complete) {
            ImGui::OpenPopup("Complete");
        } else {
            core->checkConflicts();
        }
    }
    ImGui::PopStyleColor(3);
    
    ImGui::EndChild();
}

void UIRenderer::render() {
    if (!core) return;
    
    ImGui::BeginGroup();
    drawGrid();
    ImGui::EndGroup();
    
    ImGui::SameLine();
    
    ImGui::BeginGroup();
    drawNumberPad();
    drawControls();
    ImGui::EndGroup();
    
    ImGui::BeginPopup("Complete");
    ImGui::Text("Congratulations!");
    ImGui::Text("You solved the puzzle!");
    if (ImGui::Button("OK")) {
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}

void UIRenderer::handleInput() {
    if (!core || !window) return;
    
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard) return;
    
    int8_t sel_row = core->getSelectedRow();
    int8_t sel_col = core->getSelectedCol();
    if (sel_row < 0 || sel_col < 0) return;
    
    static bool key_states[15] = {false};
    
    for (uint8_t i = 0; i < 9; ++i) {
        int key = GLFW_KEY_1 + i;
        bool pressed = glfwGetKey(window, key) == GLFW_PRESS;
        if (pressed && !key_states[i]) {
            key_states[i] = true;
            core->setValue(sel_row, sel_col, i + 1);
            return;
        }
        key_states[i] = pressed;
    }
    
    bool del_pressed = glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS;
    bool back_pressed = glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS;
    if ((del_pressed && !key_states[9]) || (back_pressed && !key_states[10])) {
        key_states[9] = del_pressed;
        key_states[10] = back_pressed;
        core->eraseValue(sel_row, sel_col);
        return;
    }
    key_states[9] = del_pressed;
    key_states[10] = back_pressed;
    
    bool up_pressed = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    bool down_pressed = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    bool left_pressed = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool right_pressed = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    
    if (up_pressed && !key_states[11] && sel_row > 0) {
        key_states[11] = true;
        core->selectCell(sel_row - 1, sel_col);
    }
    key_states[11] = up_pressed;
    
    if (down_pressed && !key_states[12] && sel_row < 8) {
        key_states[12] = true;
        core->selectCell(sel_row + 1, sel_col);
    }
    key_states[12] = down_pressed;
    
    if (left_pressed && !key_states[13] && sel_col > 0) {
        key_states[13] = true;
        core->selectCell(sel_row, sel_col - 1);
    }
    key_states[13] = left_pressed;
    
    if (right_pressed && !key_states[14] && sel_col < 8) {
        key_states[14] = true;
        core->selectCell(sel_row, sel_col + 1);
    }
    key_states[14] = right_pressed;
}

void UIRenderer::setCellSize(float size) {
    cell_size = size;
}
