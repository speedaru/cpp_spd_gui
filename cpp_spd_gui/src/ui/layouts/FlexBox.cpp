#include "pch.h"
#include <ui/layouts/FlexBox.h>

namespace spd::ui {
    void FlexBox::Update() {
        Container::Update(); // calc children and self content size
        CalculateFlex(); // grow + recalc size
    }

    void FlexBox::Arrange(ImVec2 finalPosition) {
        // save container absolute position
        Widget::Arrange(finalPosition);

        float spacing = m_style.spacing.value_or(0.f);
        Alignment align = m_style.alignment.value_or(Alignment::Default);
        AxisConfig axis{ m_orientation };

        ImVec2 contentStart = m_boxModel.CalcAlignedContentStart(m_position, align);
        float currentMain = axis.GetMain(contentStart);

        // set children position top down chain
        for (const auto& child : m_children) {
            Alignment childAlign = child->m_style.alignment.value_or(align);

            float offCross = axis.CalcCrossOffset(
                axis.GetCross(m_boxModel.contentSize), 
                axis.GetCross(child->GetTotalSize()), 
                childAlign
            );

            ImVec2 childPos;
            axis.SetMain(childPos, currentMain);
            axis.SetCross(childPos, axis.GetCross(contentStart) + offCross);

            // arrange child internal layout
            child->Arrange(childPos);

            currentMain += axis.GetMain(child->GetTotalSize()) + spacing;
        }
    }

	ImVec2 FlexBox::OnCalcSize() {
        ImVec2 calculatedSize = { 0.0f, 0.0f };
        AxisConfig axis{ m_orientation };

        float mainSum = 0.0f;
        float crossMax = 0.0f;

        for (const auto& child : m_children) {
            ImVec2 childSize = child->GetTotalSize();
            mainSum += axis.GetMain(childSize);
            crossMax = std::max(crossMax, axis.GetCross(childSize));
        }

        if (m_style.spacing.has_value() && !m_children.empty()) {
            mainSum += m_style.spacing.value() * (m_children.size() - 1);
        }

        axis.SetMain(calculatedSize, mainSum);
        axis.SetCross(calculatedSize, crossMax);

        return calculatedSize;
    }

    void FlexBox::OnRender() {
        for (const auto& child : m_children) {
            child->Render();
        }
    }

    void FlexBox::CalculateFlex() {
        bool flexed = false;
        AxisConfig axis{ m_orientation };

        float availableMain = axis.GetMain(m_boxModel.GetContentAreaSize());
        float availableCross = axis.GetCross(m_boxModel.GetContentAreaSize());

        flexed |= CalcCrossAxisGrow(axis, availableCross);
        flexed |= CalcMainAxisGrow(axis, availableMain);

        if (flexed) Widget::Update();
    }

    bool FlexBox::CalcCrossAxisGrow(const AxisConfig& axis, float availableCross) {
        bool flexed = false;
        
        for (const auto& child : m_children) {
            // grow not set to true
            if (!axis.GetCrossGrow(child->m_style)) {
                continue;
            }

			Offsets childMargin = child->m_style.margin.value_or(Offsets::ZERO);
			float targetCross = availableCross - axis.GetCrossTotal(childMargin);

			if (targetCross > axis.GetCross(child->GetBoxSize())) {
				ImVec2 newBaseSize = child->GetBoxSize();
				axis.SetCross(newBaseSize, targetCross);
				child->SetBaseSize(newBaseSize);
				child->Update();
				flexed = true;
			}
        }

        return flexed;
    }

    bool FlexBox::CalcMainAxisGrow(const AxisConfig& axis, float availableMain) {
        bool flexed = false;

        if (availableMain > 0.f) {
            float consumedMain = 0.f;
            int growCount = 0;

            for (const auto& child : m_children) {
                if (axis.GetMainGrow(child->m_style)) growCount++;
                else consumedMain += axis.GetMain(child->GetTotalSize());
            }

            if (m_children.size() > 1) {
                consumedMain += m_style.spacing.value_or(0.f) * (m_children.size() - 1);
            }

            if (growCount > 0 && availableMain > consumedMain) {
                float extraPerChild = (availableMain - consumedMain) / growCount;

                for (const auto& child : m_children) {
                    if (axis.GetMainGrow(child->m_style)) {
                        Offsets childMargin = child->m_style.margin.value_or(Offsets::ZERO);
                        float targetMain = extraPerChild - axis.GetMainTotal(childMargin);

                        ImVec2 newBaseSize = child->GetBoxSize();
                        axis.SetMain(newBaseSize, targetMain);
                        child->SetBaseSize(newBaseSize);
                        child->Update();
                        flexed = true;
                    }
                }
            }
        }

        return flexed;
    }
}