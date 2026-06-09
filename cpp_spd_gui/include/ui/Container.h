#pragma once
#include <vector>
#include "Widget.h"

namespace spd::ui {
	class Container : public Widget {
	public:
		using Child = std::unique_ptr<Widget>;

		void Update() override;
		void Render() override;

		void AddChild(Child&& child);

		void Clear();

		inline const size_t GetChildCount() const { return m_children.size(); }

	protected:
		std::vector<Child> m_children;
	};
}
