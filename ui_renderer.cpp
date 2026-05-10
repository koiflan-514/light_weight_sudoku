#include "ui_renderer.h"
#include <algorithm>
#include <cmath>

ImU32 UIRenderer::lerpColor(ImU32 col_a, ImU32 col_b, float t) {
    int r = static_cast<int>((col_a & 0xFF) + ((col_b & 0xFF) - (col_a & 0xFF)) * t);
    int g = static_cast<int>(((col_a >> 8) & 0xFF) + (((col_b >> 8) & 0xFF) - ((col_a >> 8) & 0xFF)) * t);
    int b = static_cast<int>(((col_a >> 16) & 0xFF) + (((col_b >> 16) & 0xFF) - ((col_a >> 16) & 0xFF)) * t);
    int a = static_cast<int>(((col_a >> 24) & 0xFF) + (((col_b >> 24) & 0xFF) - ((col_a >> 24) & 0xFF)) * t);
    return IM_COL32(r, g, b, a);
}

void UIRenderer::updateLayout() {
    ImVec2 win_size = ImGui::GetWindowSize();
    float max_w = win_size.x * 0.9f;
    float max_h = win_size.y * 0.9f;

    cell_size = std::min(max_w / 14.0f, max_h / 9.5f);
    cell_size = std::clamp(cell_size, 30.0f, 120.0f);
    padding = cell_size * 0.1f;

    float total_grid_w = cell_size * 9.0f;
    float total_ui_w = total_grid_w + padding * 3.0f + cell_size * 4.0f;

    float start_x = (win_size.x - total_ui_w) * 0.5f;
    float start_y = (win_size.y - total_grid_w) * 0.5f;

    // 转场滑动 Y 轴偏移量计算 (Ease-Out Cubic 算法)
    float ease = 1.0f - std::pow(1.0f - transition_t, 3.0f);
    float slide_offset = (1.0f - ease) * 40.0f; // 从下方 40px 滑入

    grid_start_pos = ImVec2(start_x, start_y + slide_offset);
    right_panel_pos = ImVec2(start_x + total_grid_w + padding * 3.0f, start_y + slide_offset);
}

void UIRenderer::triggerRipple(int target_r, int target_c) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (r == target_r || c == target_c || (r / 3 == target_r / 3 && c / 3 == target_c / 3)) {
                float dist = static_cast<float>(std::sqrt(std::pow(r - target_r, 2) + std::pow(c - target_c, 2)));
                anims[r][c].ripple_delay = dist * 0.025f;
                anims[r][c].ripple_fade = 0.0f;
            }
        }
    }
}

void UIRenderer::inputNumber(int val) {
    int8_t r = core->getSelectedRow();
    int8_t c = core->getSelectedCol();
    if (r >= 0 && c >= 0 && !core->isGiven(r, c)) {
        if (val == 0) core->eraseValue(r, c);
        else if (core->getValue(r, c) != val) {
            core->setValue(r, c, val);
            anims[r][c].pop_scale = 1.6f;
            triggerRipple(r, c);
        }
    }
}

