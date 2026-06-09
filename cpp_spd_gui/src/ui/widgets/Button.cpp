#include "pch.h"
#include <ui/widgets/Button.h>

namespace spd::ui {
	Button* spd::ui::Button::OnClick(std::function<void()> callback) {
		m_onClickCallback = callback;
		return this;
	}

	void Button::OnRender() {
		ImVec2 borderPos = GetBorderPosition();
        ImVec2 innerSize = GetInnerBorderSize();

        ImGui::SetCursorPos(borderPos);

        // 2. Create the Invisible Button for hit detection
        // We use our unique m_id so ImGui never confuses two buttons with the same text
        bool pressed = ImGui::InvisibleButton(m_id.c_str(), innerSize);

        // 3. Track Interaction State
        bool isHovered = ImGui::IsItemHovered();
        bool isActive = ImGui::IsItemActive(); // True while mouse button is held down

        // 4. Resolve Background Color
        Color bgColor = m_style.bgColor.value_or(Color{ 40, 40, 40, 255 }); // Default dark gray
        if (isActive && m_style.activeColor.has_value()) {
            bgColor = m_style.activeColor.value();
        } else if (isHovered && m_style.hoverColor.has_value()) {
            bgColor = m_style.hoverColor.value();
        } else if (isHovered && !m_style.hoverColor.has_value()) {
            // Fallback hover logic if user didn't specify one: make it slightly brighter
            bgColor.r = std::min(255, bgColor.r + 20);
            bgColor.g = std::min(255, bgColor.g + 20);
            bgColor.b = std::min(255, bgColor.b + 20);
        }

        // 5. Draw the custom background
        // Note: Using GetWindowDrawList() so it sits properly behind ImGui tooltips/popups
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float rounding = m_style.rounding.value_or(0.0f);
        
        // Grab the absolute screen position that ImGui just calculated for our cursor
        ImVec2 screenPos = ImGui::GetItemRectMin(); 
        drawList->AddRectFilled(screenPos, {screenPos.x + innerSize.x, screenPos.y + innerSize.y}, bgColor.imu32, rounding);

        // 6. Draw the text (Centered mathematically inside the button)
        ImVec2 textSize = ImGui::CalcTextSize(m_text.c_str());
        ImVec2 textPos = {
            borderPos.x + (innerSize.x - textSize.x) * 0.5f,
            borderPos.y + (innerSize.y - textSize.y) * 0.5f
        };

        if (m_style.textColor.has_value()) ImGui::PushStyleColor(ImGuiCol_Text, m_style.textColor.value().imu32);
        
        ImGui::SetCursorPos(textPos);
        ImGui::TextUnformatted(m_text.c_str());

        if (m_style.textColor.has_value()) ImGui::PopStyleColor();

        // 7. Fire the callback event!
        if (pressed && m_onClickCallback) {
            m_onClickCallback();
        }
	}

	ImVec2 Button::OnCalcSize() {
		return ImGui::CalcTextSize(m_text.data(), m_text.data() + m_text.length());
	}
}
