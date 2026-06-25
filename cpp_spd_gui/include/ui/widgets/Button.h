#pragma once
#include <functional>

namespace spd::ui {
	class Button : public Widget {
	public:
        static constexpr const char* DEFAULT_TAG = "unnamed button";

        Button(const std::string& text) : m_text(text) { m_tag = DEFAULT_TAG; }
        Button(const std::string& text, const char* tag) : m_text(text) { m_tag = DEFAULT_TAG; }

		// callback setter
        Button* OnClick(std::function<void()> callback);

        std::string GetText() const;
        void SetText(const std::string& newText);

	protected:
		ImVec2 OnCalcSize() override;
		void OnRender() override;

	private:
        std::string m_text;
        std::function<void()> m_onClickCallback = nullptr;
	};

    template <typename... Args>
    std::unique_ptr<Button> MakeButton(Args&&... args) {
        return std::make_unique<Button>(std::forward<Args>(args)...);
    }
}
