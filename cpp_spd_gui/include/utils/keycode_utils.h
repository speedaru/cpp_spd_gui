#pragma once
#include <string>
#include <Windows.h>

namespace spd::utils {
	// Convert Windows virtual key code to human-readable key name
	std::string GetKeyName(UINT keyCode);

	// Get the virtual key code for common keys (for default values)
	namespace Keys {
		constexpr UINT None = 0;
		constexpr UINT Ctrl = VK_CONTROL;
		constexpr UINT Shift = VK_SHIFT;
		constexpr UINT Alt = VK_MENU;
		constexpr UINT Space = VK_SPACE;
		constexpr UINT Enter = VK_RETURN;
	}
}
