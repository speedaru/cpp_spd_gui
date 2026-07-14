#pragma once
#include <memory>
#include <ui/Widget.h>
#include <ui/layouts/FlexCommon.h>

namespace spd::ui {
	class Divider : public Widget {
	public:
		Divider(float size, Orientation orientation);

	protected:
		ImVec2 OnCalcSize() override { return { 0.f, 0.f }; }
		void OnRender() override {}
	};

	std::unique_ptr<Divider> MakeVDivider(float size);
	std::unique_ptr<Divider> MakeHDivider(float size);
}
