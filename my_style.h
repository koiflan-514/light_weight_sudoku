#pragma once
#include "imgui.h"

namespace UI {

	void apply_win11_dark_theme();
	void apply_win11_accent_button();
	void apply_win11_dark_button();

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