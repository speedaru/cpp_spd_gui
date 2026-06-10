#include "pch.h"
#include <ui/layouts/Vbox.h>

namespace spd::ui {
    void Vbox::Update() {
        Container::Update();

        // hgrow
        CalculateFlex();
    }

    void Vbox::OnRender() {
        // styles
        float spacing = m_style.spacing.value_or(0.f);
        Offsets padding = m_style.padding.value_or(Offsets::ZERO);
        Alignment align = m_style.alignment.value_or(Alignment::Center);

        // vbox layout
        ImVec2 contentStart = m_boxModel.CalcContentPosition(m_position);
        contentStart += CalcAlignmentPos(m_boxModel.GetContentAreaSize(), m_boxModel.contentSize, align);
        float currentY = contentStart.y;

        for (const auto& child : m_children) {
            Alignment childAlign = child->m_style.alignment.value_or(align);

            float currentX = contentStart.x + m_boxModel.CalcPositionX(child->GetBox(), childAlign);
            child->SetPosition({ currentX, currentY });
            child->Render();

            currentY += child->GetTotalSize().y + spacing;
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

    void Vbox::CalculateFlex() {
        bool updated = false;

        // get available internal width from the box model
        float availableWidth = m_boxModel.GetContentAreaSize().x; 

        for (const auto& child : m_children) {
            if (!child->m_style.hgrow.value_or(false)) {
                continue;
            }

			Offsets childMargin = child->m_style.margin.value_or(Offsets::ZERO);
			
			// the child's target width is the available space minus its own margins
			float targetWidth = availableWidth - childMargin.Width();

			// only grow if the child width is smaller than target width
			if (targetWidth > child->GetBoxSize().x) {
				child->SetBaseSize({ targetWidth, child->GetBaseSize().y });
				child->Update(); // recalculate child's box model
                updated = true;
			}
        }

        if (updated) {
            Widget::Update();
        }
    }
}