#pragma once
#include <imgui/imgui.h>

#define IMVEC4_TO_COLOR(vec) Color({ \
	(uint8_t)(vec.x * 255), \
	(uint8_t)(vec.y * 255), \
	(uint8_t)(vec.z * 255), \
	(uint8_t)(vec.w * 255) \
})

constexpr const float COLOR_TO_FLOAT = 1 / 255.f;
#define COLOR_TO_IMVEC4(col) ImVec4({ \
	(col.r * COLOR_TO_FLOAT), \
	(col.g * COLOR_TO_FLOAT), \
	(col.b * COLOR_TO_FLOAT), \
	(col.a * COLOR_TO_FLOAT) \
})

namespace spd::utils {
	ImVec4 GetDefaultImGuiColor(ImGuiCol col);
}
