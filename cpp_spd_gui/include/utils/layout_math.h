#pragma once
#include <ui/types.h>
using namespace spd::ui;

namespace spd::utils {
	// calc x position of an item based on its width and a container's width
	float CalcAlignmentX(float containerWidth, float itemWidth, Alignment alignment);

	// calc y position of an item based on its height and a container's height
	float CalcAlignmentY(float containerHeight, float itemHeight, Alignment alignment);

	// calc position of an item based on its size and a container's size
	ImVec2 CalcAlignmentPos(ImVec2 containerSize, ImVec2 itemSize, Alignment alignment);
}
