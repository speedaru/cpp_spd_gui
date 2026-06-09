#pragma once
#include <ui/Widget.h>

namespace spd::ui {
    class Label : public Widget {
    public:
        Label(const std::string& text);

        void Update() override;
        void Render() override;

        ImVec2 CalcSize() override;

        // Setters/Getters
        void SetText(const std::string& text);
        std::string GetText() const;

    private:
        std::string m_text;
    };

    template <typename... Args>
    std::unique_ptr<Label> MakeLabel(Args&&... args) {
        return std::make_unique<Label>(std::move(args)...);
    }
}
