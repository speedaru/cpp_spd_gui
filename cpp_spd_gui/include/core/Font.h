#pragma once
#include <imgui/imgui.h>
#include <unordered_map>
#include <vector>

namespace spd::core {
    enum GlyphRanges {
        GlyphRanges_BasicLatin          = 1 << 0, // 0x0020 - 0x00FF
        GlyphRanges_Cyrillic            = 1 << 1, // 0x0400 - 0x052F
        GlyphRanges_CyrillicExtendedA   = 1 << 2, // 0x2DE0 - 0x2DFF
        GlyphRanges_CyrillicExtendedB   = 1 << 3, // 0xA640 - 0xA69F
        GlyphRanges_PrivateUserArea     = 1 << 4, // 0xE000 - 0xF8FF

        GlyphRanges_Basic = 0b0000'1111, // basic latin + all cyrillic
        GlyphRanges_Full = 0b0001'1111, // basic + private user area (PUA)
    };

    std::vector<ImWchar> GlyphRangesToImWchar(GlyphRanges glyphRanges);

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
        static Font LoadFromMemory(const void* ttfData, size_t ttfSize, const std::vector<float>& sizes, const ImFontConfig* customCfg = nullptr, GlyphRanges glyphRanges = GlyphRanges_Basic);
    };
}
