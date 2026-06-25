#include "pch.h"
#include <core/event_dispatcher.h>
#include <ui/widgets/Dropdown.h>
#include <utils/widget_draw.h> 
#include <utils/imgui_utils.h> 

namespace spd::ui {
	Dropdown::Dropdown(const std::string_view& preview, const char* tag)
        : m_preview(preview) {
        m_tag = tag;
    }

    Dropdown::Dropdown(const std::string_view& preview, std::initializer_list<std::string_view> options)
        : m_preview(preview) {
        m_tag = DEFAULT_TAG;
        for (const auto& opt : options) {
            m_options.push_back(std::string(opt));
        }
    }

    void Dropdown::Add(const std::string_view& option) {
        m_options.push_back(std::string(option));
    }

    void Dropdown::SetPreview(const std::string& preview) {
        m_preview = preview;
    }

    Dropdown* Dropdown::OnSelect(std::function<void(const std::string&, size_t)> callback) {
        m_onSelectCallback = callback;
        return this;
    }

    std::string Dropdown::GetSelectedText() const {
        if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_options.size())) {
            return m_options[m_selectedIndex];
        }
        return m_preview;
    }

    void Dropdown::RenderBorder() {
        // Skip default border rendering - we'll handle it ourselves
        if (!m_style.borderThickness.value_or(0.f)) return;

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec4 defaultColor = utils::GetDefaultImGuiColor(ImGuiCol_Border);
        Color color = m_style.borderColor.value_or(IMVEC4_TO_COLOR(defaultColor));
        float thickness = m_style.borderThickness.value();
        float rounding = ResolveStyle(&Style::rounding, 0.f);

        ImVec2 pos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;

        if (m_isOpen && !m_options.empty()) {
            // Draw unified border around main widget + dropdown items
            float itemHeight = ImGui::GetFontSize() + 8.f;
            float totalHeight = boxSize.y + (itemHeight * m_options.size());

            draw->AddRect(pos, pos + ImVec2(boxSize.x, totalHeight), color.imu32, rounding, 0, thickness);
        } else {
            // Draw normal border just around main widget
            draw->AddRect(pos, pos + boxSize, color.imu32, rounding, 0, thickness);
        }
    }

    float Dropdown::CalculateContentWidth() {
        float maxTextWidth = ImGui::CalcTextSize(m_preview.c_str()).x;
        for (const auto& opt : m_options) {
            maxTextWidth = std::max(maxTextWidth, ImGui::CalcTextSize(opt.c_str()).x);
        }
        return maxTextWidth;
    }

    ImVec2 Dropdown::OnCalcSize() {
        m_contentWidth = CalculateContentWidth();
        // Account for left padding, right chevron icon area, and horizontal padding
        return ImVec2(m_contentWidth + 32.0f, ImGui::GetFontSize());
    }

    // =================================================================
    // CONTROLLER / DISPATCHER
    // =================================================================
    void Dropdown::OnRender() {
        ImVec2 borderPos = m_boxModel.CalcBoxPosition(m_position);
        ImVec2 boxSize = m_boxModel.boxSize;

        // Position invisible interaction region to catch activation clicks
        ImGui::SetCursorPos(borderPos);
        std::string triggerId = std::string(m_id) + "_trigger";
        if (ImGui::InvisibleButton(triggerId.c_str(), boxSize)) {
            m_isOpen = !m_isOpen;
        }

        bool isHovered = ImGui::IsItemHovered();
        bool isActive  = ImGui::IsItemActive() || m_isOpen;

        // Dispatch clean sub-routine rendering layers sequential fields
        RenderAnchorField(borderPos, boxSize, isHovered, isActive);
        RenderChevronIcon(borderPos, boxSize);

        if (m_isOpen) {
            RenderFloatingMenu(borderPos, boxSize);
        }
    }

    // =================================================================
    // PARTITIONED GRAPHICS LAYER SUB-ROUTINES
    // =================================================================
    void Dropdown::RenderAnchorField(ImVec2 pos, ImVec2 size, bool isHovered, bool isActive) {
        utils::DrawAnimatedRect(
            this, m_id, pos, size, isHovered, isActive,
            &Style::bgColor, &Style::hoverColor, &Style::activeColor,
            ImGuiCol_FrameBg, ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive
        );

        // Display selection label or fallback placeholder preview string safely
        const Offsets& padding = m_style.padding.value_or(Offsets::ZERO);
        ImVec2 textPos = { pos.x + padding.left + 6.f, pos.y + (size.y - ImGui::GetFontSize()) * 0.5f };

        bool textPushed = utils::PushTextColor(this);
        ImGui::SetCursorPos(textPos);
        ImGui::TextUnformatted(GetSelectedText().c_str());
        utils::PopTextColor(textPushed);
    }

    void Dropdown::RenderChevronIcon(ImVec2 pos, ImVec2 size) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float iconSize = 5.0f;
        ImVec2 center = { pos.x + size.x - 16.0f, pos.y + size.y * 0.5f };

        Color iconColor = ResolveStyle(&Style::textColor, Color{ 255, 255, 255, 180 });

        if (m_isOpen) {
            drawList->AddTriangleFilled(
                { center.x - iconSize, center.y + iconSize * 0.5f },
                { center.x + iconSize, center.y + iconSize * 0.5f },
                { center.x, center.y - iconSize * 0.5f },
                iconColor.imu32
            );
        } else {
            drawList->AddTriangleFilled(
                { center.x - iconSize, center.y - iconSize * 0.5f },
                { center.x + iconSize, center.y - iconSize * 0.5f },
                { center.x, center.y + iconSize * 0.5f },
                iconColor.imu32
            );
        }
    }

    void Dropdown::RenderFloatingMenu(ImVec2 anchorPos, ImVec2 anchorSize) {
        // Place menu exactly below the anchor bar with no gap
        ImVec2 popupPos = { anchorPos.x, anchorPos.y + anchorSize.y };
        ImGui::SetNextWindowPos(popupPos);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;

        std::string popupWindowName = std::string("##Menu_") + m_id;

        // No padding and no border to align flush with the main widget and sit inside the unified border
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ResolveStyle(&Style::bgColor, Color{ 24, 24, 24, 255 }).imu32);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

        if (ImGui::Begin(popupWindowName.c_str(), nullptr, flags)) {
            for (size_t i = 0; i < m_options.size(); ++i) {
                std::string itemId = m_id + std::string("_opt_") + std::to_string(i);
                ImVec2 itemSize = ImVec2(anchorSize.x, ImGui::GetFontSize() + 8.f);

                ImGui::SetCursorPosX(0.f);
                bool isSelected = (static_cast<int>(i) == m_selectedIndex);

                ImVec2 rowPos = ImGui::GetCursorScreenPos();
                bool clicked = ImGui::Selectable(itemId.c_str(), isSelected, ImGuiSelectableFlags_None, itemSize);

                // Render animated backdrop layers underneath text inside our overlay window canvas context
                utils::DrawAnimatedRect(
                    this, itemId.c_str(), rowPos, itemSize, ImGui::IsItemHovered(), ImGui::IsItemActive() || isSelected,
                    &Style::bgColor, &Style::hoverColor, &Style::activeColor,
                    ImGuiCol_Header, ImGuiCol_HeaderHovered, ImGuiCol_HeaderActive
                );

                ImGui::SetCursorScreenPos({ rowPos.x + 8.f, rowPos.y + 4.f });
                ImGui::TextUnformatted(m_options[i].c_str());
                ImGui::SetCursorScreenPos({ rowPos.x, rowPos.y + itemSize.y }); // Step layout cursor context straight down

                if (clicked) {
                    m_selectedIndex = static_cast<int>(i);
                    LOG_D("new selected index %llu\n", i);
                    m_isOpen = false;

                    if (m_onSelectCallback) {
                        core::event_dispatcher::Defer([this, i]() {
                            m_onSelectCallback(m_options[i], i);
                        });
                    }
                }
            }

            // Close automatically if user clicks away completely outside popup bounds
            if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
                m_isOpen = false;
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(4);
    }
}
