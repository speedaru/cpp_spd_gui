#include "pch.h"
#include <ui/types.h>

namespace spd::ui{
	const Offsets Offsets::ZERO{ 0.f, 0.f, 0.f, 0.f };

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
}
