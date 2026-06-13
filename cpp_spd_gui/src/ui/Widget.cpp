#include "pch.h"
#include <ui/Widget.h>
#include <utils/imgui_utils.h>

namespace spd::ui {
	void Widget::Update() {
		// resolve font from this widget or parents
		std::optional<ImFont*> resolvedFont = ResolveStyleOpt<ImFont*>(&Style::font, std::nullopt);
		if (resolvedFont.has_value()) ImGui::PushFont(resolvedFont.value());

		// wrap size calculation with font block
		ImVec2 contentSize = OnCalcSize();

		if (resolvedFont.has_value()) ImGui::PopFont();

		// recalc box
		m_boxModel.Recalculate(contentSize, m_baseSize);
	}

	void Widget::Arrange(ImVec2 finalPosition) {
		// just set absolute position
		m_position = finalPosition;
	}

	void Widget::Render() {
		// draw background for all widgets
		RenderBackground();

		// render main stuff
		std::optional<ImFont*> resolvedFont = ResolveStyleOpt<ImFont*>(&Style::font, std::nullopt);
		if (resolvedFont.has_value()) ImGui::PushFont(resolvedFont.value());

		// draw derived widget content
		OnRender();

		if (resolvedFont.has_value()) ImGui::PopFont();

		// draw border for all widgets
		RenderBorder();
	}

	void Widget::RenderBorder() {
		// if no border thickness then dont draw
		if (!m_style.borderThickness.value_or(0.f)) return;

        ImDrawList* draw = ImGui::GetWindowDrawList();

		ImVec4 defaultColor = utils::GetDefaultImGuiColor(ImGuiCol_Border);
		Color color = m_style.borderColor.value_or(IMVEC4_TO_COLOR(defaultColor));

		float thickness = m_style.borderThickness.value();
        float rounding = ResolveStyle(&Style::rounding, 0.f);

        ImVec2 pos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;
        draw->AddRect(pos, pos + boxSize, color.imu32, rounding, 0, thickness);
	}

    void Widget::RenderBackground() {
        // only draw background if bg color specified
		if (!m_style.frameBgColor.has_value()) return;

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;
        float frameRounding = m_style.frameRounding.value_or(0.f);

        draw->AddRectFilled(pos, pos + boxSize, m_style.frameBgColor.value().imu32, frameRounding);
    }

	void Widget::GenerateID() {
		char* it = m_id;

		// copy prefix ##w_
		for (size_t i{}; i < sizeof(ID_PREFIX) - 1; i++, it++) {
			*it = ID_PREFIX[i];
		}

		// copy ptr value
		static const char* hexDigits = "0123456789ABCDEF";
		uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
		for (size_t byteIdx = sizeof(void*); byteIdx-- > 0;) {
			uint8_t byte = 0xFF & (ptr >> byteIdx * 8);
			*it++ = hexDigits[byte >> 4 & 0x0F]; // high digit
			*it++ = hexDigits[byte & 0x0F]; // low digit
		}

		// null terminate
		m_id[sizeof(m_id) - 1] = '\0';
	}
}
