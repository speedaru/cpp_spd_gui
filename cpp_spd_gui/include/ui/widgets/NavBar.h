#pragma once
#include <string>
#include <functional>
#include <ui/layouts/Box.h>

namespace spd::ui {
    class Label;
    class Button;

    class NavBar : public Box {
    public:
        NavBar();

        NavBar* SetNavButtonSize(ImVec2 size);

        NavBar* AddTitle(const std::string& title);
        NavBar* SetTitle(const std::string& title);

        NavBar* AddMinButton(std::function<void()> minCallback);
        NavBar* AddCloseButton(std::function<void()> closeCallback);

    protected:
        // Override background sequence to capture theme defaults gracefully
        void RenderBackground() override; 

    private:
        ImVec2 m_navButtonSize{};
        Label* m_titleLabel{};
        Button* m_minButton{};
        Button* m_closeButton{};
    };

    // Inline composition factory wrapper
    template <typename... Args>
    std::unique_ptr<NavBar> MakeNavBar(Args&&... args) {
        auto nav = std::make_unique<NavBar>();
        (nav->Add(std::move(args)), ...);
        return nav;
    }
}
