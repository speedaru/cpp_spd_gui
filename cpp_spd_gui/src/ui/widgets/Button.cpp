#include "pch.h"
#include <core/event_dispatcher.h>

#include <ui/widgets/Button.h>
#include <ui/fx/Animator.h>

#include <utils/imgui_utils.h>
#include <utils/widget_draw.h>

namespace spd::ui {
    Button* spd::ui::Button::OnClick(std::function<void()> callback) {
		m_onClickCallback = callback;
		return this;
	}

	ImVec2 Button::OnCalcSize() {
		return ImGui::CalcTextSize(m_text.data(), m_text.data() + m_text.length());
	}

    void Button::OnRender() {
		ImVec2 borderPos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;

        // handle event capture
        ImGui::SetCursorPos(borderPos);
        if (ImGui::InvisibleButton(m_id, boxSize) && m_onClickCallback) {
            core::event_dispatcher::Defer(m_onClickCallback);
        }

        // background drawing
        utils::DrawAnimatedRect(
            this, m_id, borderPos, boxSize, ImGui::IsItemHovered(), ImGui::IsItemActive(),
            &Style::bgColor, &Style::hoverColor, &Style::activeColor,
            ImGuiCol_Button, ImGuiCol_ButtonHovered, ImGuiCol_ButtonActive
        );

        // text drawing
        ImVec2 textSize = ImGui::CalcTextSize(m_text.c_str());
        ImVec2 textPos = {
            borderPos.x + (boxSize.x - textSize.x) * 0.5f,
            borderPos.y + (boxSize.y - textSize.y) * 0.5f
        };

        bool textPushed = utils::PushTextColor(this);
        ImGui::SetCursorPos(textPos);
        ImGui::TextUnformatted(m_text.c_str());
        utils::PopTextColor(textPushed);
	}
}
