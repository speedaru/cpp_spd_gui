#pragma once
#include <utils/layout_math.h>

namespace spd::ui {
    enum class Orientation {
        Horizontal,
        Vertical
    };

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
            return orientation == Orientation::Horizontal
                ? utils::CalcAlignmentY(layoutSize, contentSize, align)
                : utils::CalcAlignmentX(layoutSize, contentSize, align);
        }
    };
}
