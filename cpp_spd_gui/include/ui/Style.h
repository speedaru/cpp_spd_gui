#pragma once
#include <optional>
#include <imgui/imgui.h>
#include <ui/Types.h>

namespace spd::ui {
	// pass arg to setter by val
	#define ADD_FIELD(T, name, setter) \
		std::optional<T> name; \
		Style& setter(T newVal) { name = newVal; return *this; }

	// pass arg to setter by ref
	#define ADD_FIELD_REF(T, name, setter) \
		std::optional<T> name; \
		Style& setter(const T& newVal) { name = newVal; return *this; }

	struct Style {
		// layout
		ADD_FIELD(float, spacing, SetSpacing);
		ADD_FIELD(Alignment, alignment, SetAlignment);
		ADD_FIELD_REF(Offsets, padding, SetPadding);
		ADD_FIELD_REF(Offsets, margin, SetMargin);

		// visuals
		ADD_FIELD_REF(Color, bgColor, SetBgColor);
		ADD_FIELD_REF(Color, textColor, SetTextColor);
		ADD_FIELD_REF(Color, borderColor, SetBorderColor);
		ADD_FIELD(float, rounding, SetRounding);
		ADD_FIELD(float, borderSize, SetBorderSize);

		// interactive
		ADD_FIELD_REF(Color, hoverColor, SetHoverColor);
		ADD_FIELD_REF(Color, activeColor, SetActiveColor);
	};
}
