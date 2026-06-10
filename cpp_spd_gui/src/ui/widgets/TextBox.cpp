#include "pch.h"
#include <ui/widgets/TextBox.h>
#include <imgui/imgui_internal.h>

namespace spd::ui {
    TextBox::TextBox(const std::string& placeholder)
        : m_placeholder(placeholder) {
        LOG_D("id for textbox: %s\n", m_id.c_str());
    }

    TextBox* TextBox::OnChange(std::function<void(const std::string&)> callback) {
        m_onChangeCallback = callback;
        return this;
    }

    ImVec2 TextBox::OnCalcSize() {
        // ImGui::CalcTextSize works perfectly, but an empty input box needs a minimum width
        ImVec2 calcSize = ImGui::CalcTextSize(m_buffer);

        // If the box is empty or text is short, force a minimum footprint
        if (calcSize.x < 120.0f) calcSize.x = 120.0f;

        // ImGui fonts inherently need a bit of vertical height 
        calcSize.y = ImGui::GetFontSize();

        return calcSize;
    }

    void TextBox::OnRender() {
        ImVec2 borderPos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;

        bool isHovered = ImGui::IsMouseHoveringRect(borderPos, { borderPos.x + boxSize.x, borderPos.y + boxSize.y });

        // check if this widget is focused
        bool isActive = (ImGui::GetActiveID() == ImGui::GetID(m_id.c_str()));

        // resolve colors
        ImVec4 imguiFrameColor = GetDefaultImGuiColor(ImGuiCol_FrameBg);
        Color frameColor = ResolveStyle(&Style::bgColor, IMVEC4_TO_COLOR(imguiFrameColor)); // default imgui textbox bg color

        if (isActive) {
            const ImVec4& imguiFrameActive = GetDefaultImGuiColor(ImGuiCol_FrameBgActive);
            frameColor = ResolveStyle(&Style::activeColor, IMVEC4_TO_COLOR(imguiFrameActive));
        }
        else if (isHovered) {
            const ImVec4& imguiFrameHovered = GetDefaultImGuiColor(ImGuiCol_FrameBgHovered);
            frameColor = ResolveStyle(&Style::hoverColor, IMVEC4_TO_COLOR(imguiFrameHovered));
        }

        // inherit text color from parent
        std::optional<Color> textColor = std::nullopt;
        if (ResolveStyle(&Style::textColor, {}).imu32 != Color().imu32) { // no default
            textColor = ResolveStyle(&Style::textColor, {});
        }

        // draw custom background exactly to box model dimensions
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float rounding = m_style.rounding.value_or(0.0f);
        drawList->AddRectFilled(borderPos, { borderPos.x + boxSize.x, borderPos.y + boxSize.y }, frameColor.imu32, rounding);

        // position the internal text slightly offset by box model padding
        ImGui::SetCursorPos(borderPos);

        // strip imgui of its styling so it acts only as a text renderer
        const Offsets& padding = m_style.padding.value_or(Offsets::ZERO);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding.top, padding.left)); // kill imgui double padding
        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0); // transparent background
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, 0);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, 0);
        if (textColor.has_value()) ImGui::PushStyleColor(ImGuiCol_Text, textColor.value().imu32);

        // force the invisible imgui input frame to match our exact content width
        ImGui::PushItemWidth(boxSize.x);

        ImGuiInputTextFlags flags = 0;
        if (m_isPassword) flags |= ImGuiInputTextFlags_Password;

        // render the raw imgui text input
        bool changed = ImGui::InputTextWithHint(m_id.c_str(), m_placeholder.c_str(), m_buffer, sizeof(m_buffer), flags);

        ImGui::PopItemWidth();
        if (textColor.has_value()) ImGui::PopStyleColor(); // pop text color if set
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();

        // text changed callback
        if (changed && m_onChangeCallback) {
            m_onChangeCallback(std::string(m_buffer));
        }
    }

    std::string TextBox::GetText() const { return std::string(m_buffer); }

    void TextBox::SetText(const std::string& text) {
        strncpy_s(m_buffer, sizeof(m_buffer), text.c_str(), _TRUNCATE);
    }
}