#pragma once
#include <algorithm>
#include <ui/types.h>
using namespace spd::ui;

namespace spd::utils {
    // standard scalar linear interpolation
    inline float Lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    // component wise linear interpolation for structured packed color blocks
    inline Color LerpColor(Color a, Color b, float t) {
        Color result;
        result.r = static_cast<uint8_t>(std::clamp(Lerp(a.r, b.r, t), 0.f, 255.f));
        result.g = static_cast<uint8_t>(std::clamp(Lerp(a.g, b.g, t), 0.f, 255.f));
        result.b = static_cast<uint8_t>(std::clamp(Lerp(a.b, b.b, t), 0.f, 255.f));
        result.a = static_cast<uint8_t>(std::clamp(Lerp(a.a, b.a, t), 0.f, 255.f));
        return result;
    }

	// calc x position of an item based on its width and a container's width
	float CalcAlignmentX(float containerWidth, float itemWidth, Alignment alignment);

	// calc y position of an item based on its height and a container's height
	float CalcAlignmentY(float containerHeight, float itemHeight, Alignment alignment);

	// calc position of an item based on its size and a container's size
	inline ImVec2 CalcAlignmentPos(ImVec2 containerSize, ImVec2 itemSize, Alignment alignment) {
		return {
			CalcAlignmentX(containerSize.x, itemSize.x, alignment),
			CalcAlignmentY(containerSize.y, itemSize.y, alignment),
		};
	}
}