void UIRenderer::updateAnimations(float dt) {
    // 状态机渐变切换动画
    if (current_state != next_state) {
        transition_t -= dt * 4.0f; // Fade out 速度
        if (transition_t <= 0.0f) {
            current_state = next_state; // 实际切换
            transition_t = 0.0f;
        }
    }
    else if (transition_t < 1.0f) {
        transition_t = std::min(1.0f, transition_t + dt * 4.0f); // Fade in 速度
    }

    // 只有在 PLAYING 状态才处理波浪动画
    if (current_state == AppState::PLAYING) {
        bool is_comp = core->isComplete();
        if (is_comp && !was_complete) win_timer = 0.0f;
        was_complete = is_comp;
        if (is_comp) win_timer += dt;

        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (anims[r][c].pop_scale > 1.0f) {
                    anims[r][c].pop_scale = std::max(1.0f, anims[r][c].pop_scale - dt * 5.0f);
                }

                if (anims[r][c].ripple_delay > 0.0f) {
                    anims[r][c].ripple_delay -= dt;
                    if (anims[r][c].ripple_delay <= 0.0f) anims[r][c].ripple_fade = 1.0f;
                }
                else if (anims[r][c].ripple_fade > 0.0f) {
                    anims[r][c].ripple_fade = std::max(0.0f, anims[r][c].ripple_fade - dt * 2.5f);
                }

                bool is_sel = core->isSelected(r, c);
                bool is_hi = core->isHighlighted(r, c);
                anims[r][c].select_fade = std::clamp(anims[r][c].select_fade + (is_sel ? dt * 10.0f : -dt * 8.0f), 0.0f, 1.0f);
                anims[r][c].highlight_fade = std::clamp(anims[r][c].highlight_fade + (is_hi ? dt * 10.0f : -dt * 8.0f), 0.0f, 1.0f);
            }
        }
    }
}

void UIRenderer::drawStartMenu(float alpha, float y_offset) {
    ImVec2 win_size = ImGui::GetWindowSize();

    // 根据屏幕高度计算一个基础缩放系数 (以 1080p 屏幕作为基准 1.0)
    float base_scale = win_size.y / 1080.0f;
    base_scale = std::clamp(base_scale, 0.8f, 2.0f); // 限制缩放极值

    // 1. 巨大霸气的主标题
    ImGui::SetWindowFontScale(3.5f * base_scale);
    const char* title = "Win11 Sudoku";
    float title_w = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPos(ImVec2((win_size.x - title_w) * 0.5f, win_size.y * 0.25f + y_offset));
    ImVec4 title_col = ImGui::GetStyle().Colors[ImGuiCol_Text];
    if (UI::CurrentTheme.text_normal == IM_COL32(0, 180, 255, 255)) title_col = ImVec4(0.0f, 0.47f, 0.83f, 1.0f);
    ImGui::TextColored(ImVec4(title_col.x, title_col.y, title_col.z, alpha), "%s", title);

    // 2. 优雅的副标题
    ImGui::SetWindowFontScale(1.2f * base_scale);
    const char* subtitle = "Select Difficulty to Begin";
    float sub_w = ImGui::CalcTextSize(subtitle).x;
    ImGui::SetCursorPos(ImVec2((win_size.x - sub_w) * 0.5f, win_size.y * 0.25f + 80.0f * base_scale + y_offset));
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, alpha), "%s", subtitle);

    // 3. 随屏幕放大的按钮
    float btn_w = 300.0f * base_scale;
    float btn_h = 55.0f * base_scale;
    float start_y = win_size.y * 0.5f + y_offset;

    // 给按钮设置统一的大字体
    ImGui::SetWindowFontScale(1.2f * base_scale);

    auto drawDiffBtn = [&](const char* label, float offset_y, Difficulty diff, bool is_accent) {
        ImGui::SetCursorPos(ImVec2((win_size.x - btn_w) * 0.5f, start_y + offset_y * base_scale));
        int pushes = is_accent ? UI::apply_accent_button() : UI::apply_dark_button();
        if (ImGui::Button(label, ImVec2(btn_w, btn_h))) {
            core->loadPuzzle(diff);
            next_state = AppState::PLAYING;
        }
        ImGui::PopStyleColor(pushes);
        };

    drawDiffBtn("Easy", 0.0f, Difficulty::EASY, false);
    drawDiffBtn("Normal", 70.0f, Difficulty::NORMAL, true);
    drawDiffBtn("Hard", 140.0f, Difficulty::HARD, false);
    drawDiffBtn("Expert", 210.0f, Difficulty::EXPERT, false);

    ImGui::SetCursorPos(ImVec2((win_size.x - btn_w) * 0.5f, start_y + 300.0f * base_scale));
    int pushes = UI::apply_dark_button();
    if (ImGui::Button("Exit Game", ImVec2(btn_w, btn_h))) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    ImGui::PopStyleColor(pushes);

    // 主题切换底栏
    ImGui::SetWindowFontScale(1.0f * base_scale);
    ImGui::SetCursorPos(ImVec2((win_size.x - 260 * base_scale) * 0.5f, win_size.y - 60.0f * base_scale + y_offset));
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, alpha), "Theme:");
    ImGui::SameLine();
    ImGui::PushItemWidth(180.0f * base_scale);
    const char* themes[] = { "Win11 Dark", "Win11 Light", "Matcha Green" };
    static int current_theme = 0;
    if (ImGui::Combo("##ThemeCombo", &current_theme, themes, 3)) {
        UI::apply_theme((UI::ThemeType)current_theme);
    }
    ImGui::PopItemWidth();

    // 必须恢复字体缩放，以免影响其他未设置的地方
    ImGui::SetWindowFontScale(1.0f);
}

