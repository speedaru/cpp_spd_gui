#pragma once
#include <imgui/imgui.h>
#include <ui/types.h>

namespace spd::ui::fx {
	// color animator
	Color AnimateColor(ImGuiID widgetId, const char* propertyName, Color targetColor, float speed);

    // float animator (for size, rounding, alphas)
    float AnimateFloat(ImGuiID widgetId, const char* propertyName, float targetValue, float speed);
}
