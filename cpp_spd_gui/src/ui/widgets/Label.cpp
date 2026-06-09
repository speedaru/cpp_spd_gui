#include "pch.h"
#include <ui/widgets/Label.h>

namespace spd::ui {
	Label::Label(const std::string& text) : m_text(text) {
		CalcSize();
	}

	void Label::Render() {
		//ImGui::Text(m_text.c_str());
        ImGui::SetCursorPos(m_position);
        ImGui::TextUnformatted(m_text.c_str());
	}

	ImVec2 Label::CalcSize() {
		//m_size = ImGui::CalcTextSize(m_text.data(), m_text.data() + m_text.length());
		m_size = { 100.f, 12.f };
		return m_size;
	}

	void Label::SetText(const std::string& text) {
		m_text = text;
		CalcSize();
	}

	std::string Label::GetText() const {
		return m_text;
	}
}
