#pragma once
#include <string>
#include <ui/Widget.h>
#include <ui/types.h>
#include <imgui/imgui.h>
using namespace spd::ui;

namespace spd::utils {
    // Automatically resolves interactive style configurations, calculates animations, 
    // and draws a clean filled backdrop rectangle to the current window draw list
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
    );

    // Standardizes optional text color cascading up the tree. 
    // Returns true if a custom color context was pushed to ImGui's stack
    bool PushTextColor(const Widget* widget);
    
    // Safely pops the color scope context if it was previously pushed
    void PopTextColor(bool pushed);

}
