#include "pch.h"
#include <ui/widgets/HotkeySelector.h>
#include <ui/fx/Animator.h>
#include <utils/imgui_utils.h>
#include <utils/widget_draw.h>
#include <utils/keycode_utils.h>
#include <core/event_dispatcher.h>

namespace spd::ui {
	void HotkeySelector::SetListeningText(const std::string& text) {
		m_listeningText = text;
	}

	uint32_t HotkeySelector::GetKeyCode() const {
		return m_keyCode;
	}

	void HotkeySelector::SetKeyCode(uint32_t keyCode) {
		if (m_keyCode != keyCode) {
			m_keyCode = keyCode;
			if (m_onKeyChangedCallback) {
				core::event_dispatcher::Defer([this, keyCode]() { m_onKeyChangedCallback(keyCode); });
			}
		}
	}

	void HotkeySelector::StartListening() {
		m_isListening = true;
		m_listeningStartTime = ImGui::GetTime();
	}

	void HotkeySelector::StopListening() {
		StopListeningInternal();
	}

	bool HotkeySelector::IsListening() const {
		return m_isListening;
	}

	HotkeySelector* HotkeySelector::OnKeyChanged(std::function<void(uint32_t)> callback) {
		m_onKeyChangedCallback = callback;
		return this;
	}

	void HotkeySelector::SetCancelKey(uint32_t cancelKeyCode) {
		m_cancelKeyCode = cancelKeyCode;
	}

	ImVec2 HotkeySelector::OnCalcSize() {
		std::string displayText = GetDisplayText();
		ImVec2 size = ImGui::CalcTextSize(displayText.data(), displayText.data() + displayText.size());
		size.x = std::max(150.f, size.x); // at least 50px
		return size;
	}

	void HotkeySelector::OnRender() {
		ImVec2 borderPos = m_boxModel.CalcBoxPosition(m_position);
		ImVec2 boxSize = m_boxModel.boxSize;

		// input capture region
		ImGui::SetCursorPos(borderPos);
		bool clicked = false;
		clicked = ImGui::InvisibleButton(m_id, boxSize);

		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();

		// start listening on click
		if (clicked && !m_isListening) {
			auto fn = &HotkeySelector::StartListening;
			core::event_dispatcher::Defer([fn, this]() { (this->*fn)(); });
		}

		// capture key input while listening
		if (m_isListening) {
			auto fn = &HotkeySelector::CaptureInput;
			core::event_dispatcher::Defer([fn, this]() { (this->*fn)(); });
		}

		// background drawing
		utils::DrawAnimatedRect(
			this, m_id, borderPos, boxSize, hovered, active,
			&Style::bgColor, &Style::hoverColor, &Style::activeColor,
			ImGuiCol_Button, ImGuiCol_ButtonHovered, ImGuiCol_ButtonActive
		);

		// text drawing
		std::string displayText = GetDisplayText();
		ImVec2 textSize = ImGui::CalcTextSize(displayText.data(), displayText.data() + displayText.size());
		ImVec2 textPos = {
			borderPos.x + (boxSize.x - textSize.x) * 0.5f,
			borderPos.y + (boxSize.y - textSize.y) * 0.5f
		};

		bool textPushed = utils::PushTextColor(this);
		ImGui::SetCursorPos(textPos);
		ImGui::TextUnformatted(displayText.data(), displayText.data() + displayText.size());
		utils::PopTextColor(textPushed);
	}

	std::string HotkeySelector::GetDisplayText() const {
		return m_isListening
			? m_listeningText
			: std::format("selected key: {}", utils::GetKeyName(m_keyCode));
	}

	void HotkeySelector::CaptureInput() {
		ImGuiIO& io = ImGui::GetIO();

		// keys to check
		const uint32_t keyCodesToCheck[] = {
			// special keys
			VK_ESCAPE, VK_TAB, VK_RETURN, VK_BACK, VK_DELETE, VK_INSERT,
			VK_HOME, VK_END, VK_PRIOR, VK_NEXT, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT,
			VK_SPACE, VK_SHIFT, VK_CONTROL, VK_MENU,

			// function keys
			VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8,
			VK_F9, VK_F10, VK_F11, VK_F12,

			// number pad
			VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4,
			VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,
			VK_MULTIPLY, VK_ADD, VK_SUBTRACT, VK_DECIMAL, VK_DIVIDE,

			// mouse buttons
			VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2,

			// letters
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
			'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
			'U', 'V', 'W', 'X', 'Y', 'Z',

			// numbers
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

			// symbol keys
			VK_OEM_1, VK_OEM_PLUS, VK_OEM_COMMA, VK_OEM_MINUS, VK_OEM_PERIOD,
			VK_OEM_2, VK_OEM_3, VK_OEM_4, VK_OEM_5, VK_OEM_6, VK_OEM_7,
		};

		// check first if cancel key pressed
		if ((GetAsyncKeyState(m_cancelKeyCode) & 0x8000)) {
			StopListeningInternal();
			return;
		}

		for (uint32_t keyCode : keyCodesToCheck) {
			// check other keycodes
			if (GetAsyncKeyState(keyCode) & 0x8000) {
				SetKeyCode(keyCode);
				StopListeningInternal();
				return;
			}
		}
	}

	void HotkeySelector::StopListeningInternal() {
		m_isListening = false;
		m_listeningStartTime = 0.f;
	}
}
