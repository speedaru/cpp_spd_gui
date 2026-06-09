#pragma once
#include <ui/Widget.h>
#include <utils/logger.h>

namespace spd::ui {
    class Label : public Widget {
    public:
		ADD_CLASS_TAG;
        static constexpr const char* DEFAULT_TAG = "unnamed label";

        Label(const std::string& text);
        Label(const std::string& text, const char* tag);

        // getters and setters
        std::string GetText() const;
        void SetText(const std::string& text);

	protected:
        void OnRender() override;
        ImVec2 OnCalcSize() override;

    protected:
        std::string m_text;
    };

    template <typename... Args>
    std::unique_ptr<Label> MakeLabel(Args&&... args) {
        return std::make_unique<Label>(std::move(args)...);
    }
}
