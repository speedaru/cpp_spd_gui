#pragma once
#include <functional>
#include <ui/widgets/Label.h>

namespace spd::ui {
	class Button : public Label {
	public:
        static constexpr const char* DEFAULT_TAG = "unnamed button";

        Button(const std::string& text) : Label(text, DEFAULT_TAG) {}
        Button(const std::string& text, const char* tag) : Label(text, tag) {}

		// callback setter
        Button* OnClick(std::function<void()> callback);

	protected:
		ImVec2 OnCalcSize() override;
		void OnRender() override;

	private:
        std::function<void()> m_onClickCallback = nullptr;
	};

    template <typename... Args>
    std::unique_ptr<Button> MakeButton(Args&&... args) {
        return std::make_unique<Button>(std::forward<Args>(args)...);
    }
}
