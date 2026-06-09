#include "pch.h"
#include <ui/Container.h>

namespace spd::ui {
	void Container::Update() {
		for (const auto& child : m_children) {
			child->Update();
		}
	}

	void Container::Render() {
		for (const auto& child : m_children) {
			child->Render();
		}
	}

	void Container::AddChild(Child&& child) {
		if (child) {
			m_children.push_back(std::move(child));
		}
	}

	void Container::Clear() {
		m_children.clear();
	}
}
