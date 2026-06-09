#include "pch.h"
#include <ui/layouts/Vbox.h>

namespace spd::ui {
    void Vbox::OnRender() {
        ImVec2 cursor = GetContentPosition();
        float spacing = m_style.spacing.value_or(0.f);
        
        for (const auto& child : m_children) {
            child->SetPosition(cursor);
            child->Render();

            cursor.y += child->GetTotalSize().y + spacing;
        }
    }

    ImVec2 Vbox::OnCalcSize() {
        ImVec2 calculatedSize = { 0.0f, 0.0f };

        // sum up the heights and find the widest child
        for (const auto& child : m_children) {
            ImVec2 childSize = child->GetTotalSize();

            calculatedSize.x = std::max(calculatedSize.x, childSize.x);
            calculatedSize.y += childSize.y;
        }

		// add spacing between elements
		if (m_style.spacing.has_value() && !m_children.empty()) {
			calculatedSize.y += m_style.spacing.value() * (m_children.size() - 1); // only space items vertically
		}

        return calculatedSize;
    }
}