#include "pch.h"
#include <utils/screen.h>
#include <Windows.h>

namespace spd::utils {
	vec2 GetScreenSize() {
		return {
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN)
        };
	}
}