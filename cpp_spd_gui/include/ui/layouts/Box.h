#pragma once
#include <ui/Container.h>

namespace spd::ui {
    enum class Orientation {
        Horizontal,
        Vertical
    };

    struct AxisConfig;

    // flex container
    class Box : public Container {
    public:
        ADD_CLASS_TAG;
        static constexpr const char* DEFAULT_TAG = "unnamed box";

        Box(Orientation orientation, const char* tag = DEFAULT_TAG) : m_orientation(orientation), m_tag(tag) {}

        void Update() override;

    protected:
        void OnRender() override;
        ImVec2 OnCalcSize() override;

    private:
        void CalculateFlex();
        bool CalcCrossAxisGrow(const AxisConfig& axis, float availableCross);
        bool CalcMainAxisGrow(const AxisConfig& axis, float availableMain);

    private:
        Orientation m_orientation;
    };

    template <typename... Args>
    std::unique_ptr<Box> MakeHbox(Args&&... args) {
        auto box = std::make_unique<Box>(Orientation::Horizontal);
        (box->Add(std::move(args)), ...);
        return box;
    }

    template <typename... Args>
    std::unique_ptr<Box> MakeVbox(Args&&... args) {
        auto box = std::make_unique<Box>(Orientation::Vertical);
        (box->Add(std::move(args)), ...);
        return box;
    }
}
