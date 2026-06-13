#include "pch.h"
#include <ui/fx/Animator.h>
#include <imgui/imgui_internal.h>
#include <utils/layout_math.h>

namespace spd::ui::fx {
    Color AnimateColor(ImGuiID widgetId, const char* propertyName, Color targetColor, float speed) {
        ImGuiID propertyHash = ImHashStr(propertyName, 0, widgetId);
        
        // use imgui internal storage to avoid allocations
        ImGuiStorage* storage = ImGui::GetStateStorage();
        
        ImU32 currentPacked = storage->GetInt(propertyHash, targetColor.imu32);
        Color current{ .imu32 = currentPacked };
        
        // lerp
        float deltaTime = ImGui::GetIO().DeltaTime;
        float t = std::min(speed * deltaTime, 1.0f);
        Color blended = utils::LerpColor(current, targetColor, t);
  //      LOG_D("current color: %hhu %hhu %hhu %hhu target color: %hhu %hhu %hhu %hhu\n",
  //          current.r, current.g, current.b, current.a,
  //          targetColor.r, targetColor.g, targetColor.b, targetColor.a
		//);
        
        // pack data in imu32 colors
        storage->SetInt(propertyHash, blended.imu32);
        return blended;
    }

    float AnimateFloat(ImGuiID widgetId, const char* propertyName, float targetValue, float speed) {
        ImGuiID propertyHash = ImHashStr(propertyName, 0, widgetId);

        ImGuiStorage* storage = ImGui::GetStateStorage();
        float current = storage->GetFloat(propertyHash, targetValue);

        float deltaTime = ImGui::GetIO().DeltaTime;
        float t = std::min(speed * deltaTime, 1.0f);
        float blended = utils::Lerp(current, targetValue, t);
        
        storage->SetFloat(propertyHash, blended);
        return blended;
    }
}
