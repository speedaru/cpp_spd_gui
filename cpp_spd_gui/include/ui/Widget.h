#pragma once
#include <memory>
#include <string>
#include <ui/Style.h>
#include <ui/BoxModel.h>
#include <utils/logger.h>

namespace spd::ui {
	class Widget {
	public:
		ADD_CLASS_TAG;

		Widget() { GenerateID(); }
		virtual ~Widget() = default;

		// recalculate sizes
		virtual void Update();

		// arrange position
		virtual void Arrange(ImVec2 finalPosition);

		void Render();

		template <typename T>
		T ResolveStyle(std::optional<T> Style::* property, T defaultValue) const;

		template <typename T>
		std::optional<T> ResolveStyleOpt(std::optional<T> Style::* property, std::optional<T> defaultValue) const;

		// set base size of widget
		inline Widget* SetBaseSize(ImVec2 newSize) { m_baseSize = newSize; return this; }
		inline Widget* SetParent(Widget* parent) { m_parent = parent; return this; }
		inline Widget* SetPosition(const ImVec2 newPos) { m_position = newPos; return this; }

		inline ImVec2 GetBaseSize() const { return m_baseSize; }
		inline ImVec2 GetContentSize() const { return m_boxModel.contentSize; }
		inline ImVec2 GetBoxSize() const { return m_boxModel.boxSize; }
		inline ImVec2 GetTotalSize() const { return m_boxModel.totalSize; }
		inline const BoxModel& GetBox() const { return m_boxModel; }

		Style m_style;

	protected:
		// calc content size
        virtual ImVec2 OnCalcSize() = 0;
		// render content
        virtual void OnRender() = 0;

		virtual void RenderBackground();

        void RenderBorder();
        void GenerateID();

	protected:
		static constexpr const char ID_PREFIX[] = "##w_";
		static constexpr const size_t PTR_STR_LEN = sizeof(void*) * 2;
		static constexpr const size_t ID_BUFF_SIZE = sizeof(ID_PREFIX) - 1 + PTR_STR_LEN + 1; // null terminated

		char m_id[ID_BUFF_SIZE];
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

	template <typename T>
	inline std::optional<T> Widget::ResolveStyleOpt(std::optional<T> Style::* property, std::optional<T> defaultValue) const {
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
