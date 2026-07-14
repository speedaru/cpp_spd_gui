#include "pch.h"
#include <ui/widgets/Divider.h>
#include <utils/imgui_utils.h>

namespace spd::ui {
	spd::ui::Divider::Divider(float size, Orientation orientation) {
		ImVec4 borderColor = spd::utils::GetDefaultImGuiColor(ImGuiCol_Border);
		borderColor = {
			borderColor.x * 0.7f,
			borderColor.y * 0.7f,
			borderColor.z * 0.7f,
			borderColor.w * 0.7f,
		};
		
		this->m_style
			.SetRounding(0.f)
			.SetFrameBgColor(IMVEC4_TO_COLOR(borderColor));

		// orientation specific styling
		switch (orientation) {
		case Orientation::Horizontal:
			SetBaseSize({ 0.f, size });
			this->m_style.SetHgrow(true);
			break;
		case Orientation::Vertical:
			SetBaseSize({ size, 0.f });
			this->m_style.SetVgrow(true);
			break;
		}
	}

	std::unique_ptr<Divider> MakeVDivider(float size) {
		return std::make_unique<Divider>(size, Orientation::Vertical);
	}

	std::unique_ptr<Divider> MakeHDivider(float size) {
		return std::make_unique<Divider>(size, Orientation::Horizontal);
	}
}
