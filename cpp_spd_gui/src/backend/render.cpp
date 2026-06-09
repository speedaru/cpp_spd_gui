#include "pch.h"
#include <backend/render.h>

// private function declarations
void CreateRenderTarget(DxData& dxData);
void CleanupRenderTarget(DxData& dxData);


// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI render::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    //case WM_SIZE:
    //    if (wParam == SIZE_MINIMIZED)
    //        return 0;
    //    g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
    //    g_ResizeHeight = (UINT)HIWORD(lParam);
    //    return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void render::CreateWindowClass(WindowData& windowData, WNDPROC lpfnWndProc) {
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW& wc = windowData.windowClass;
    wc.cbSize = sizeof(wc);
    wc.style = 0;
    wc.lpfnWndProc = lpfnWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = windowData.menuName;
    wc.lpszClassName = windowData.className;
    wc.hIcon = nullptr;

    ::RegisterClassExW(&wc);

    windowData.hwnd = ::CreateWindowExW(
        windowData.extendedStyles,
        wc.lpszClassName,
        wc.lpszMenuName,
        windowData.styles,
        windowData.startPos.x,
        windowData.startPos.y,
        static_cast<int>(windowData.size.x * main_scale),
        static_cast<int>(windowData.size.y * main_scale),
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    //// Show the window
    //::ShowWindow(windowData.hwnd, SW_SHOWDEFAULT);
    //::UpdateWindow(windowData.hwnd);
}

void render::DestroyWindowClass(WindowData& windowData) {
    ::DestroyWindow(windowData.hwnd);
    ::UnregisterClassW(windowData.className, windowData.windowClass.hInstance);
}

void render::CreateD3DDevice(DxDevice& dxDevice) {
    const D3D_FEATURE_LEVEL deviceFeatures[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    UINT features = sizeof(deviceFeatures) / sizeof(D3D_FEATURE_LEVEL);
    UINT deviceFlags = 0;

    HRESULT res = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, deviceFeatures, features, D3D11_SDK_VERSION, &dxDevice.pd3dDevice, nullptr, &dxDevice.pd3dDeviceContext);
    assert(res == S_OK);
}

void render::CreateD3DData(DxData& dxData, DxDevice& dxDevice, WindowData& windowData) {
    dxData.device = &dxDevice;
    ID3D11Device* pd3dDevice = dxDevice.pd3dDevice;

    // define swapchain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = windowData.hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // get dxgi device
	IDXGIDevice* dxgiDevice = nullptr;
	HRESULT hr = pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    assert(hr == S_OK);

    // get dxgi adapter
	IDXGIAdapter* dxgiAdapter = nullptr;
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);
    assert(hr == S_OK);

	dxgiDevice->Release();

	IDXGIFactory* dxgiFactory = nullptr;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
    assert(hr == S_OK);

	dxgiAdapter->Release();

    // create swapchain using dxgi factory
	hr = dxgiFactory->CreateSwapChain(pd3dDevice, &sd, &dxData.pSwapChain);
    assert(hr == S_OK);

	dxgiFactory->Release();

	CreateRenderTarget(dxData);
}

void render::CleanupD3DDevice(DxDevice& dxDevice) {
    ID3D11Device*& device = dxDevice.pd3dDevice;
    ID3D11DeviceContext*& context = dxDevice.pd3dDeviceContext;

    if (context) { context->ClearState(); context->Flush(); context->Release(); context = nullptr; }
    if (device) { device->Release(); device = nullptr; }
}

void render::CleanupD3DData(DxData& dxData) {
    ID3D11DeviceContext*& context = dxData.device->pd3dDeviceContext;

    CleanupRenderTarget(dxData);
    if (dxData.pSwapChain) { dxData.pSwapChain->Release(); dxData.pSwapChain = nullptr; }
}

void render::CreateImGui(DxData& dxData, WindowData& windowData, ImGuiData& imguiData) {
    // init win32 and dx11 implementations
	ImGui_ImplWin32_Init(windowData.hwnd);
	ImGui_ImplDX11_Init(dxData.device->pd3dDevice, dxData.device->pd3dDeviceContext);
}

void render::DestroyImGui(ImGuiData& imguiData) {
    ImGui::SetCurrentContext(imguiData.context);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}


// -------------------- private stuff

void CreateRenderTarget(DxData& dxData) {
    ID3D11Texture2D* pBackBuffer;
    dxData.pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (!pBackBuffer) {
        fprintf(stderr, "error at " __FILE__ ":%d" "\n", __LINE__);
        return;
    }

    dxData.device->pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &dxData.pRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget(DxData& renderData) {
    if (renderData.pRenderTargetView) {
        renderData.pRenderTargetView->Release();
        renderData.pRenderTargetView = nullptr;
    }
}


