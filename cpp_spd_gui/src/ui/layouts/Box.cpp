#include "pch.h"
#include <ui/layouts/Box.h>

namespace spd::ui {
    struct AxisConfig {
        Orientation orientation;

        inline float GetMain(ImVec2 v) const { return orientation == Orientation::Horizontal ? v.x : v.y; }
        inline float GetCross(ImVec2 v) const { return orientation == Orientation::Horizontal ? v.y : v.x; }

        inline void SetMain(ImVec2& v, float val) const { if (orientation == Orientation::Horizontal) v.x = val; else v.y = val; }
        inline void SetCross(ImVec2& v, float val) const { if (orientation == Orientation::Horizontal) v.y = val; else v.x = val; }

        inline float GetMainTotal(Offsets o) const { return orientation == Orientation::Horizontal ? o.Width() : o.Height(); }
        inline float GetCrossTotal(Offsets o) const { return orientation == Orientation::Horizontal ? o.Height() : o.Width(); }

        inline bool GetMainGrow(const Style& s) const { return orientation == Orientation::Horizontal ? s.hgrow.value_or(false) : s.vgrow.value_or(false); }
        inline bool GetCrossGrow(const Style& s) const { return orientation == Orientation::Horizontal ? s.vgrow.value_or(false) : s.hgrow.value_or(false); }

        inline float CalcCrossOffset(float layoutSize, float contentSize, Alignment align) const {
            return orientation == Orientation::Horizontal ? CalcAlignmentY(layoutSize, contentSize, align) : CalcAlignmentX(layoutSize, contentSize, align);
        }
    };

    void Box::Update() {
        Container::Update();
        CalculateFlex();
    }

    void Box::OnRender() {
        float spacing = m_style.spacing.value_or(0.f);
        Alignment align = m_style.alignment.value_or(Alignment::Default);

        AxisConfig axis{ m_orientation };

        ImVec2 contentStart = m_boxModel.CalcContentPosition(m_position);
        contentStart += CalcAlignmentPos(m_boxModel.GetContentAreaSize(), m_boxModel.contentSize, align);

        float currentMain = axis.GetMain(contentStart);

        // render children
        for (const auto& child : m_children) {
            Alignment childAlign = child->m_style.alignment.value_or(align);

            // calculate item offset on cross axis
            float offCross = axis.CalcCrossOffset(axis.GetCross(m_boxModel.contentSize), axis.GetCross(child->GetTotalSize()), childAlign);

            ImVec2 childPos;
            axis.SetMain(childPos, currentMain);
            axis.SetCross(childPos, axis.GetCross(contentStart) + offCross);

            child->SetPosition(childPos);
            child->Render();

            currentMain += axis.GetMain(child->GetTotalSize()) + spacing;
        }
    }

	ImVec2 Box::OnCalcSize() {
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

    void Box::CalculateFlex() {
        bool flexed = false;
        AxisConfig axis{ m_orientation };

        float availableMain = axis.GetMain(m_boxModel.GetContentAreaSize());
        float availableCross = axis.GetCross(m_boxModel.GetContentAreaSize());

        flexed |= CalcCrossAxisGrow(axis, availableCross);
        flexed |= CalcMainAxisGrow(axis, availableMain);

        if (flexed) Widget::Update();
    }

    bool Box::CalcCrossAxisGrow(const AxisConfig& axis, float availableCross) {
        bool flexed = true;
        
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

    bool Box::CalcMainAxisGrow(const AxisConfig& axis, float availableMain) {
        bool flexed = true;

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