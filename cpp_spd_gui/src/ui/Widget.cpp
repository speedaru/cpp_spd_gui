#include "pch.h"
#include <ui/Widget.h>

namespace spd::ui {
	void Widget::GenerateID() {
		m_id = "##Widget_" + std::to_string(reinterpret_cast<uintptr_t>(this));
	}
}
