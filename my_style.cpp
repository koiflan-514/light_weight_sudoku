#include "my_style.h"
#include "color_rgba.h"

namespace UI {

    namespace Win11Dark {
        const ImVec4 WindowBg = rgba::to_ImVec4(31, 31, 31, 255);         // #1f1f1f - Win11主背景
        const ImVec4 TitleBg = rgba::to_ImVec4(37, 37, 38, 255);          // #252526 - 侧边栏/标题栏
        const ImVec4 FrameBg = rgba::to_ImVec4(45, 45, 46, 255);          // #2d2d2e - 卡片/控件背景
        const ImVec4 FrameBgHovered = rgba::to_ImVec4(54, 54, 54, 255);    // #363636 - 悬停状态
        const ImVec4 Button = rgba::to_ImVec4(45, 45, 46, 255);           // #2d2d2e - 按钮背景
        const ImVec4 ButtonHovered = rgba::to_ImVec4(54, 54, 54, 255);     // #363636 - 按钮悬停
        const ImVec4 ButtonActive = rgba::to_ImVec4(50, 50, 50, 255);      // #3f3f3f - 按钮按下
        const ImVec4 Accent = rgba::to_ImVec4(0, 120, 212, 255);          // #0078d4 - Win11强调蓝
        const ImVec4 AccentHovered = rgba::to_ImVec4(0, 140, 255, 255);    // #008cff - 高亮蓝
        const ImVec4 Text = rgba::to_ImVec4(255, 255, 255, 255);          // #ffffff - 主文字
        const ImVec4 TextDisabled = rgba::to_ImVec4(110, 110, 110, 255);   // #6e6e6e - 禁用文字
        const ImVec4 Border = rgba::to_ImVec4(48, 48, 48, 255);            // #303030 - 边框
        const ImVec4 CheckMark = rgba::to_ImVec4(255, 255, 255, 255);      // #ffffff - 勾选标记
        const ImVec4 SliderGrab = rgba::to_ImVec4(255, 255, 255, 255);     // #ffffff - 滑块手柄
        const ImVec4 PopupBg = rgba::to_ImVec4(45, 45, 46, 255);           // #2d2d2e - 弹出层背景
        constexpr float Rounding = 8.0f;
        constexpr float FrameRounding = 4.0f;
        constexpr float GrabRounding = 4.0f;
        constexpr ImVec2 FramePadding = ImVec2(12, 10);
        constexpr ImVec2 WindowPadding = ImVec2(16, 12);
    }

    static void push_style_color(ImGuiCol idx, ImVec4 color) {
        ImGui::PushStyleColor(idx, color);
    }

    static void push_style_var(ImGuiStyleVar idx, float val) {
        ImGui::PushStyleVar(idx, val);
    }
    static void push_style_var(ImGuiStyleVar idx, ImVec2 val) {
        ImGui::PushStyleVar(idx, val);
    }

    // === 窗口样式 ===
    void set_window_rounding(float rounding) {
        push_style_var(ImGuiStyleVar_WindowRounding, rounding);
    }

    void set_window_padding(ImVec2 padding) {
        push_style_var(ImGuiStyleVar_WindowPadding, padding);
    }

    void set_window_border(bool enable) {
        // ImGui 没有直接开关边框的变量，通过边框颜色透明度控制
        ImVec4 border_color = enable ? ImVec4(0.5f, 0.5f, 0.5f, 1.0f) : ImVec4(0, 0, 0, 0);
        push_style_color(ImGuiCol_Border, border_color);
    }

    void set_window_title_align(ImVec2 align) {
        push_style_var(ImGuiStyleVar_WindowTitleAlign, align);
    }

    void set_window_bg_color(ImVec4 color) {
        push_style_color(ImGuiCol_WindowBg, color);
    }

    void set_window_title_bg_color(ImVec4 color) {
        push_style_color(ImGuiCol_TitleBg, color);
        push_style_color(ImGuiCol_TitleBgActive, color);
        push_style_color(ImGuiCol_TitleBgCollapsed, color);
    }

    // === 按钮样式 ===
    void set_button_rounding(float rounding) {
        push_style_var(ImGuiStyleVar_FrameRounding, rounding);
        // 注意: 按钮圆角与 FrameRounding 共享，也可单独用 ChildRounding 但一般不单独
    }

    void set_button_padding(ImVec2 padding) {
        push_style_var(ImGuiStyleVar_FramePadding, padding);
    }

    void set_button_colors(ImVec4 normal, ImVec4 hovered, ImVec4 active) {
        push_style_color(ImGuiCol_Button, normal);
        push_style_color(ImGuiCol_ButtonHovered, hovered);
        push_style_color(ImGuiCol_ButtonActive, active);
    }

    void set_button_text_color(ImVec4 color) {
        push_style_color(ImGuiCol_Text, color);
    }

    // === 帧样式（输入框、组合框等）===
    void set_frame_rounding(float rounding) {
        push_style_var(ImGuiStyleVar_FrameRounding, rounding);
    }

    void set_frame_padding(ImVec2 padding) {
        push_style_var(ImGuiStyleVar_FramePadding, padding);
    }

    void set_frame_bg_color(ImVec4 color) {
        push_style_color(ImGuiCol_FrameBg, color);
    }

    void set_frame_border_color(ImVec4 color) {
        push_style_color(ImGuiCol_Border, color);
    }

