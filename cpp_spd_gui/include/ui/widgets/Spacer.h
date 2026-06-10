#pragma once
#include <memory>
#include <ui/Widget.h>

namespace spd::ui {
	class Spacer : public Widget {
	public:
		Spacer() = default;

	protected:
		void OnRender() override {}
		ImVec2 OnCalcSize() override { return { 0.f, 0.f }; }
	};

	template <typename... Args>
	std::unique_ptr<Spacer> MakeSpacer(Args&&... args) {
		return std::make_unique<Spacer>(std::forward<Args>(args)...);
	}
}
