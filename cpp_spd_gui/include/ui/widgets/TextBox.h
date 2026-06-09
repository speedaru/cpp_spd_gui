#pragma once
#include <ui/Widget.h>

namespace spd::ui {
	class TextBox : public Widget {
	public:
        // tag is used for imgui id generation
        TextBox(const std::string& placeholder = "");

        // builder properties
        TextBox* OnChange(std::function<void(const std::string&)> callback);
        TextBox* SetPassword(bool isPassword) { m_isPassword = isPassword; return this; }

        std::string GetText() const;
        void SetText(const std::string& text);

    protected:
        void OnRender() override;
        ImVec2 OnCalcSize() override;

    private:
        char m_buffer[256] = { 0 };
        std::string m_placeholder;
        bool m_isPassword = false;
        std::function<void(const std::string&)> m_onChangeCallback = nullptr;
	};

    template <typename... Args>
    std::unique_ptr<TextBox> MakeTextBox(Args&&... args) {
        return std::make_unique<TextBox>(std::forward<Args>(args)...);
    }
}
