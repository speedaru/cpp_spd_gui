#pragma once
#include <vector>
#include <ui/Widget.h>
#include <utils/logger.h>

namespace spd::ui {
	class Container : public Widget {
	public:
		using Child = std::unique_ptr<Widget>;

		void Update() override;

		void Clear();

		template <typename T>
		inline T* Add(std::unique_ptr<T> child) {
			T* ptr = child.get();
			ptr->SetParent(this);
			m_children.push_back(std::move(child));
			return ptr;
		}

		const size_t GetChildCount() const;
		
	protected:
        virtual void OnRender() = 0;
        virtual ImVec2 OnCalcSize() = 0;

	protected:
		std::vector<Child> m_children;
	};
}
