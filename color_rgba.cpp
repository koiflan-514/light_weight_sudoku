#include "color_rgba.h"
namespace rgba {
	uint32_t to_ImU32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
		return IM_COL32(red, green, blue, alpha);
	}
}