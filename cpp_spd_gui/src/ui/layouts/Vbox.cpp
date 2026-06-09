#include "pch.h"
#include <ui/layouts/Vbox.h>

namespace spd::ui {
    ImVec2 Vbox::CalcSize() {
        ImVec2 calculatedSize = { 0.0f, 0.0f };

        // 1. Sum up the heights and find the widest child
        for (const auto& child : m_children) {
            ImVec2 childSize = child->CalcSize();

            calculatedSize.x = max(calculatedSize.x, childSize.x);
            calculatedSize.y += childSize.y;
        }

        // 2. Add the spacing between elements
        if (!m_children.empty()) {
            calculatedSize.y += static_cast<float>(m_children.size() - 1) * m_spacing;
        }

        // 3. Add the internal padding
        calculatedSize.x += m_padding.x * 2.0f;
        calculatedSize.y += m_padding.y * 2.0f;

        // Cache and return
        this->m_size = calculatedSize;
        return this->m_size;
    }

    void Vbox::Render() {
        // If we haven't calculated our size yet, do it now
        if (m_size.x == 0 && m_size.y == 0) {
            CalcSize();
        }

        // Start drawing from our top-left corner, plus the padding
        ImVec2 currentCursorPos = {
            m_position.x + m_padding.x,
            m_position.y + m_padding.y
        };

        for (auto& child : m_children) {
            // Tell the child exactly where it needs to draw itself
            child->SetPosition(currentCursorPos);

            // Draw it
            child->Render();

            // Push the cursor down by the child's height + the VBox spacing
            currentCursorPos.y += child->GetSize().y + m_spacing;
        }
    }

}