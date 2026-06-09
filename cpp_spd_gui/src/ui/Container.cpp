#include "pch.h"
#include <ui/Container.h>

namespace spd::ui {
	void Container::Update() {
		// update children before calculating own size
		for (const auto& child : m_children) {
			child->Update();
		}

		// calculate own size
		Widget::Update();
	}

	void Container::Clear() {
		m_children.clear();
	}

	const size_t Container::GetChildCount() const {
		return m_children.size();
	}
}
