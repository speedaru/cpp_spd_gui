#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <ui/Widget.h>

namespace spd::ui {
	// string of max 15 chars
	struct RadioGroup {
		static constexpr const size_t MAX_LEN = 15;
		char buff[MAX_LEN];
		uint8_t length;

		RadioGroup(const char* str) {
			size_t len = strlen(str);

			// copy buff and set len
			memcpy(buff, str, std::min(len, MAX_LEN));
			length = static_cast<uint8_t>(len);

			if (len > MAX_LEN) {
				LOG_W("radio button group name too long: %s (max %zu)\n", str, MAX_LEN);
			}
		}

		bool operator==(const RadioGroup& other) const {
			if (!length || length != other.length) return false;

			// compare buffers
			return memcmp(buff, other.buff, length) == 0;
		}

		inline bool IsValid() const { return length > 0; }
	};

	struct RadioGroupHash {
		std::size_t operator()(const RadioGroup& group) const {
			return std::hash<std::string_view>{}(
				std::string_view(group.buff, group.length)
			);
		}
	};

	class RadioButton : public Widget {
	public:
        static constexpr const char* DEFAULT_TAG = "unnamed radio button";

        RadioButton(const char* radioGroup, const std::string& text);
		RadioButton(const char* radioGroup, const std::string& text, const char* tag);

        // remove button from group
        ~RadioButton() override;

		// callback setter
        RadioButton* OnClick(std::function<void()> callback);

        std::string GetText() const;
        void SetText(const std::string& newText);

        RadioGroup GetGroup() const;
        void SetGroup(const char* radioGroup);

	protected:
		ImVec2 OnCalcSize() override;
		void OnRender() override;

    private:
		// activate this button, and deactivate others from same group
		void Activate();

        // register this button into static list
        void Register();

        // removes this button from static list
        void Deregister();

	private:
        // map of all radio button groups with the list of buttons in that group
		inline static std::unordered_map<RadioGroup, std::vector<RadioButton*>, RadioGroupHash> s_buttons;

        RadioGroup m_group;
        std::string m_text;
		bool m_activated{};
		std::function<void()> m_onClickCallback{};
	};

    template <typename... Args>
    std::unique_ptr<RadioButton> MakeRadioButton(Args&&... args) {
        return std::make_unique<RadioButton>(std::forward<Args>(args)...);
    }
}
