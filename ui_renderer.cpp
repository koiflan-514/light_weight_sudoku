#include "ui_renderer.h"
#include "imgui.h"
#include "color_rgba.h"
#include "my_style.h"

// 渲染大优化：放弃 BeginChild，使用 ImDrawList 零开销渲染
void UIRenderer::drawCell(int8_t row, int8_t col) {
    float x = padding + col * cell_size;
    float y = padding + row * cell_size;

    ImGui::SetCursorPos(ImVec2(x, y));

    // O(1) 生成 ID，替代原先的 snprintf
    ImGui::PushID(row * 9 + col);

    ImVec4 bg_color = rgba::to_ImVec4(45, 45, 46, 255);
    if (core->isSelected(row, col)) bg_color = rgba::to_ImVec4(0, 120, 212, 255);
    else if (core->isHighlighted(row, col)) bg_color = rgba::to_ImVec4(63, 63, 63, 255);

    ImGui::PushStyleColor(ImGuiCol_Button, bg_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rgba::to_ImVec4(63, 63, 63, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, rgba::to_ImVec4(72, 72, 72, 255));

    if (ImGui::Button("", ImVec2(cell_size - 1, cell_size - 1))) {
        core->selectCell(row, col);
    }
    ImGui::PopStyleColor(3);

    // 获取按钮在屏幕上的左上角绝对坐标
    ImVec2 p0 = ImGui::GetItemRectMin();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    uint8_t value = core->getValue(row, col);
    if (value != 0) {
        uint32_t text_color = core->isGiven(row, col) ? IM_COL32(255, 255, 255, 255) : IM_COL32(0, 140, 255, 255);
        if (core->isError(row, col)) text_color = IM_COL32(232, 17, 35, 255);

        char text[2] = { (char)('0' + value), '\0' };
        ImVec2 text_size = ImGui::CalcTextSize(text);
        // 居中绘制
        draw_list->AddText(ImVec2(p0.x + (cell_size - text_size.x) * 0.5f, p0.y + (cell_size - text_size.y) * 0.5f), text_color, text);
    }
    else {
        uint16_t notes = core->getNotes(row, col);
        if (notes) {
            float note_size = cell_size / 3.0f;
            for (int i = 0; i < 9; ++i) {
                if (notes & (1 << i)) { // 通过位掩码判断笔记
                    int n_row = i / 3, n_col = i % 3;
                    char n_text[2] = { (char)('1' + i), '\0' };
                    ImVec2 n_pos = ImVec2(p0.x + n_col * note_size + 4.0f, p0.y + n_row * note_size + 2.0f);
                    draw_list->AddText(n_pos, IM_COL32(110, 110, 110, 255), n_text);
                }
            }
        }
    }

    ImGui::PopID();
}

void UIRenderer::drawGrid() {
    float grid_size = padding * 2 + cell_size * 9;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba::to_ImVec4(37, 37, 38, 255));
    ImGui::BeginChild("sudoku_grid", ImVec2(grid_size, grid_size), true, ImGuiWindowFlags_NoDecoration);
    for (uint8_t row = 0; row < 9; ++row)
        for (uint8_t col = 0; col < 9; ++col)
            drawCell(row, col);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void UIRenderer::drawNumberPad() {
    ImGui::BeginChild("number_pad", ImVec2(cell_size * 3 + padding * 2, cell_size * 4 + padding * 2), true, ImGuiWindowFlags_NoDecoration);
    static const char* num_labels[9] = { "1","2","3","4","5","6","7","8","9" };

    for (int i = 0; i < 9; ++i) {
        float x = padding + (i % 3) * cell_size;
        float y = padding + (i / 3) * cell_size;

        ImGui::SetCursorPos(ImVec2(x, y));
        int pushes = UI::apply_win11_accent_button(); // 必须使用返回值 Pop
        if (ImGui::Button(num_labels[i], ImVec2(cell_size - 1, cell_size - 1))) {
            int8_t r = core->getSelectedRow(), c = core->getSelectedCol();
            if (r >= 0 && c >= 0) core->setValue(r, c, i + 1);
        }
        ImGui::PopStyleColor(pushes);
    }

    ImGui::SetCursorPos(ImVec2(padding, padding + 3 * cell_size));
    int pushes = UI::apply_win11_dark_button();
    if (ImGui::Button("Erase", ImVec2(cell_size * 3 - 1, cell_size - 1))) {
        int8_t r = core->getSelectedRow(), c = core->getSelectedCol();
        if (r >= 0 && c >= 0) core->eraseValue(r, c);
    }
    ImGui::PopStyleColor(pushes);
    ImGui::EndChild();
}

void UIRenderer::drawControls() {
    ImGui::BeginChild("controls", ImVec2(cell_size * 3 + padding * 2, 120), true, ImGuiWindowFlags_NoDecoration);
    ImGui::SetCursorPos(ImVec2(padding, padding));

    int pushes = UI::apply_win11_accent_button();
    if (ImGui::Button("New Game", ImVec2(-1, 32))) core->loadPuzzle(Difficulty::NORMAL);
    ImGui::PopStyleColor(pushes);

    ImGui::SetCursorPos(ImVec2(padding, padding + 40));
    pushes = UI::apply_win11_dark_button();
    if (ImGui::Button("Reset", ImVec2(-1, 32))) core->reset();

    ImGui::SetCursorPos(ImVec2(padding, padding + 80));
    if (ImGui::Button("Check", ImVec2(-1, 32))) {
        if (core->isComplete()) ImGui::OpenPopup("Complete");
        else core->checkConflicts();
    }
    ImGui::PopStyleColor(pushes);
    ImGui::EndChild();
}

void UIRenderer::render() {
    ImGui::BeginGroup(); drawGrid(); ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup(); drawNumberPad(); drawControls(); ImGui::EndGroup();

    if (ImGui::BeginPopupModal("Complete", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Congratulations! You solved the puzzle!");
        if (ImGui::Button("OK", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

// 抛弃 GLFW，使用原生的 ImGui 键盘处理，自带连按过滤机制
void UIRenderer::handleInput() {
    int8_t r = core->getSelectedRow();
    int8_t c = core->getSelectedCol();
    if (r < 0 || c < 0) return;

    // 1-9 数字键（主键盘或小键盘）
    for (int i = 0; i < 9; ++i) {
        if (ImGui::IsKeyPressed((ImGuiKey)(ImGuiKey_1 + i), false) ||
            ImGui::IsKeyPressed((ImGuiKey)(ImGuiKey_Keypad1 + i), false)) {
            core->setValue(r, c, i + 1);
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Delete, false) || ImGui::IsKeyPressed(ImGuiKey_Backspace, false)) {
        core->eraseValue(r, c);
    }

    // 键盘移动逻辑处理 (阻止连按刷屏：传入 false)
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, true) && r > 0) core->selectCell(r - 1, c);
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, true) && r < 8) core->selectCell(r + 1, c);
    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, true) && c > 0) core->selectCell(r, c - 1);
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, true) && c < 8) core->selectCell(r, c + 1);
}