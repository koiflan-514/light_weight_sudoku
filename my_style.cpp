#include "my_style.h"

namespace UI {
    ThemeData CurrentTheme;
    ThemeType ActiveTheme = ThemeType::Dark;

    ImU32 fade_color(ImU32 col, float alpha_mul) {
        int a = (int)(((col >> 24) & 0xFF) * alpha_mul);
        return (col & 0x00FFFFFF) | (a << 24);
    }

    void apply_theme(ThemeType theme) {
        ActiveTheme = theme;
        ImGuiStyle& style = ImGui::GetStyle();

        // 全局圆角统一
        style.WindowRounding = 8.0f;
        style.FrameRounding = 6.0f;
        style.ChildRounding = 6.0f;
        style.PopupRounding = 6.0f;

        ImVec4* colors = style.Colors;

        if (theme == ThemeType::Dark) {
            colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f); // #1f1f1f
            colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

            CurrentTheme = {
                IM_COL32(37, 37, 38, 255),    // grid_bg
                IM_COL32(45, 45, 46, 255),    // cell_bg
                IM_COL32(80, 80, 80, 255),    // cell_hover
                IM_COL32(63, 63, 63, 255),    // cell_highlight
                IM_COL32(0, 120, 212, 255),   // cell_select
                IM_COL32(255, 255, 255, 255), // text_given
                IM_COL32(0, 180, 255, 255),   // text_normal
                IM_COL32(255, 80, 80, 255),   // text_error
                IM_COL32(150, 150, 150, 255), // text_note
                IM_COL32(20, 20, 20, 255),    // border
                IM_COL32(255, 255, 255, 200), // ripple
                IM_COL32(0, 200, 100, 255)    // wave
            };
        }
        else if (theme == ThemeType::Light) {
            colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f); // #f2f2f2
            colors[ImGuiCol_Text] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

            CurrentTheme = {
                IM_COL32(255, 255, 255, 255), // grid_bg
                IM_COL32(243, 243, 243, 255), // cell_bg
                IM_COL32(225, 225, 225, 255), // cell_hover
                IM_COL32(235, 235, 235, 255), // cell_highlight
                IM_COL32(204, 232, 255, 255), // cell_select
                IM_COL32(0, 0, 0, 255),       // text_given
                IM_COL32(0, 103, 192, 255),   // text_normal
                IM_COL32(232, 17, 35, 255),   // text_error
                IM_COL32(130, 130, 130, 255), // text_note
                IM_COL32(200, 200, 200, 255), // border
                IM_COL32(255, 255, 255, 200), // ripple
                IM_COL32(40, 180, 99, 255)    // wave
            };
        }
        else if (theme == ThemeType::Matcha) {
            colors[ImGuiCol_WindowBg] = ImVec4(0.96f, 0.97f, 0.94f, 1.0f); // Fresh Green
            colors[ImGuiCol_Text] = ImVec4(0.2f, 0.25f, 0.2f, 1.0f);

            CurrentTheme = {
                IM_COL32(255, 255, 255, 255),
                IM_COL32(240, 244, 236, 255),
                IM_COL32(215, 228, 205, 255),
                IM_COL32(227, 235, 220, 255),
                IM_COL32(163, 196, 140, 255),
                IM_COL32(44, 54, 37, 255),
                IM_COL32(92, 128, 69, 255),
                IM_COL32(211, 47, 47, 255),
                IM_COL32(140, 150, 130, 255),
                IM_COL32(180, 190, 170, 255),
                IM_COL32(255, 255, 255, 200),
                IM_COL32(100, 200, 120, 255)
            };
        }
    }

    int apply_accent_button() {
        if (ActiveTheme == ThemeType::Dark) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.47f, 0.83f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.55f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.47f, 0.83f, 1.0f));
        }
        else if (ActiveTheme == ThemeType::Light) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.40f, 0.75f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.47f, 0.83f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.35f, 0.65f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // 强行设为白字
            return 4;
        }
        else { // Matcha
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.63f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.53f, 0.73f, 0.41f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45f, 0.63f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            return 4;
        }
        return 3;
    }

    int apply_dark_button() {
        if (ActiveTheme == ThemeType::Dark) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        }
        else { // Light & Matcha
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
            return 4;
        }
        return 3;
    }
}