void UIRenderer::drawGrid(ImDrawList* draw_list, float alpha) {
    ImVec2 p0 = ImGui::GetCursorScreenPos();
    p0.x += grid_start_pos.x;
    p0.y += grid_start_pos.y;

    ImGui::SetCursorScreenPos(p0);
    ImGui::InvisibleButton("SudokuGridArea", ImVec2(cell_size * 9, cell_size * 9));

    // 1. 获取当前真实被悬停的格子坐标
    int current_hover_r = -1;
    int current_hover_c = -1;

    if (transition_t == 1.0f && (ImGui::IsItemActive() || ImGui::IsItemHovered())) {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        int hc = static_cast<int>((mouse_pos.x - p0.x) / cell_size);
        int hr = static_cast<int>((mouse_pos.y - p0.y) / cell_size);
        if (hr >= 0 && hr < 9 && hc >= 0 && hc < 9) {
            current_hover_r = hr;
            current_hover_c = hc;
            if (ImGui::IsMouseClicked(0)) core->selectCell(hr, hc);
        }
    }

    // 2. 互斥计算渐变（绝对不会再闪烁）
    float dt = ImGui::GetIO().DeltaTime;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (r == current_hover_r && c == current_hover_c) {
                // 如果是当前鼠标悬停的格子，平滑增加亮度
                anims[r][c].hover_fade = std::min(anims[r][c].hover_fade + dt * 12.0f, 1.0f);
            }
            else {
                // 否则，平滑衰减亮度
                anims[r][c].hover_fade = std::max(anims[r][c].hover_fade - dt * 8.0f, 0.0f);
            }
        }
    }

    // 绘制九宫格大底色
    draw_list->AddRectFilled(p0, ImVec2(p0.x + cell_size * 9, p0.y + cell_size * 9), UI::fade_color(UI::CurrentTheme.grid_bg, alpha), 8.0f);

    ImFont* font = ImGui::GetFont();
    float base_font_size = cell_size * 0.6f;

    // 1. 渲染普通格子
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            ImVec2 cell_pmin(p0.x + c * cell_size, p0.y + r * cell_size);
            ImVec2 cell_pmax(cell_pmin.x + cell_size, cell_pmin.y + cell_size);

            ImU32 bg_col = UI::CurrentTheme.cell_bg;
            bg_col = lerpColor(bg_col, UI::CurrentTheme.cell_highlight, anims[r][c].highlight_fade);
            bg_col = lerpColor(bg_col, UI::CurrentTheme.cell_hover, anims[r][c].hover_fade);
            bg_col = lerpColor(bg_col, UI::CurrentTheme.cell_select, anims[r][c].select_fade);
            bg_col = lerpColor(bg_col, UI::CurrentTheme.ripple, anims[r][c].ripple_fade);

            if (was_complete) {
                float delay = (r + c) * 0.08f;
                float t = std::clamp(win_timer - delay, 0.0f, 1.0f);
                if (t > 0.0f && t < 1.0f) bg_col = lerpColor(bg_col, UI::CurrentTheme.wave, std::sin(t * 3.14159f));
            }

            draw_list->AddRectFilled(ImVec2(cell_pmin.x + 1, cell_pmin.y + 1), ImVec2(cell_pmax.x - 1, cell_pmax.y - 1), UI::fade_color(bg_col, alpha), 4.0f);

            uint8_t val = core->getValue(r, c);
            if (val != 0) {
                ImU32 text_col = core->isGiven(r, c) ? UI::CurrentTheme.text_given : UI::CurrentTheme.text_normal;
                if (core->isError(r, c)) text_col = UI::CurrentTheme.text_error;

                char txt[2] = { (char)('0' + val), '\0' };
                float anim_font_size = base_font_size * anims[r][c].pop_scale;
                ImVec2 t_size = font->CalcTextSizeA(anim_font_size, FLT_MAX, 0.0f, txt);
                draw_list->AddText(font, anim_font_size, ImVec2(cell_pmin.x + (cell_size - t_size.x) * 0.5f, cell_pmin.y + (cell_size - t_size.y) * 0.5f), UI::fade_color(text_col, alpha), txt);
            }
        }
    }

    // 2. 渲染 3x3 加粗边界
    float thick = std::max(2.0f, cell_size * 0.05f);
    ImU32 border_col = UI::fade_color(UI::CurrentTheme.border, alpha);

    for (int i = 1; i < 3; ++i) {
        float vx = p0.x + i * 3 * cell_size;
        float vy = p0.y + i * 3 * cell_size;
        draw_list->AddLine(ImVec2(vx, p0.y), ImVec2(vx, p0.y + 9 * cell_size), border_col, thick);
        draw_list->AddLine(ImVec2(p0.x, vy), ImVec2(p0.x + 9 * cell_size, vy), border_col, thick);
    }
    draw_list->AddRect(p0, ImVec2(p0.x + 9 * cell_size, p0.y + 9 * cell_size), border_col, 8.0f, 0, thick);
}

