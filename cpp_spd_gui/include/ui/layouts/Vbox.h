#pragma once
#include <ui/Container.h>

namespace spd::ui {
	class Vbox : public Container {
    public:
		Vbox() = default;
        ~Vbox() = default;

        // Core overrides
        void Update() override {}
        void Render() override;
        ImVec2 CalcSize() override;

        // Builder-pattern styling methods
        Vbox* SetSpacing(float spacing) { m_spacing = spacing; return this; }
        Vbox* SetPadding(const ImVec2& padding) { m_padding = padding; return this; }

    private:
        float m_spacing = 8.0f; // Default gap between elements
        ImVec2 m_padding = { 0.0f, 0.0f }; // Internal padding inside the box
	};

    template <typename... Widgets>
    std::unique_ptr<Vbox> MakeVBox(Widgets... widgets) {
        std::unique_ptr<Vbox> box = std::make_unique<Vbox>();
        (box->AddChild(std::move(widgets)), ...);
        return box;
    }
}
