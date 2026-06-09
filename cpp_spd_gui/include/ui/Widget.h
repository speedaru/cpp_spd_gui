#pragma once
#include <memory>
#include <string>
#include <ui/Style.h>

namespace spd::ui {
	class Widget {
	public:
		Widget() { GenerateID(); }
		virtual ~Widget() = default;

		virtual void Update();
		void Render();

		// getters and setters

		// set base size of widget
		inline void SetSize(ImVec2 newSize) { m_size = newSize; }

		// returns total widget size with margin + padding
		inline ImVec2 GetTotalSize() const { return m_totalSize; }

		// returns widget position
		inline void SetPosition(const ImVec2 newPos) { m_position = newPos; }

		Style m_style;

	protected:
		// virtual methods for derived classes
        virtual void OnRender() = 0;
		// calc content size
        virtual ImVec2 OnCalcSize() = 0;

        // helpers for box model

		// get position of content (position + margin + padding)
        ImVec2 GetContentPosition() const;
		// get position of border (position + margin)
        ImVec2 GetBorderPosition() const;
		// get size of stuff inside border (content + padding)
        ImVec2 GetInnerBorderSize() const;
        
        void RenderBorder();
        void GenerateID();

	protected:
		std::string m_id;
		ImVec2 m_size{};
		ImVec2 m_totalSize{};
		ImVec2 m_contentSize{};
		ImVec2 m_position{};
	};
}
