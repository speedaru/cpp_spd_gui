#pragma once
#include <cstdint>
#include <string>
#include <imgui/imgui.h>

typedef unsigned int        ImU32;  // 32-bit unsigned integer (often used to store packed colors)

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

		Default = Center,
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

	struct Border {
		Color color{ 255, 255, 255, 255 };
		float thickness{ 0.f };
	};

	std::string GetAlignmentStr(Alignment alignment);

	static Alignment operator|(Alignment a, Alignment b) { return static_cast<Alignment>((uint8_t)a | (uint8_t)b); }
	static bool operator&(Alignment a, Alignment b) { return (uint8_t)a & (uint8_t)b; }
}
