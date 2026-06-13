#pragma once
#include <optional>
#include <imgui/imgui.h>
#include <ui/types.h>

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
		static constexpr const bool DEFAULT_ENABLE_ANIMATIONS{ true };
		static constexpr const float DEFAULT_ANIMATION_SPEED{ 12.f };

		// visuals
		ADD_FIELD(Alignment, alignment, SetAlignment);
		ADD_FIELD(bool, hgrow, SetHgrow);
		ADD_FIELD(bool, vgrow, SetVgrow);
		ADD_FIELD(float, spacing, SetSpacing);

		// fx
		ADD_FIELD(bool, enableAnimations, SetEnableAnimations);
		ADD_FIELD(float, animationSpeed, SetAnimationSpeed);

		// layout
		ADD_FIELD_REF(Offsets, padding, SetPadding);
		ADD_FIELD_REF(Offsets, margin, SetMargin);

		// containers
		ADD_FIELD(Color, frameBgColor, SetFrameBgColor);
		ADD_FIELD(float, frameRounding, SetFrameRounding);

		// buttons and widgets
		ADD_FIELD(Color, bgColor, SetBgColor);
		ADD_FIELD(float, rounding, SetRounding);
		ADD_FIELD(Color, borderColor, SetBorderColor);
		ADD_FIELD(float, borderThickness, SetBorderThickness);

		// text
		ADD_FIELD(Color, textColor, SetTextColor);
		ADD_FIELD(float, textSize, SetTextSize);
		ADD_FIELD(ImFont*, font, SetFont);

		// interactive
		ADD_FIELD(Color, hoverColor, SetHoverColor);
		ADD_FIELD(Color, activeColor, SetActiveColor);
	};
}
