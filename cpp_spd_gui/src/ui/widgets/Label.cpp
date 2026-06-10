#include "pch.h"
#include <ui/widgets/Label.h>

namespace spd::ui {
	Label::Label(const std::string& text) : m_text(text) {}
	Label::Label(const std::string& text, const char* tag) : m_text(text), m_tag(tag) {}

	void Label::OnRender() {
        Alignment align = m_style.alignment.value_or(Alignment::Center);

		ImVec2 contentStart = m_boxModel.CalcContentPosition(m_position);
		ImVec2 contentArea = m_boxModel.GetContentAreaSize();
		//LOG_D("label content area: %.2f %.2f, content size: %.2f %.2f\n");
		contentStart += CalcAlignmentPos(contentArea, m_boxModel.contentSize, align);
        ImGui::SetCursorPos(contentStart);

		ImVec4 imguiTextColor = GetDefaultImGuiColor(ImGuiCol_Text);
		Color color = ResolveStyle(&Style::textColor, IMVEC4_TO_COLOR(imguiTextColor));

		ImGui::TextColored(COLOR_TO_IMVEC4(color), "%.*s", m_text.length(), m_text.data());
	}

	ImVec2 Label::OnCalcSize() {
		ImVec2 size = ImGui::CalcTextSize(m_text.data(), m_text.data() + m_text.length());
		return size;
	}

	std::string Label::GetText() const {
		return m_text;
	}

	void Label::SetText(const std::string& text) {
		m_text = text;
	}
}
