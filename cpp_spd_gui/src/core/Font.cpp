#include "pch.h"
#include <core/Font.h>

namespace spd::core {
	ImFont* Font::Get(float size) const {
        // find requested size
        auto it = m_sizes.find(size);
        if (it != m_sizes.end()) {
            return it->second;
        }

        // fallback to default
        auto defaultIt = m_sizes.find(m_defaultSize);
        if (defaultIt != m_sizes.end()) {
            LOG_W("Font size %.1f not found, falling back to %.1f\n", size, m_defaultSize);
            return defaultIt->second;
        }

        // return nullptr or imgui default
        if (!m_sizes.empty()) {
            return m_sizes.begin()->second;
        }
        
        return nullptr;
    }

    void Font::SetDefaultSize(float size) {
        m_defaultSize = size;
    }

    Font Font::LoadFromMemory(const void* ttfData, size_t ttfSize, const std::vector<float>& sizes, const ImFontConfig* customCfg) {
        Font result;
        if (sizes.empty()) return result;

        ImGuiIO& io = ImGui::GetIO();

        ImFontConfig cfg;
        if (customCfg) {
            cfg = *customCfg;
        } else {
            cfg.FontDataOwnedByAtlas = false; // essential for static byte arrays!
            cfg.OversampleH = 2; // better text rendering
            cfg.OversampleV = 2;
        }

        // make font
        for (float s : sizes) {
            ImFont* imFont = io.Fonts->AddFontFromMemoryTTF((void*)ttfData, (int)ttfSize, s, &cfg);
            result.m_sizes[s] = imFont;
        }

        // make the first size the safe fallback default
        result.SetDefaultSize(sizes[0]);

        return result;
    }
}