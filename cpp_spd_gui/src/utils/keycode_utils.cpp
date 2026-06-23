#include "pch.h"
#include <utils/keycode_utils.h>
#include <unordered_map>

namespace spd::utils {
	static const std::unordered_map<UINT, std::string> KEYCODE_NAMES = {
		// Special keys
		{ VK_ESCAPE, "Esc" },
		{ VK_TAB, "Tab" },
		{ VK_RETURN, "Enter" },
		{ VK_SHIFT, "Shift" },
		{ VK_CONTROL, "Ctrl" },
		{ VK_MENU, "Alt" },
		{ VK_SPACE, "Space" },
		{ VK_BACK, "Backspace" },
		{ VK_DELETE, "Delete" },
		{ VK_INSERT, "Insert" },
		{ VK_HOME, "Home" },
		{ VK_END, "End" },
		{ VK_PRIOR, "Page Up" },
		{ VK_NEXT, "Page Down" },
		{ VK_UP, "Up" },
		{ VK_DOWN, "Down" },
		{ VK_LEFT, "Left" },
		{ VK_RIGHT, "Right" },

		// Function keys
		{ VK_F1, "F1" }, { VK_F2, "F2" }, { VK_F3, "F3" }, { VK_F4, "F4" },
		{ VK_F5, "F5" }, { VK_F6, "F6" }, { VK_F7, "F7" }, { VK_F8, "F8" },
		{ VK_F9, "F9" }, { VK_F10, "F10" }, { VK_F11, "F11" }, { VK_F12, "F12" },

		// Number pad
		{ VK_NUMPAD0, "Numpad 0" }, { VK_NUMPAD1, "Numpad 1" },
		{ VK_NUMPAD2, "Numpad 2" }, { VK_NUMPAD3, "Numpad 3" },
		{ VK_NUMPAD4, "Numpad 4" }, { VK_NUMPAD5, "Numpad 5" },
		{ VK_NUMPAD6, "Numpad 6" }, { VK_NUMPAD7, "Numpad 7" },
		{ VK_NUMPAD8, "Numpad 8" }, { VK_NUMPAD9, "Numpad 9" },
		{ VK_MULTIPLY, "Numpad *" },
		{ VK_ADD, "Numpad +" },
		{ VK_SUBTRACT, "Numpad -" },
		{ VK_DECIMAL, "Numpad ." },
		{ VK_DIVIDE, "Numpad /" },

		// Mouse buttons
		{ VK_LBUTTON, "Mouse Left" },
		{ VK_RBUTTON, "Mouse Right" },
		{ VK_MBUTTON, "Mouse Middle" },
		{ VK_XBUTTON1, "Mouse X1" },
		{ VK_XBUTTON2, "Mouse X2" },

		// Letter keys (A-Z)
		{ 'A', "A" }, { 'B', "B" }, { 'C', "C" }, { 'D', "D" }, { 'E', "E" },
		{ 'F', "F" }, { 'G', "G" }, { 'H', "H" }, { 'I', "I" }, { 'J', "J" },
		{ 'K', "K" }, { 'L', "L" }, { 'M', "M" }, { 'N', "N" }, { 'O', "O" },
		{ 'P', "P" }, { 'Q', "Q" }, { 'R', "R" }, { 'S', "S" }, { 'T', "T" },
		{ 'U', "U" }, { 'V', "V" }, { 'W', "W" }, { 'X', "X" }, { 'Y', "Y" },
		{ 'Z', "Z" },

		// Number keys (0-9)
		{ '0', "0" }, { '1', "1" }, { '2', "2" }, { '3', "3" }, { '4', "4" },
		{ '5', "5" }, { '6', "6" }, { '7', "7" }, { '8', "8" }, { '9', "9" },

		// Symbol keys
		{ VK_OEM_1, ";" },
		{ VK_OEM_PLUS, "=" },
		{ VK_OEM_COMMA, "," },
		{ VK_OEM_MINUS, "-" },
		{ VK_OEM_PERIOD, "." },
		{ VK_OEM_2, "/" },
		{ VK_OEM_3, "`" },
		{ VK_OEM_4, "[" },
		{ VK_OEM_5, "\\" },
		{ VK_OEM_6, "]" },
		{ VK_OEM_7, "'" },
	};

	std::string GetKeyName(UINT keyCode) {
		if (keyCode == 0) {
			return "None";
		}

		auto it = KEYCODE_NAMES.find(keyCode);
		if (it != KEYCODE_NAMES.end()) {
			return it->second;
		}

		// Fallback: format as hex
		return "0x" + std::to_string(keyCode);
	}
}
