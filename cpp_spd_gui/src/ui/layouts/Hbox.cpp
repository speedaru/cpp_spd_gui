#include "pch.h"
#include <ui/layouts/Hbox.h>

namespace spd::ui {
    void Hbox::OnRender() {
        // styles
        float spacing = m_style.spacing.value_or(0.f);
        Offsets padding = m_style.padding.value_or(Offsets::ZERO);
        Alignment align = m_style.alignment.value_or(Alignment::Center);

        // vbox layout
        ImVec2 contentStart = m_boxModel.CalcContentPosition(m_position);
        contentStart += CalcAlignmentPos(m_boxModel.GetContentAreaSize(), m_boxModel.contentSize, align);
        float currentX = contentStart.x;

        for (const auto& child : m_children) {
            Alignment childAlign = child->m_style.alignment.value_or(align);

            float currentY = contentStart.y + m_boxModel.CalcPositionY(child->GetBox(), childAlign);
            child->SetPosition({ currentX, currentY });
            child->Render();

            currentX += child->GetTotalSize().x + spacing;
        }
    }

    ImVec2 Hbox::OnCalcSize() {
        ImVec2 calculatedSize = { 0.0f, 0.0f };

        // sum up the heights and find the widest child
        for (const auto& child : m_children) {
            ImVec2 childSize = child->GetTotalSize();
            calculatedSize.x += childSize.x;
            calculatedSize.y = std::max(calculatedSize.y, childSize.y);
        }

		// add spacing between elements
		if (m_style.spacing.has_value() && !m_children.empty()) {
			calculatedSize.x += m_style.spacing.value() * (m_children.size() - 1); // only space items vertically
		}

        return calculatedSize;
    }
}