void UIRenderer::drawNumberPad(float alpha) {
    ImGui::SetCursorPos(right_panel_pos);
    ImGui::BeginChild("Numpad", ImVec2(cell_size * 4, cell_size * 5), false, ImGuiWindowFlags_NoScrollbar);

    float btn_sz = cell_size * 1.1f;
    static const char* labels[9] = { "1","2","3","4","5","6","7","8","9" };

    // 让数字按键字体跟随 cell_size 动态放大 (大号字体)
    ImGui::SetWindowFontScale(cell_size / 35.0f);

    for (int i = 0; i < 9; ++i) {
        if (i % 3 != 0) ImGui::SameLine(0, padding);
        int pushes = UI::apply_accent_button();
        if (ImGui::Button(labels[i], ImVec2(btn_sz, btn_sz))) inputNumber(i + 1);
        ImGui::PopStyleColor(pushes);
    }

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

    // 擦除按钮字体稍微小一点
    ImGui::SetWindowFontScale(cell_size / 45.0f);
    int pushes = UI::apply_dark_button();
    if (ImGui::Button("Erase", ImVec2(btn_sz * 3 + padding * 2, btn_sz * 0.8f))) inputNumber(0);
    ImGui::PopStyleColor(pushes);

    ImGui::SetWindowFontScale(1.0f); // 恢复
    ImGui::EndChild();
}


void UIRenderer::drawControls(float alpha) {
    ImGui::SetCursorPos(ImVec2(right_panel_pos.x, right_panel_pos.y + cell_size * 6.0f));
    ImGui::BeginChild("Controls", ImVec2(cell_size * 4, cell_size * 3), false, ImGuiWindowFlags_NoScrollbar);

    float btn_w = cell_size * 3 + padding * 2;
    float btn_h = cell_size * 0.8f;

    // 控制按钮字体跟随 cell_size 缩放
    ImGui::SetWindowFontScale(cell_size / 50.0f);

    int pushes = UI::apply_accent_button();
    if (ImGui::Button("Check", ImVec2(btn_w, btn_h))) core->checkConflicts();
    ImGui::PopStyleColor(pushes);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 0.5f);
    pushes = UI::apply_dark_button();
    if (ImGui::Button("Restart", ImVec2(btn_w, btn_h))) core->reset();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 0.5f);
    if (ImGui::Button("Menu", ImVec2(btn_w, btn_h))) next_state = AppState::START_MENU;
    ImGui::PopStyleColor(pushes);

    ImGui::SetWindowFontScale(1.0f); // 恢复
    ImGui::EndChild();
}

