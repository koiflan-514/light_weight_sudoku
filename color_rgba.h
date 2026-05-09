#pragma once
#include <cstdint>
#include "imgui.h"
namespace rgba {
	ImVec4 to_ImVec4(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	uint32_t to_ImU32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	float to_float(uint8_t color);
};