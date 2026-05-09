#pragma once
#include <cstdint>
#include "imgui.h"
namespace rgba {
    constexpr float to_float(uint8_t color) { return color / 255.0f; }
    inline ImVec4 to_ImVec4(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) {
        return ImVec4(to_float(red), to_float(green), to_float(blue), to_float(alpha));
    }
    uint32_t to_ImU32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
};