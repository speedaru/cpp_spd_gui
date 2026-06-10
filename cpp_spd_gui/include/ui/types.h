#pragma once
#include <cstdint>

typedef unsigned int        ImU32;  // 32-bit unsigned integer (often used to store packed colors)

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

namespace spd::ui {
	union Color {
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		ImU32 imu32;
	};

	struct Offsets {
		Offsets(float off) : top(off), right(off), bottom(off), left(off) {}
		Offsets(float x, float y) : top(y), right(x), bottom(y), left(x) {}
		Offsets(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}

		float top;
		float right;
		float bottom;
		float left;

		static const Offsets ZERO;

		// horizontal size
		inline constexpr float Width() const { return left + right; }
		// vertical size
		inline constexpr float Height() const { return top + bottom; }
	};

	enum class Alignment : uint8_t {
		Left = 1 << 0,
		Right = 1 << 1,
		Top = 1 << 2,
		Bottom = 1 << 3,
		Center = 1 << 4,

		TopLeft = Top | Left,
		TopRight = Top | Right,
		BottomLeft = Bottom | Left,
		BottomRight = Bottom | Right,
	};

	ImVec4 GetDefaultImGuiColor(ImGuiCol col);

	std::string GetAlignmentStr(Alignment alignment);

	float CalcAlignmentX(float layoutWidth, float contentWidth, Alignment alignment);
	float CalcAlignmentY(float layoutHeight, float contentHeight, Alignment alignment);
	ImVec2 CalcAlignmentPos(ImVec2 layoutSize, ImVec2 contentSize, Alignment alignment);

	static Alignment operator|(Alignment a, Alignment b) { return static_cast<Alignment>((uint8_t)a | (uint8_t)b); }
	static bool operator&(Alignment a, Alignment b) { return (uint8_t)a & (uint8_t)b; }
}
