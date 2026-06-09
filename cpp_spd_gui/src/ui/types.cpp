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

	float CalcAlignmentX(float layoutWidth, float contentWidth, Alignment alignment) {
		float halfX = layoutWidth / 2.f - contentWidth / 2.f;

		// by default equivalent to Alignment::Center
		float x = halfX;

		if (alignment & Alignment::Left)
			x -= halfX;
		if (alignment & Alignment::Right)
			x += halfX;

		return x;
	}

	float CalcAlignmentY(float layoutHeight, float contentHeight, Alignment alignment) {
		float halfY = layoutHeight / 2.f - contentHeight / 2.f;

		// by default equivalent to Alignment::Center
		float y = halfY;

		if (alignment & Alignment::Top)
			y -= halfY;
		if (alignment & Alignment::Bottom)
			y += halfY;

		return y;
	}

	ImVec2 CalcAlignmentPos(ImVec2 layoutSize, ImVec2 contentSize, Alignment alignment) {
		return {
			CalcAlignmentX(layoutSize.x, contentSize.x, alignment),
			CalcAlignmentY(layoutSize.y, contentSize.y, alignment),
		};
	}
}
