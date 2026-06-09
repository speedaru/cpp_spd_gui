#include "pch.h"
#include <ui/Widget.h>

namespace spd::ui {
	void Widget::Update() {
		// get derived class size
		m_contentSize = OnCalcSize();

		// get inner size (content + padding) and add margin to get total size
		Offsets margin = m_style.margin.value_or(Offsets::ZERO);
		ImVec2 innerSize = GetInnerBorderSize();
		m_totalSize = { innerSize.x += margin.Width(), innerSize.y += margin.Height() };
	}

	void Widget::Render() {
		// draw border for all widgets
		RenderBorder();

		// draw derived widget content
		OnRender();
	}

	ImVec2 Widget::GetContentPosition() const {
		Offsets margin = m_style.margin.value_or(Offsets::ZERO);
        Offsets padding = m_style.padding.value_or(Offsets::ZERO);

		return {
			m_position.x + margin.left + padding.left,
			m_position.y + margin.top + padding.top,
		};
	}

	ImVec2 Widget::GetBorderPosition() const {
		Offsets margin = m_style.margin.value_or(Offsets::ZERO);
		return { m_position.x + margin.left, m_position.y + margin.top };
	}

	ImVec2 Widget::GetInnerBorderSize() const {
		Offsets padding = m_style.padding.value_or(Offsets::ZERO);

		ImVec2 calcSize = m_contentSize;
		calcSize.x += padding.Width();
		calcSize.y += padding.Height();

		// try to use predefined size if set and bigger
		ImVec2 innerSize;
		innerSize.x = std::max(m_size.x, calcSize.x);
		innerSize.y = std::max(m_size.y, calcSize.y);

		return innerSize;
	}

	void Widget::RenderBorder() {
		if (!m_style.borderColor.has_value() || !m_style.borderSize.has_value()) return;

        ImDrawList* draw = ImGui::GetForegroundDrawList();
        Color borderColor = m_style.borderColor.value();
        float borderSize = m_style.borderSize.value();
        float rounding = m_style.rounding.value_or(0.f);

        ImVec2 pos = GetBorderPosition();
        ImVec2 size = GetInnerBorderSize();

        draw->AddRect(pos, pos + size, borderColor.imu32, rounding, 0, borderSize);
	}

	void Widget::GenerateID() {
		m_id = "##Widget_" + std::to_string(reinterpret_cast<uintptr_t>(this));
	}
}
