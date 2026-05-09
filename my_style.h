#pragma once
#include "imgui.h"
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

	void apply_win11_dark_theme();
	inline int apply_win11_accent_button() {
		ImGui::PushStyleColor(ImGuiCol_Button, Win11Dark::Accent);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Win11Dark::AccentHovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Win11Dark::Accent);
		return 3; // 返回弹出的层数
	}

	inline int apply_win11_dark_button() {
		ImGui::PushStyleColor(ImGuiCol_Button, Win11Dark::Button);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Win11Dark::ButtonHovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Win11Dark::ButtonActive);
		return 3; // 返回弹出的层数
	}

	void set_window_rounding(float rounding);
	void set_window_padding(ImVec2 padding);
	void set_window_border(bool enable);
	void set_window_title_align(ImVec2 align);
	void set_window_bg_color(ImVec4 color);
	void set_window_title_bg_color(ImVec4 color);

	void set_button_rounding(float rounding);
	void set_button_padding(ImVec2 padding);
	void set_button_colors(ImVec4 normal, ImVec4 hovered, ImVec4 active);
	void set_button_text_color(ImVec4 color);

	void set_frame_rounding(float rounding);
	void set_frame_padding(ImVec2 padding);
	void set_frame_bg_color(ImVec4 color);
	void set_frame_border_color(ImVec4 color);

	void set_checkbox_rounding(float rounding);
	void set_checkbox_colors(ImVec4 normal, ImVec4 hovered, ImVec4 checked);

	void set_slider_grab_rounding(float rounding);
	void set_slider_grab_color(ImVec4 color);
	void set_slider_thumb_size(float size);

	void set_text_color(ImVec4 color);
	void set_disabled_text_color(ImVec4 color);

	void set_popup_rounding(float rounding);
	void set_popup_bg_color(ImVec4 color);
	void set_popup_border_color(ImVec4 color);

	void reset_all_styles();

} // namespace UI