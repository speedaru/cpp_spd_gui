#include "pch.h"
#include <ui/layouts/ViewSwitcher.h>

namespace spd::ui {
    void ViewSwitcher::Update() {
        if (!m_activeView) return;

        LOG_D("[MEASURE] ---> Entering ViewSwitcher (%s) Update. Active: %s\n",
            m_tag, m_activeView.name.c_str());

        // update only active child
        m_activeView->Update();
        Widget::Update();

        // if hgrow or vgrow then grow child size
        CalculateFlex();
    }

    void ViewSwitcher::Arrange(ImVec2 finalPosition) {
		// Save the switcher's own verified screen boundaries
        Widget::Arrange(finalPosition);
        
        if (!m_activeView) return;

        // Resolve structural layout parameters for our sub-view panel
        Alignment childAlign = m_activeView->m_style.alignment.value_or(Alignment::Default);
        ImVec2 contentStart = m_boxModel.CalcAlignedContentStart(m_position, childAlign);

        // Forward the arrangement flow directly down to the active view node
        m_activeView->Arrange(contentStart);
    }

    void ViewSwitcher::SwitchTo(const std::string& name) {
        auto it = m_views.find(name);
        if (it == m_views.end()) {
            LOG_W("failed to switch to view %.*s bcs it doesnt exist\n", name.length(), name.data());
            return;
        }

        m_activeView = { .name = name, .view = it->second };
    }

    ImVec2 ViewSwitcher::OnCalcSize() {
        // size of container is the size of the active child
        if (m_activeView) {
            return m_activeView->GetTotalSize();
        }
        return { 0.f, 0.f };
    }

    void ViewSwitcher::OnRender() {
        if (!m_activeView) return;

        m_activeView->Render();
    }

    void ViewSwitcher::CalculateFlex() {
        ImVec2 containerSpace = m_boxModel.GetContentAreaSize();
        
        bool requiresUpdate = false;
        ImVec2 childTargetSize = m_activeView->GetBoxSize();

        // check child grow properties
        if (m_activeView->m_style.hgrow.value_or(false) && containerSpace.x > childTargetSize.x) {
            childTargetSize.x = containerSpace.x;
            requiresUpdate = true;
        }
        if (m_activeView->m_style.vgrow.value_or(false) && containerSpace.y > childTargetSize.y) {
            childTargetSize.y = containerSpace.y;
            requiresUpdate = true;
        }

        if (requiresUpdate) {
            m_activeView->SetBaseSize(childTargetSize);
            m_activeView->Update(); // sync child box model
            Widget::Update();
        }
    }
}