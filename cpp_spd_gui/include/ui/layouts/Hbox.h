#pragma once
#include <ui/Container.h>
#include <utils/logger.h>

namespace spd::ui {
	class Hbox : public Container {
    public:
        ADD_CLASS_TAG;
        static constexpr const char* DEFAULT_TAG = "unnamed vbox";

        Hbox(const char* tag = DEFAULT_TAG) : m_tag(tag) {}

	protected:
        void OnRender() override;
        ImVec2 OnCalcSize() override;
    };

    template <typename... T>
    std::unique_ptr<Hbox> MakeHbox(std::unique_ptr<T>&&... widgets) {
        std::unique_ptr<Hbox> box = std::make_unique<Hbox>();
        (box->Add(std::move(widgets)), ...);
        return box;
    }
}
