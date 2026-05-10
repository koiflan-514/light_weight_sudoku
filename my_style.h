#pragma once
#include "imgui.h"

namespace UI {
    // 棋盘绘制所需的主题颜色数据
    struct ThemeData {
        ImU32 grid_bg;
        ImU32 cell_bg;
        ImU32 cell_hover;
        ImU32 cell_highlight;
        ImU32 cell_select;
        ImU32 text_given;
        ImU32 text_normal;
        ImU32 text_error;
        ImU32 text_note;
        ImU32 border;
        ImU32 ripple;
        ImU32 wave;
    };

    enum class ThemeType {
        Dark = 0,
        Light,
        Matcha
    };

    extern ThemeData CurrentTheme;

    void apply_theme(ThemeType theme);

    // 动态返回当前主题的按钮颜色
    int apply_accent_button();
    int apply_dark_button();

    // 辅助：处理颜色透明度混合 (用于转场动画)
    ImU32 fade_color(ImU32 col, float alpha_mul);
}