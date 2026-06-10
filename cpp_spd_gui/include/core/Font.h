#pragma once
#include <imgui/imgui.h>
#include <unordered_map>
#include <vector>

namespace spd::core {
    class Font {
    public:
        Font() = default;

        // retrieve the exact font size or safely fallback
        ImFont* Get(float size) const;

        // set the fallback size if a requested size doesn't exist
        void SetDefaultSize(float size);

    private:
        std::unordered_map<float, ImFont*> m_sizes;
        float m_defaultSize = 0.0f;

    public:
        // factory helper for loading
        static Font LoadFromMemory(const void* ttfData, size_t ttfSize, const std::vector<float>& sizes, const ImFontConfig* customCfg = nullptr);
    };
}
