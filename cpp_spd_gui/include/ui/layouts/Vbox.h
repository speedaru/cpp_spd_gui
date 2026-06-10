#pragma once
#include <ui/Container.h>
#include <utils/logger.h>

namespace spd::ui {
	class Vbox : public Container {
    public:
        ADD_CLASS_TAG;
        static constexpr const char* DEFAULT_TAG = "unnamed vbox";

        Vbox(const char* tag = DEFAULT_TAG) : m_tag(tag) {}

        void Update() override;

    private:
        void CalculateFlex();

	protected:
        void OnRender() override;
        ImVec2 OnCalcSize() override;
    };

    template <typename... T>
    std::unique_ptr<Vbox> MakeVbox(std::unique_ptr<T>&&... widgets) {
        std::unique_ptr<Vbox> box = std::make_unique<Vbox>();
        (box->Add(std::move(widgets)), ...);
        return box;
    }
}
