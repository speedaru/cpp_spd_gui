#include "pch.h"
#include <ui/Widget.h>

namespace spd::ui {
	void Widget::Update() {
		// resolve font from this widget or parents
        ImFont* resolvedFont = ResolveStyle(&Style::font, (ImFont*)nullptr);
		if (resolvedFont) ImGui::PushFont(resolvedFont);

		// wrap size calculation with font block
		ImVec2 contentSize = OnCalcSize();

		if (resolvedFont) ImGui::PopFont();

		// recalc box
		m_boxModel.Recalculate(contentSize, m_baseSize);
	}

	void Widget::Render() {
		// draw background for all widgets
		RenderBackground();

		// render main stuff
		ImFont* resolvedFont = ResolveStyle(&Style::font, (ImFont*)nullptr);
		if (resolvedFont) ImGui::PushFont(resolvedFont);

		// draw derived widget content
		OnRender();

		if (resolvedFont) ImGui::PopFont();

		// draw border for all widgets
		RenderBorder();
	}

	void Widget::RenderBorder() {
		if (!m_style.borderColor.has_value() || !m_style.borderSize.has_value() || !m_style.borderSize.value()) return;

        ImDrawList* draw = ImGui::GetWindowDrawList();
        Color borderColor = m_style.borderColor.value();
        float borderSize = m_style.borderSize.value();
        float rounding = ResolveStyle(&Style::rounding, 0.f);

        ImVec2 pos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;
        draw->AddRect(pos, pos + boxSize, borderColor.imu32, rounding, 0, borderSize);
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
		m_id = "##Widget_" + std::to_string(reinterpret_cast<uintptr_t>(this));
	}
}
