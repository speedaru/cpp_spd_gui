#pragma once
#include <memory>
#include <string>
#include <ui/Style.h>
#include <ui/BoxModel.h>

namespace spd::ui {
	class Widget {
	public:
		Widget() { GenerateID(); }
		virtual ~Widget() = default;

		virtual void Update();
		void Render();

		template <typename T>
		T ResolveStyle(std::optional<T> Style::* property, T defaultValue) const;

		// set base size of widget
		inline Widget* SetBaseSize(ImVec2 newSize) { m_baseSize = newSize; return this; }
		inline Widget* SetParent(Widget* parent) { m_parent = parent; return this; }
		inline Widget* SetPosition(const ImVec2 newPos) { m_position = newPos; return this; }

		inline ImVec2 GetContentSize() const { return m_boxModel.contentSize; }
		inline ImVec2 GetBoxSize() const { return m_boxModel.boxSize; }
		inline ImVec2 GetTotalSize() const { return m_boxModel.totalSize; }
		inline const BoxModel& GetBox() const { return m_boxModel; }

		Style m_style;

	protected:
		// virtual methods for derived classes
        virtual void OnRender() = 0;
		// calc content size
        virtual ImVec2 OnCalcSize() = 0;

        void RenderBorder();
        void GenerateID();

	protected:
		std::string m_id;
		Widget* m_parent{};
		ImVec2 m_baseSize{};
		ImVec2 m_position{};
		BoxModel m_boxModel{ &m_style.padding, &m_style.margin };
	};

	template <typename T>
	inline T Widget::ResolveStyle(std::optional<T> Style::* property, T defaultValue) const {
		const Widget* current = this;

		while (current != nullptr) {
			// check if the current widget set this style property
			if ((current->m_style.*property).has_value()) {
				return (current->m_style.*property).value();
			}
			// move up to the parent
			current = current->m_parent;
		}

		// if nobody in the tree set it use default value
		return defaultValue;
	}
}
