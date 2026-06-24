#pragma once
#include <vector>
#include <string>
#include <ui/Widget.h>

namespace spd::ui {
	class Dropdown : public Widget {
	public:
		static constexpr const char* DEFAULT_TAG = "unnamed dropdown";

		Dropdown(const std::string_view& preview, const char* tag);
		Dropdown(const std::string_view& preview, std::initializer_list<std::string_view> options);

		void Add(const std::string_view& option);
		void SetPreview(const std::string& preview);

		Dropdown* OnSelect(std::function<void(const std::string&, size_t)> callback);

		std::string GetSelectedText() const;
		int GetSelectedIndex() const { return m_selectedIndex; }

	protected:
		ImVec2 OnCalcSize() override;
		void OnRender() override;

	private:
		void RenderAnchorField(ImVec2 pos, ImVec2 size, bool isHovered, bool isActive);
		void RenderChevronIcon(ImVec2 pos, ImVec2 size);
		void RenderFloatingMenu(ImVec2 anchorPos, ImVec2 anchorSize);

	private:
		std::string m_preview;
		std::vector<std::string> m_options;
		int m_selectedIndex = -1;
		bool m_isOpen = false;

		std::function<void(const std::string&, size_t)> m_onSelectCallback = nullptr;
	};

    template <typename... Args>
    std::unique_ptr<Dropdown> MakeDropdown(Args&&... args) {
        return std::make_unique<Dropdown>(std::forward<Args>(args)...);
    }
}
