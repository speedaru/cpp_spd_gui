#pragma once
#include <ui/layouts/FlexBox.h>

namespace spd::ui {
    class Vbox : public FlexBox {
    public:
        static constexpr const char* DEFAULT_TAG = "unnamed vbox";

        Vbox(const char* tag = DEFAULT_TAG) : FlexBox(Orientation::Vertical, tag) {}
    };

    template <typename... Args>
    std::unique_ptr<Vbox> MakeVbox(Args&&... args) {
        auto box = std::make_unique<Vbox>();
        (box->Add(std::move(args)), ...);
        return box;
    }
}
