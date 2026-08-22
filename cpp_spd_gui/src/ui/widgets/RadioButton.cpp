#include "pch.h"
#include <ui/widgets/RadioButton.h>
#include <core/event_dispatcher.h>
#include <utils/widget_draw.h>

namespace spd::ui {
	spd::ui::RadioButton::RadioButton(const char* radioGroup, const std::string& text)
		: m_text(text), m_group(radioGroup) {
		m_tag = DEFAULT_TAG;
		if (m_group.IsValid()) {
			Register();
		}
	}

	RadioButton::RadioButton(const char* radioGroup, const std::string& text, const char* tag)
		: m_text(text), m_group(radioGroup) {
		m_tag = tag;
		if (m_group.IsValid()) {
			Register();
		}
	}

	RadioButton::~RadioButton() {
		Deregister();
	}

	RadioButton* RadioButton::OnClick(std::function<void()> callback) {
		m_onClickCallback = callback;
		return this;
	}

	std::string RadioButton::GetText() const {
		return m_text;
	}

	void RadioButton::SetText(const std::string& newText) {
		m_text = newText;
	}

	RadioGroup RadioButton::GetGroup() const {
		return m_group;
	}

	void RadioButton::SetGroup(const char* radioGroup) {
		m_group = RadioGroup(radioGroup);
	}

	ImVec2 RadioButton::OnCalcSize() {
		return ImGui::CalcTextSize(m_text.data(), m_text.data() + m_text.length());
	}

	void RadioButton::OnRender() {
		ImVec2 borderPos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;

        // handle event capture
        ImGui::SetCursorPos(borderPos);
        if (ImGui::InvisibleButton(m_id, boxSize)) {
			// activate radio button
			core::event_dispatcher::Defer([this]() { this->Activate(); });

			if (m_onClickCallback) {
				core::event_dispatcher::Defer(m_onClickCallback);
			}
        }

        // background drawing
        utils::DrawAnimatedRect(
            this, m_id, borderPos, boxSize, ImGui::IsItemHovered(), m_activated,
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

	void RadioButton::Activate() {
		if (!m_group.IsValid() || m_activated) return;

		m_activated = true;

		// deactivate all other buttons from this group
		for (auto& button : s_buttons[m_group]) {
			button->m_activated = false;
		}
	}

	void RadioButton::Register() {
		auto& buttons = s_buttons[m_group];

		// register only if not already registered
		auto it = std::find(buttons.begin(), buttons.end(), this);
		if (it != buttons.end()) {
			buttons.push_back(this);
		}
	}

	void RadioButton::Deregister() {
		auto& buttons = s_buttons[m_group];

		// if not registered
		auto it = std::find(buttons.begin(), buttons.end(), this);
		if (it == buttons.end()) {
			LOG_W("trying to deregister radio button, but it was not registered\n");
			return;
		}

		buttons.erase(it);
	}
}