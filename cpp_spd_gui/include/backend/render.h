#pragma once
#include "backend_defs.h"

// forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace spd::backend {
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void CreateWindowClass(WindowData& windowData, WNDPROC lpfnWndProc);
	void DestroyWindowClass(WindowData& windowData);

	void CreateD3DDevice(DxDevice& dxDevice);
	void CreateD3DData(DxData& dxData, DxDevice& dxDevice, WindowData& windowData);
	void CleanupD3DDevice(DxDevice& dxDevice);
	void CleanupD3DData(DxData& dxData);

	// callback is to init the specific imgui stuff customly like load fonts and themes
	void CreateImGui(DxData& dxData, WindowData& windowData, ImGuiData& imguiData);
	void DestroyImGui(ImGuiData& imguiData);
}
