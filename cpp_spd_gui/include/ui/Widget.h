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

		// set base size of widget
		inline void SetBaseSize(ImVec2 newSize) { m_baseSize = newSize; }

		// returns widget position
		inline void SetPosition(const ImVec2 newPos) { m_position = newPos; }

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
		ImVec2 m_baseSize{};
		ImVec2 m_position{};
		BoxModel m_boxModel{ &m_style.padding, &m_style.margin };
	};
}
