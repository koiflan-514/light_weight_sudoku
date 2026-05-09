#include "my_style.h"

namespace UI {

    // === 内部辅助函数 ===
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

    // === 重置所有样式 ===
    void reset_all_styles() {
        ImGui::PopStyleColor(ImGuiCol_COUNT);
        ImGui::PopStyleVar(ImGuiStyleVar_COUNT);
        // 注意: 这需要你知道之前压入了多少，实际使用中建议手动配对
        // 更可靠的做法是调用 ImGui::GetStyle() 并逐个字段重置，但这里为了简洁，提供一个示例
        // 实际工程中建议在每帧开始时用 ImGui::Push... 并在该帧结束时 Pop，避免全局混乱。
        // 这里提供一个快速重置为默认 ImGui 风格的方法：
        ImGui::GetStyle() = ImGuiStyle();
    }

} // namespace UI