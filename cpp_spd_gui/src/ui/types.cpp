#include "pch.h"
#include <ui/types.h>

namespace spd::ui{
	const Offsets Offsets::ZERO{ 0.f, 0.f, 0.f, 0.f };

	ImVec4 GetDefaultImGuiColor(ImGuiCol col) {
		return ImGui::GetStyle().Colors[col];
	}

	std::string GetAlignmentStr(Alignment alignment) {
		std::string res;

		if (alignment & Alignment::Left)
			res += "Left | ";
		if (alignment & Alignment::Right)
			res += "Right | ";
		if (alignment & Alignment::Top)
			res += "Top | ";
		if (alignment & Alignment::Bottom)
			res += "Bottom | ";
		if (alignment & Alignment::Center)
			res += "Center | ";

		if (res.ends_with(" | ")) {
			res = res.substr(0, res.size() - 3);
		}

		return res;
	}

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
