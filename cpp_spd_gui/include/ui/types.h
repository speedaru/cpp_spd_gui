#pragma once
#include <cstdint>
#include <string>
#include <imgui/imgui.h>

namespace spd::ui {
	union Color {
		struct {
			uint8_t r, g, b, a;
		};
		ImU32 imu32;
	};

	namespace Colors {
		inline constexpr Color Red = { 255, 0, 0, 255 };
		inline constexpr Color Yellow = { 255, 255, 0, 255 };
		inline constexpr Color Pink = { 255, 0, 255, 255 };

		inline constexpr Color Green = { 0, 128, 0, 255 };
		inline constexpr Color Lime = { 0, 255, 0, 255 };

		inline constexpr Color Cyan = { 0, 255, 255, 255 };
		inline constexpr Color Blue = { 0, 0, 255, 255 };
	}

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

	std::string GetAlignmentStr(Alignment alignment);

	static Alignment operator|(Alignment a, Alignment b) { return static_cast<Alignment>((uint8_t)a | (uint8_t)b); }
	static bool operator&(Alignment a, Alignment b) { return (uint8_t)a & (uint8_t)b; }
}
