#include "pch.h"
#include <ui/widgets/Label.h>

namespace spd::ui {
	Label::Label(const std::string& text) : m_text(text) {}
	Label::Label(const std::string& text, const char* tag) : m_text(text), m_tag(tag) {}

	void Label::OnRender() {
        ImGui::SetCursorPos(m_boxModel.CalcContentPosition(m_position));
        ImGui::TextUnformatted(m_text.c_str());
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
