#include "pch.h"
#include <utils/widget_draw.h>
#include <ui/fx/Animator.h>
#include <utils/imgui_utils.h>
#include <imgui/imgui_internal.h>
using namespace spd::ui;

namespace spd::utils {
    void DrawAnimatedRect(
        const Widget* widget,
        const char* widgetId,
        ImVec2 position,
        ImVec2 size,
        bool isHovered,
        bool isActive,
        std::optional<Color> ui::Style::* normalColorProp,
        std::optional<Color> ui::Style::* hoverColorProp,
        std::optional<Color> ui::Style::* activeColorProp,
        ImGuiCol fallbackBase,
        ImGuiCol fallbackHover,
        ImGuiCol fallbackActive
    ) {
        // resolve target bg colors
        ImVec4 defaultBaseCol = GetDefaultImGuiColor(fallbackBase);
        ui::Color targetColor = widget->ResolveStyle(normalColorProp, IMVEC4_TO_COLOR(defaultBaseCol));

        if (isActive) {
            ImVec4 defaultActiveCol = utils::GetDefaultImGuiColor(fallbackActive);
            targetColor = widget->ResolveStyle(activeColorProp, IMVEC4_TO_COLOR(defaultActiveCol));
        }
        else if (isHovered) {
            ImVec4 defaultHoverCol = utils::GetDefaultImGuiColor(fallbackHover);
            targetColor = widget->ResolveStyle(hoverColorProp, IMVEC4_TO_COLOR(defaultHoverCol));
        }

        float targetRounding = widget->ResolveStyle(&ui::Style::rounding, 0.0f);

        // animation
        bool animationsEnabled = widget->ResolveStyle(&ui::Style::enableAnimations, ui::Style::DEFAULT_ENABLE_ANIMATIONS);
        float animSpeed = widget->ResolveStyle(&ui::Style::animationSpeed, ui::Style::DEFAULT_ANIMATION_SPEED);

        Color finalColor;
        float finalRounding;

        if (animationsEnabled) {
            ImGuiID numericId = ImGui::GetID(widgetId);
            finalColor = fx::AnimateColor(numericId, "rect_bg", targetColor, animSpeed);
            finalRounding = targetRounding;
        }
        else {
            finalColor = targetColor;
            finalRounding = targetRounding;
        }

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(position, { position.x + size.x, position.y + size.y }, finalColor.imu32, finalRounding);
    }

    bool PushTextColor(const Widget* widget) {
        std::optional<Color> textColor = widget->ResolveStyleOpt<Color>(&ui::Style::textColor, std::nullopt);
        if (!textColor.has_value()) {
			return false;
        }

		ImGui::PushStyleColor(ImGuiCol_Text, textColor.value().imu32);
		return true;
    }

    void PopTextColor(bool pushed) {
        if (pushed) {
            ImGui::PopStyleColor();
        }
    }

}