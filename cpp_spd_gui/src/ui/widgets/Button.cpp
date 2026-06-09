#include "pch.h"
#include <ui/widgets/Button.h>

namespace spd::ui {
	Button* spd::ui::Button::OnClick(std::function<void()> callback) {
		m_onClickCallback = callback;
		return this;
	}

	void Button::OnRender() {
		ImVec2 borderPos = m_boxModel.CalcBorderPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;

        ImGui::SetCursorPos(borderPos);

        // invisible imgui button for click detection
        bool pressed = ImGui::InvisibleButton(m_id.c_str(), boxSize);

        // interaction state
        bool isHovered = ImGui::IsItemHovered();
        bool isActive = ImGui::IsItemActive(); // mouse held down

        // resolve background color
        ImVec4 imguiButtonColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
        Color bgColor = m_style.bgColor.value_or(IMVEC4_TO_COLOR(imguiButtonColor)); // default imgui button color

        // use default imgui colors if no styles specified
        if (isActive) {
			const ImVec4& imguiButtonActive = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
            bgColor = m_style.activeColor.value_or(IMVEC4_TO_COLOR(imguiButtonActive));
        }
        else if (isHovered) {
			const ImVec4& imguiButtonHovered = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            bgColor = m_style.hoverColor.value_or(IMVEC4_TO_COLOR(imguiButtonHovered));
        }

        // draw backgrond
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float rounding = m_style.rounding.value_or(0.0f);
        
        ImVec2 screenPos = ImGui::GetItemRectMin(); 
        drawList->AddRectFilled(screenPos, { screenPos.x + boxSize.x, screenPos.y + boxSize.y }, bgColor.imu32, rounding);

        // draw text (centered inside button)
        ImVec2 textSize = ImGui::CalcTextSize(m_text.c_str());
        ImVec2 textPos = {
            borderPos.x + (boxSize.x - textSize.x) * 0.5f,
            borderPos.y + (boxSize.y - textSize.y) * 0.5f
        };

        if (m_style.textColor.has_value()) ImGui::PushStyleColor(ImGuiCol_Text, m_style.textColor.value().imu32);
        
        ImGui::SetCursorPos(textPos);
        ImGui::TextUnformatted(m_text.c_str());

        if (m_style.textColor.has_value()) ImGui::PopStyleColor();

        // call click callback if clicked
        if (pressed && m_onClickCallback) {
            m_onClickCallback();
        }
	}

	ImVec2 Button::OnCalcSize() {
		return ImGui::CalcTextSize(m_text.data(), m_text.data() + m_text.length());
	}
}
