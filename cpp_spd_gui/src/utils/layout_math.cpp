#include "pch.h"
#include <utils/layout_math.h>

namespace spd::utils {
	float CalcAlignmentX(float containerWidth, float itemWidth, Alignment alignment) {
		float halfX = containerWidth / 2.f - itemWidth / 2.f;

		// by default equivalent to Alignment::Center
		float x = halfX;

		if (alignment & Alignment::Left)
			x -= halfX;
		if (alignment & Alignment::Right)
			x += halfX;

		return x;
	}

	float CalcAlignmentY(float containerHeight, float itemHeight, Alignment alignment) {
		float halfY = containerHeight / 2.f - itemHeight / 2.f;

		// by default equivalent to Alignment::Center
		float y = halfY;

		if (alignment & Alignment::Top)
			y -= halfY;
		if (alignment & Alignment::Bottom)
			y += halfY;

		return y;
	}

	ImVec2 CalcAlignmentPos(ImVec2 containerSize, ImVec2 itemSize, Alignment alignment) {
		return {
			CalcAlignmentX(containerSize.x, itemSize.x, alignment),
			CalcAlignmentY(containerSize.y, itemSize.y, alignment),
		};
	}
}