#include "pch.h"
#include <ui/widgets/Label.h>
#include <utils/imgui_utils.h>

namespace spd::ui {
	Label::Label(const std::string& text) : m_text(text) { m_tag = DEFAULT_TAG; }
	Label::Label(const std::string& text, const char* tag) : m_text(text) { m_tag = tag; }

	ImVec2 Label::OnCalcSize() {
		ImVec2 size = ImGui::CalcTextSize(m_text.data(), m_text.data() + m_text.length());
		return size;
	}

	void Label::OnRender() {
        Alignment align = m_style.alignment.value_or(Alignment::Center);

		ImVec2 contentStart = m_boxModel.CalcAlignedContentStart(m_position, align);
        ImGui::SetCursorPos(contentStart);

		ImVec4 imguiTextColor = utils::GetDefaultImGuiColor(ImGuiCol_Text);
		Color color = ResolveStyle(&Style::textColor, IMVEC4_TO_COLOR(imguiTextColor));

		ImGui::TextColored(COLOR_TO_IMVEC4(color), "%.*s", m_text.length(), m_text.data());
	}

	std::string Label::GetText() const {
		return m_text;
	}

	void Label::SetText(const std::string& text) {
		m_text = text;
	}
}
