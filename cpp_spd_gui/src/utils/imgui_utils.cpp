#include "pch.h"
#include <utils/imgui_utils.h>

namespace spd::utils {
	ImVec4 GetDefaultImGuiColor(ImGuiCol col) {
		return ImGui::GetStyle().Colors[col];
	}
}