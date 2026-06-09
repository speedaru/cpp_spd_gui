#pragma once
#include "pch.h"

struct vec2 {
	int x{};
	int y{};
};

struct DxDevice {
	ID3D11Device* pd3dDevice{};
	ID3D11DeviceContext* pd3dDeviceContext{};
};

struct DxData {
	DxDevice* device;
	IDXGISwapChain* pSwapChain{};
	bool SwapChainOccluded{};
	ID3D11RenderTargetView* pRenderTargetView{};
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
};

struct WindowData {
	// stuff to specify
	LPCWSTR menuName;
	LPCWSTR className;
	DWORD styles{};
	DWORD extendedStyles{};
	vec2 startPos{};
	POINTS winLoc{};
	vec2 size{};

	// gets filled
	WNDCLASSEXW windowClass{};
	HWND hwnd{};
};
using CreateWindowCallback = void(*)(WindowData& windowData);

struct ImGuiData {
	struct ImGuiContext* context{};
};
using CreateImGuiCallback = void(*)(ImGuiData& imguiData);


