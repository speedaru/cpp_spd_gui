#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <thread>
#include <functional>
#include <algorithm>
#include <cassert>

// data structures
#include <vector>
#include <string>
#include <unordered_map>

// imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

// directx
#include <d3d11.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

// Windows API
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <utils/logger.h>
