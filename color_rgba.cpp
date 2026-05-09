#include "color_rgba.h"

float rgba::to_float(uint8_t color) {
	return color / 255.0f;
}

ImVec4 rgba::to_ImVec4(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	return ImVec4(rgba::to_float(red), rgba::to_float(green), rgba::to_float(blue), rgba::to_float(alpha));
}

uint32_t to_ImU32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	return IM_COL32(red, green, blue, alpha);
}