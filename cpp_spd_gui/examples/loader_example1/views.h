#pragma once
#include <core/Application.h>
#include <core/Font.h>
#include <ui/Container.h>
#include <memory>
using namespace spd;

namespace assets {
	inline core::Font quicksand;
	inline core::Font firaCode;
}

namespace views {
    // Composes the overarching UI shell (Top bar + Master ViewSwitcher)
    std::unique_ptr<ui::Container> CreateMasterLayout(core::Application& app);

}
