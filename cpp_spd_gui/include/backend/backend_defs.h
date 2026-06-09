#pragma once
#include <imgui/imgui.h>
#include <d3d11.h>
#define _AMD_
#define NOMINMAX
#include <minwindef.h>
#include <backend/types.h>

namespace spd::backend {
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
		char menuName[64]{ 0 };
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
	using CreateWindowCallback = void(*)(WindowData&);

	struct ImGuiData {
		struct ImGuiContext* context{};
	};
	using CreateImGuiCallback = void(*)(ImGuiData&);

	struct BackendData {
		DxData dxData{};
		WindowData windowData{};
		ImGuiData imGuiData{};
		POINTS dragStartPoint{};
	};
}