    // === 复选框/单选框 ===
    void set_checkbox_rounding(float rounding) {
        push_style_var(ImGuiStyleVar_FrameRounding, rounding);
    }

    void set_checkbox_colors(ImVec4 normal, ImVec4 hovered, ImVec4 checked) {
        push_style_color(ImGuiCol_CheckMark, checked);
        push_style_color(ImGuiCol_FrameBg, normal);
        push_style_color(ImGuiCol_FrameBgHovered, hovered);
    }

    // === 滑块 ===
    void set_slider_grab_rounding(float rounding) {
        push_style_var(ImGuiStyleVar_GrabRounding, rounding);
    }

    void set_slider_grab_color(ImVec4 color) {
        push_style_color(ImGuiCol_SliderGrab, color);
        push_style_color(ImGuiCol_SliderGrabActive, color);
    }

    void set_slider_thumb_size(float size) {
        push_style_var(ImGuiStyleVar_GrabMinSize, size);
    }

    // === 文本颜色 ===
    void set_text_color(ImVec4 color) {
        push_style_color(ImGuiCol_Text, color);
    }

    void set_disabled_text_color(ImVec4 color) {
        push_style_color(ImGuiCol_TextDisabled, color);
    }

    // === 弹出层 ===
    void set_popup_rounding(float rounding) {
        push_style_var(ImGuiStyleVar_PopupRounding, rounding);
    }

    void set_popup_bg_color(ImVec4 color) {
        push_style_color(ImGuiCol_PopupBg, color);
    }

    void set_popup_border_color(ImVec4 color) {
        push_style_color(ImGuiCol_Border, color);
    }

    void apply_win11_dark_theme() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = Win11Dark::Rounding;
        style.FrameRounding = Win11Dark::FrameRounding;
        style.GrabRounding = Win11Dark::GrabRounding;
        style.FramePadding = Win11Dark::FramePadding;
        style.WindowPadding = Win11Dark::WindowPadding;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style.Colors[ImGuiCol_WindowBg] = Win11Dark::WindowBg;
        style.Colors[ImGuiCol_TitleBg] = Win11Dark::TitleBg;
        style.Colors[ImGuiCol_TitleBgActive] = Win11Dark::TitleBg;
        style.Colors[ImGuiCol_TitleBgCollapsed] = Win11Dark::TitleBg;
        style.Colors[ImGuiCol_FrameBg] = Win11Dark::FrameBg;
        style.Colors[ImGuiCol_FrameBgHovered] = Win11Dark::FrameBgHovered;
        style.Colors[ImGuiCol_FrameBgActive] = Win11Dark::ButtonActive;
        style.Colors[ImGuiCol_Button] = Win11Dark::Button;
        style.Colors[ImGuiCol_ButtonHovered] = Win11Dark::ButtonHovered;
        style.Colors[ImGuiCol_ButtonActive] = Win11Dark::ButtonActive;
        style.Colors[ImGuiCol_Text] = Win11Dark::Text;
        style.Colors[ImGuiCol_TextDisabled] = Win11Dark::TextDisabled;
        style.Colors[ImGuiCol_Border] = Win11Dark::Border;
        style.Colors[ImGuiCol_CheckMark] = Win11Dark::CheckMark;
        style.Colors[ImGuiCol_SliderGrab] = Win11Dark::SliderGrab;
        style.Colors[ImGuiCol_SliderGrabActive] = Win11Dark::Accent;
        style.Colors[ImGuiCol_PopupBg] = Win11Dark::PopupBg;
        style.Colors[ImGuiCol_Header] = Win11Dark::FrameBg;
        style.Colors[ImGuiCol_HeaderHovered] = Win11Dark::FrameBgHovered;
        style.Colors[ImGuiCol_HeaderActive] = Win11Dark::ButtonActive;
        style.Colors[ImGuiCol_Tab] = Win11Dark::FrameBg;
        style.Colors[ImGuiCol_TabHovered] = Win11Dark::FrameBgHovered;
        style.Colors[ImGuiCol_TabActive] = Win11Dark::WindowBg;
        style.Colors[ImGuiCol_ResizeGrip] = Win11Dark::Border;
        style.Colors[ImGuiCol_ResizeGripHovered] = Win11Dark::SliderGrab;
        style.Colors[ImGuiCol_ResizeGripActive] = Win11Dark::Accent;
        style.Colors[ImGuiCol_Separator] = Win11Dark::Border;
        style.Colors[ImGuiCol_ScrollbarBg] = Win11Dark::FrameBg;
        style.Colors[ImGuiCol_ScrollbarGrab] = Win11Dark::SliderGrab;
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = Win11Dark::FrameBgHovered;
        style.Colors[ImGuiCol_ScrollbarGrabActive] = Win11Dark::Accent;
    }

    void apply_win11_accent_button() {
        push_style_color(ImGuiCol_Button, Win11Dark::Accent);
        push_style_color(ImGuiCol_ButtonHovered, Win11Dark::AccentHovered);
        push_style_color(ImGuiCol_ButtonActive, Win11Dark::Accent);
    }

    void apply_win11_dark_button() {
        push_style_color(ImGuiCol_Button, Win11Dark::Button);
        push_style_color(ImGuiCol_ButtonHovered, Win11Dark::ButtonHovered);
        push_style_color(ImGuiCol_ButtonActive, Win11Dark::ButtonActive);
    }

    // === 重置所有样式 ===
    void reset_all_styles() {
        ImGui::GetStyle() = ImGuiStyle();
    }

} // namespace UI