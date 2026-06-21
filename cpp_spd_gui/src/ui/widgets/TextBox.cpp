#include "pch.h"
#include <core/event_dispatcher.h>
#include <ui/widgets/TextBox.h>
#include <ui/fx/Animator.h>
#include <utils/imgui_utils.h>
#include <utils/widget_draw.h>

#include <imgui/imgui_internal.h>

namespace spd::ui {
    TextBox::TextBox(const std::string& placeholder, const char* tag)
        : m_placeholder(placeholder) {
        m_tag = tag;
    }

    TextBox* TextBox::OnChange(std::function<void(ImGuiInputTextCallbackData*)> callback) {
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
        bool isActive  = (ImGui::GetActiveID() == ImGui::GetID(m_id));

        // background
        utils::DrawAnimatedRect(
            this, m_id, borderPos, boxSize, isHovered, isActive,
            &Style::bgColor, &Style::hoverColor, &Style::activeColor,
            ImGuiCol_FrameBg, ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive
        );

        // text
        ImGui::SetCursorPos(borderPos);
        const Offsets& padding = m_style.padding.value_or(Offsets::ZERO);
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding.top, padding.left));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0); 
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, 0);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, 0);
        
        bool textPushed = utils::PushTextColor(this);
        ImGui::PushItemWidth(boxSize.x);

        ImGuiInputTextFlags flags = m_isPassword ? ImGuiInputTextFlags_Password : 0;
        flags |= ImGuiInputTextFlags_CallbackEdit;
        ImGui::InputTextWithHint(m_id, m_placeholder.c_str(), m_buffer, sizeof(m_buffer), flags, TextBoxCallback, this);

        ImGui::PopItemWidth();
        utils::PopTextColor(textPushed);
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();

        //// event handling
        //if (IsTextChanged() && m_onChangeCallback) {
        //    core::event_dispatcher::Defer(m_onChangeCallback, std::string(m_buffer));
        //}
    }

    int TextBox::TextBoxCallback(ImGuiInputTextCallbackData* data) {
        auto* textbox = static_cast<TextBox*>(data->UserData);

        if (textbox->m_onChangeCallback) {
            textbox->m_onChangeCallback(data);
        }

        return 0;
    }

    bool TextBox::IsTextChanged() {
        static char previousText[sizeof(m_buffer)]{0};
        bool res = false;

        size_t lenBuff = strlen(m_buffer);
        assert(lenBuff < sizeof(m_buffer));

        // different text len means edited
        if (strlen(previousText) != strlen(m_buffer)) {
            res = true;
            goto end;
        }

        // same strlen, compare buffs
		// only compare text
        res = !memcmp(previousText, m_buffer, lenBuff);

        // save previous text
        end:
        memcpy(previousText, m_buffer, lenBuff);
        previousText[lenBuff] = '\0'; // add null terminator
        return res;
    }

    std::string TextBox::GetText() const { return std::string(m_buffer); }

    void TextBox::SetText(const std::string& text) {
        strncpy_s(m_buffer, sizeof(m_buffer), text.c_str(), _TRUNCATE);
    }
}