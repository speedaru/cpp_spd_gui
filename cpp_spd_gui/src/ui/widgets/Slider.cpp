#include "pch.h"
#include <core/event_dispatcher.h>
#include <ui/widgets/Slider.h>
#include <ui/fx/Animator.h>
#include <utils/imgui_utils.h>
#include <utils/widget_draw.h>

#include <imgui/imgui_internal.h>

namespace spd::ui {
	Slider::Slider(float minValue, float maxValue, float step, const char* tag)
		: m_minValue(minValue), m_maxValue(maxValue), m_value(minValue) {
		// convert step to string format precision
		int decimalPlaces = 0;
		while (step < 0.9999f) {
			decimalPlaces++;
			step *= 10.f;
		}
		snprintf(m_stepFmt, sizeof(m_stepFmt) - 1, "%%.%df", decimalPlaces);

		m_tag = tag;
	}

	Slider* Slider::OnChange(std::function<void(float)> callback) {
		m_onChangeCallback = callback;
		return this;
	}

	ImVec2 Slider::OnCalcSize() {
		ImVec2 size = ImGui::CalcTextSize("100"); // placeholder text just to cal size
		size.x = 150.0f; // min silze
		return size;
	}

	void Slider::OnRender() {
		ImVec2 borderPos = m_boxModel.CalcBoxPosition(m_position);
		ImVec2 boxSize = m_boxModel.boxSize;

		//// slider pos and size is the same as border and box but x is offset by padding
		//ImVec2 sliderPos = { m_boxModel.CalcContentPosition(m_position).x, borderPos.y };
		//ImVec2 sliderSize = { m_boxModel.GetContentAreaSize().x, boxSize.y };

		bool isHovered = ImGui::IsMouseHoveringRect(borderPos, { borderPos.x + boxSize.x, borderPos.y + boxSize.y });
		bool isActive = (ImGui::GetActiveID() == ImGui::GetID(m_id));

		// background
		utils::DrawAnimatedRect(
			this, m_id, borderPos, boxSize, isHovered, isActive,
			&Style::bgColor, &Style::hoverColor, &Style::activeColor,
			ImGuiCol_FrameBg, ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive
		);

		// slider control
		//ImGui::SetCursorPos(sliderPos);
		ImGui::SetCursorPos(borderPos);
		const Offsets& padding = m_style.padding.value_or(Offsets::ZERO);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding.left, padding.top));
		ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12.f);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0); 
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, 0);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, 0);

		// text color + slider width
		bool textPushed = utils::PushTextColor(this);
		//ImGui::PushItemWidth(sliderSize.x);
		ImGui::PushItemWidth(boxSize.x);

		// draw slider
		float prevValue = m_value;
		ImGui::SliderFloat(m_id, &m_value, m_minValue, m_maxValue, m_stepFmt);
		m_value = std::clamp(m_value, m_minValue, m_maxValue);

		ImGui::PopItemWidth();
		utils::PopTextColor(textPushed);
		ImGui::PopStyleColor(3); // frame colors
		ImGui::PopStyleVar(2); // frame padding + grab rounding

		// event handling
		if (m_value != prevValue && m_onChangeCallback) {
			core::event_dispatcher::Defer(m_onChangeCallback, m_value);
		}
	}
}
