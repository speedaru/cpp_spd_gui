#pragma once
#include <ui/Container.h>
#include <ui/layouts/FlexCommon.h>

namespace spd::ui {
    class FlexBox : public Container {
    public:
        static constexpr const char* DEFAULT_TAG = "unnamed flex box";

        FlexBox(Orientation orientation, const char* tag = DEFAULT_TAG) : m_orientation(orientation) { m_tag = tag; }

        // calculate children sizes + self size, calculate children flex
        void Update() override;

        // 
        void Arrange(ImVec2 finalPosition) override;

    protected:
        ImVec2 OnCalcSize() override;
        void OnRender() override;

    private:
		void CalculateFlex();
        bool CalcCrossAxisGrow(const AxisConfig& axis, float availableCross);
        bool CalcMainAxisGrow(const AxisConfig& axis, float availableMain);

    private:
        Orientation m_orientation;
    };
}
