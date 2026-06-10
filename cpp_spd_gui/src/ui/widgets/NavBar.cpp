#include "pch.h"
#include <ui/widgets/NavBar.h>
#include <ui/widgets/Label.h>
#include <ui/widgets/Button.h>

namespace spd::ui {
    NavBar::NavBar() : Box(Orientation::Horizontal) {
        SetTag("navigation_bar");
        
        // navigation bars should instinctively fill the cross-axis width of the application frame
        m_style.SetHgrow(true);
        
        // apply comfortable layout baseline behaviors
        m_style.SetPadding({ 8.f, 16.f, 8.f, 16.f });
        m_style.SetSpacing(12.f);
        m_style.SetAlignment(Alignment::Left);
    }

    NavBar* NavBar::SetNavButtonSize(ImVec2 size) {
        m_navButtonSize = size;
        return this;
    }

    NavBar* NavBar::AddTitle(const std::string& title) {
        if (m_titleLabel != nullptr) {
            return this;
        }

        auto titleLabel = MakeLabel(title);
        titleLabel->m_style.SetHgrow(true).SetAlignment(Alignment::Left);

        m_titleLabel = titleLabel.get();
        Add(std::move(titleLabel));
        return this;
    }

    NavBar* NavBar::SetTitle(const std::string& title) {
        m_titleLabel->SetText(title);
        return this;
    }

    NavBar* NavBar::AddMinButton(std::function<void()> minCallback) {
        if (m_minButton != nullptr) {
            return this;
        }

        auto minButton = MakeButton("-", "min");
        minButton->OnClick(minCallback)->SetBaseSize(m_navButtonSize);

        m_minButton = minButton.get();
        Add(std::move(minButton));
        return this;
    }

    NavBar* NavBar::AddCloseButton(std::function<void()> closeCallback) {
        if (m_closeButton != nullptr) {
            return this;
        }
        
        auto closeButton = MakeButton("X", "close");
        closeButton->OnClick(closeCallback)->SetBaseSize(m_navButtonSize);

        m_closeButton = closeButton.get();
        Add(std::move(closeButton));
        return this;
    }

    void NavBar::RenderBackground() {
        Color resolvedColor;

        if (m_style.frameBgColor.has_value()) {
            // use explicit background color
            resolvedColor = m_style.frameBgColor.value();
        }
        else {
            // default imgui 
            ImVec4 imguiDefault = GetDefaultImGuiColor(ImGuiCol_TitleBgActive);
            resolvedColor = IMVEC4_TO_COLOR(imguiDefault);
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;
        float rounding = m_style.frameRounding.value_or(0.f);

        // draw background
        draw->AddRectFilled(pos, pos + boxSize, resolvedColor.imu32, rounding);
    }

}