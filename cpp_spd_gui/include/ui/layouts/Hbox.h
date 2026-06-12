#pragma once
#include <ui/layouts/FlexBox.h>

namespace spd::ui {
    class Hbox : public FlexBox {
    public:
        static constexpr const char* DEFAULT_TAG = "unnamed hbox";

        Hbox(const char* tag = DEFAULT_TAG) : FlexBox(Orientation::Horizontal, tag) {}
    };

    template <typename... Args>
    std::unique_ptr<Hbox> MakeHbox(Args&&... args) {
        auto box = std::make_unique<Hbox>();
        (box->Add(std::move(args)), ...);
        return box;
    }
}
