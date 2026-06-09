#pragma once
#include <memory>
#include <string>
#include <imgui/imgui.h>

namespace spd::ui {
	class Widget {
	public:
		Widget() {}
		virtual ~Widget() = default;

		virtual void Update() = 0;
		virtual void Render() = 0;

		virtual ImVec2 CalSize() = 0;

		// getters and setters
		inline ImVec2 GetPosition() const { return m_position; }
		inline void SetPosition(const ImVec2 newPos) { m_position = newPos; }

		inline ImVec2 GetSize() const { return m_size; }
		inline void SetSize(const ImVec2 newSize) { m_size = newSize; }

	protected:
		// generate unique imgui id (based on mem address)
		void GenerateID();

	protected:
		ImVec2 m_size{};
		ImVec2 m_position{};
		std::string m_id;
	};
}
