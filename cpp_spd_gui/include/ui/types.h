#pragma once
#include <cstdint>

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

	struct Offsets {
		Offsets(float top, float right, float bottom, float left) : top(top), right(right), bottom(bottom), left(left) {}
		Offsets(float off) : top(off), right(off), bottom(off), left(off) {}

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
}