void UIRenderer::handleInput() {
    if (transition_t < 1.0f) return; // 动画期间锁死输入，防误触
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) return;

    int8_t r = core->getSelectedRow();
    int8_t c = core->getSelectedCol();

    for (int i = 0; i < 9; ++i) {
        if (ImGui::IsKeyPressed((ImGuiKey)(ImGuiKey_1 + i), false) ||
            ImGui::IsKeyPressed((ImGuiKey)(ImGuiKey_Keypad1 + i), false)) {
            inputNumber(i + 1);
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Delete, false) || ImGui::IsKeyPressed(ImGuiKey_Backspace, false) ||
        ImGui::IsKeyPressed(ImGuiKey_0, false) || ImGui::IsKeyPressed(ImGuiKey_Keypad0, false)) {
        inputNumber(0);
    }

    if (r < 0 || c < 0) {
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_DownArrow) ||
            ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            core->selectCell(4, 4);
        }
    }
    else {
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, true) && r > 0) core->selectCell(r - 1, c);
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, true) && r < 8) core->selectCell(r + 1, c);
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, true) && c > 0) core->selectCell(r, c - 1);
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, true) && c < 8) core->selectCell(r, c + 1);
    }
}

void UIRenderer::handleWindowDrag() {
    // 确保鼠标悬停在空旷的背景上，没有碰到任何按钮或棋盘
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {

        // === 1. 双击切换：最大化 / 窗口化还原 ===
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
                glfwRestoreWindow(window); // 如果是最大化，则还原
            }
            else {
                glfwMaximizeWindow(window); // 如果是窗口化，则最大化
            }
            return; // 双击后直接返回，避免触发拖拽
        }

        // === 2. 拖拽逻辑：仅在【非最大化】状态下允许拖拽 ===
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            // 检查当前是否处于最大化状态
            if (!glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
                ImVec2 mouse_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
                if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f) {
                    int x, y;
                    glfwGetWindowPos(window, &x, &y);
                    glfwSetWindowPos(window, x + (int)mouse_delta.x, y + (int)mouse_delta.y);
                    ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
                }
            }
        }
    }
}

void UIRenderer::render() {
    // 注入窗口拖拽监听：只要在这帧的任何空白处按下鼠标，窗口就能跟着走
    handleWindowDrag();

    float dt = ImGui::GetIO().DeltaTime;
    updateLayout();
    updateAnimations(dt);

    float ease_alpha = std::pow(transition_t, 2.0f);
    float ease_slide = (1.0f - std::pow(1.0f - transition_t, 3.0f)) * 40.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ease_alpha);

    if (current_state == AppState::START_MENU) {
        drawStartMenu(ease_alpha, 40.0f - ease_slide);
    }
    else {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        drawGrid(draw_list, ease_alpha);
        drawNumberPad(ease_alpha);
        drawControls(ease_alpha);
        handleInput();

        if (was_complete && win_timer > 2.0f) ImGui::OpenPopup("Victory");

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Victory", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::Text("\n   Congratulations! You solved the puzzle!   \n\n");
            ImGui::Separator();
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - 60, ImGui::GetCursorPosY() + 10));
            int pushes = UI::apply_accent_button();
            if (ImGui::Button("Menu", ImVec2(120, 30))) {
                next_state = AppState::START_MENU;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(pushes);
            ImGui::EndPopup();
        }
    }

    ImGui::PopStyleVar();
}