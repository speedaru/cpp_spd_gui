#include "pch.h"
#include <ui/layouts/Vbox.h>

namespace spd::ui {
    void Vbox::Update() {
        m_size = CalcSize();
    }

    void Vbox::Render() {
        // if we haven't calculated our size yet, do it now
        if (m_size.x == 0 && m_size.y == 0) {
            LOG_E("size not calcualted\n");
            return;
        }

        // Start drawing from our top-left corner, plus the padding
        ImVec2 currentCursorPos = {
            m_position.x + m_padding.x,
            m_position.y + m_padding.y
        };

        for (auto& child : m_children) {
            child->SetPosition(currentCursorPos);
            child->Render();

            // move cursor
            currentCursorPos.y += child->GetSize().y + m_spacing;
        }
    }

    ImVec2 Vbox::CalcSize() {
        ImVec2 calculatedSize = { 0.0f, 0.0f };

        // sum up the heights and find the widest child
        for (const auto& child : m_children) {
            ImVec2 childSize = child->CalcSize();

            calculatedSize.x = std::max(calculatedSize.x, childSize.x);
            calculatedSize.y += childSize.y;
        }

        // add the spacing between elements
        if (!m_children.empty()) {
            calculatedSize.y += static_cast<float>(m_children.size() - 1) * m_spacing;
        }

        // add the internal padding
        calculatedSize.x += m_padding.x * 2.0f;
        calculatedSize.y += m_padding.y * 2.0f;

        return this->m_size;
    }

}