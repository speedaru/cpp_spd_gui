#include "pch.h"
#include <ui/layouts/ViewSwitcher.h>

namespace spd::ui {
    void ViewSwitcher::Update() {
        if (!m_activeView) return;

        //ImVec2 originalSize = m_activeView->GetBoxSize();
        //if (m_style.hgrow.value_or(false)) originalSize.x = 0.f;
        //if (m_style.vgrow.value_or(false)) originalSize.y = 0.f;
        //m_activeView->SetBaseSize(originalSize);

        // update only active child
        m_activeView->Update();
        Widget::Update();

        // if hgrow or vgrow then grow child size
        CalculateFlex();
    }

    void ViewSwitcher::SwitchTo(const std::string& name) {
        auto it = m_views.find(name);
        if (it == m_views.end()) {
            LOG_W("failed to switch to view %.*s bcs it doesnt exist\n", name.length(), name.data());
            return;
        }

        m_activeView = { .name = name, .view = it->second };
    }

    void ViewSwitcher::OnRender() {
        if (!m_activeView) return;

        // draw active view
        ImVec2 contentStart = m_boxModel.CalcContentPosition(m_position);

        // apply alignment
        Alignment childAlign = m_activeView->m_style.alignment.value_or(Alignment::Default);
        contentStart += CalcAlignmentPos(m_boxModel.GetContentAreaSize(), m_boxModel.contentSize, childAlign);

        m_activeView->SetPosition(contentStart);
        m_activeView->Render();
    }

    ImVec2 ViewSwitcher::OnCalcSize() {
        // size of container is the size of the active child
        if (m_activeView) {
            return m_activeView->GetTotalSize();
        }
        return { 0.f, 0.f };
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