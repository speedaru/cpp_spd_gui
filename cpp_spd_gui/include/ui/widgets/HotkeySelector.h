#pragma once
#include <functional>
#include <ui/Widget.h>

namespace spd::ui {
	class HotkeySelector : public Widget {
	public:
		static constexpr const char* DEFAULT_TAG = "unnamed hotkey selector";
		static constexpr const char* DEFAULT_LISTENING_TEXT = "Press a key...";

		HotkeySelector(const std::string& listeningText, const char* tag = DEFAULT_TAG)
			: m_listeningText(listeningText) {}

		// text to display when listening for input
		void SetListeningText(const std::string& text);

		uint32_t GetKeyCode() const;

		void SetKeyCode(uint32_t keyCode);

		// start listening for input
		void StartListening();

		// stop listening without changing the key
		void StopListening();

		bool IsListening() const;

		// set callback for when key changes
		HotkeySelector* OnKeyChanged(std::function<void(uint32_t)> callback);

		// set the key code that cancels listening (default: VK_ESCAPE)
		void SetCancelKey(uint32_t cancelKeyCode);

	protected:
		ImVec2 OnCalcSize() override;
		void OnRender() override;

	private:
		std::string GetDisplayText() const;
		void CaptureInput();
		void StopListeningInternal();

	private:
		uint32_t m_keyCode{};
		uint32_t m_cancelKeyCode{ 0x1B }; // VK_ESCAPE
		bool m_isListening{};
		std::string m_listeningText;
		std::function<void(uint32_t)> m_onKeyChangedCallback{};
		double m_listeningStartTime{};
	};

	template <typename... Args>
	std::unique_ptr<HotkeySelector> MakeHotkeySelector(Args&&... args) {
		return std::make_unique<HotkeySelector>(std::forward<Args>(args)...);
	}
}
