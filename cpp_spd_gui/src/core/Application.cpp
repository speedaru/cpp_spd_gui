#include "pch.h"
#include <backend/render.h>
#include <core/Application.h>
#include <core/event_dispatcher.h>
#include <utils/screen.h>
using namespace spd::backend;

// forward declare imgui wndproc handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace spd::core {
	static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return true;

        Application* app = reinterpret_cast<Application*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        if (!app || !app->IsRunning())
            return ::DefWindowProcW(hwnd, msg, wParam, lParam);

        float titleBarHeight = app->GetTitleBarHeight();
        auto* pBackend = reinterpret_cast<BackendData*>(app->GetBackend());

        switch (msg) {
            case WM_SYSCOMMAND:
                if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
                break;
            case WM_DESTROY:
                ::PostQuitMessage(0);
                return 0;
            case WM_LBUTTONDOWN:
                pBackend->dragStartPoint = MAKEPOINTS(lParam);
                return 0;
            case WM_MOUSEMOVE:
                if (wParam == MK_LBUTTON) {
                    const auto points = MAKEPOINTS(lParam);
                    const auto& windowSize = pBackend->windowData.size;
                    const auto& dragStart = pBackend->dragStartPoint;

                    RECT rect;
                    GetWindowRect(hwnd, &rect);
                    
					rect.left += points.x - pBackend->dragStartPoint.x;
					rect.top += points.y - pBackend->dragStartPoint.y;

                    // ensure draging title bar and not main window
                    if (dragStart.x >= 0 && dragStart.x <= windowSize.x && dragStart.y >= 0 && dragStart.y <= titleBarHeight) {
						SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
                    }
                }
                return 0;
        }
        return ::DefWindowProcW(hwnd, msg, wParam, lParam);
    }

	Application::Application(const std::wstring& title, int width, int height, DxDevice* sharedDevice, const AppConfig& config)
        : m_isRunning(true), m_width(width), m_height(height), m_ownsDevice(false) {
        
        m_config = config;
        m_backend = std::make_unique<BackendData>();
        SetupSharedDevice(sharedDevice);

        // setup window data
        m_backend->windowData.className = title.c_str();
        // manually copy wchar title into menu name array
        for (size_t i{}; i < title.length() && i < sizeof(WindowData::menuName); i++) {
            m_backend->windowData.menuName[i] = (char)title[i];
        }
        m_backend->windowData.size = { width, height };
        vec2 screenSize = utils::GetScreenSize();
        m_backend->windowData.startPos = { screenSize.x / 2 - width / 2, screenSize.y / 2 - height / 2 };

        // initialize everything
        SetupWindow(config);
        spd::backend::CreateWindowClass(m_backend->windowData, WndProc);
        ShowWindow();

        spd::backend::CreateD3DData(m_backend->dxData, *m_backend->dxData.device, m_backend->windowData);

        InitImGui();
        spd::backend::CreateImGui(m_backend->dxData, m_backend->windowData, m_backend->imGuiData);
        SetupImGui(config);

        // set wnd proc param
		SetWindowLongPtrW(m_backend->windowData.hwnd, GWLP_USERDATA, (LONG_PTR)this);
    }

    Application::~Application() {
		spd::backend::DestroyImGui(m_backend->imGuiData);
		spd::backend::CleanupD3DData(m_backend->dxData);
		spd::backend::DestroyWindowClass(m_backend->windowData);

        if (m_ownsDevice) {
            spd::backend::CleanupD3DDevice(*m_backend->dxData.device);
            delete m_backend->dxData.device;
        }
    }

    void Application::SetRoot(std::unique_ptr<ui::Container> rootWidget) {
        m_root = std::move(rootWidget);
    }

    void Application::Run(std::chrono::milliseconds frameDelay) {
        while (m_isRunning) {
            StartRender();
            if (!m_isRunning) break; // pressed quit
            Render();
            EndRender();

            // add delay if not zero
            static constexpr const auto noDelay = std::chrono::milliseconds(0);
            if (frameDelay > noDelay) {
				std::this_thread::sleep_for(frameDelay);
            }
        }
    }

    void Application::Minimize() {
        ::ShowWindow(m_backend->windowData.hwnd, SW_MINIMIZE);
    }

    void Application::Close() {
        m_isRunning = false;
    }

    float Application::GetTitleBarHeight() {
        if (m_config.useCustomTitleBar) {
            return m_config.customTitleBarHeight;
        }
		return ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
    }

    DxDevice* Application::GetDevice() const {
        return m_backend->dxData.device;
    }

    // ------------------------- private helpers

    void Application::SetupWindow(const AppConfig& config) {
        WindowData& windowData = m_backend->windowData;
        if (config.borderless) {
            windowData.styles |= WS_POPUP;
        }
    }

    void Application::SetupSharedDevice(DxDevice* sharedDevice) {
        if (sharedDevice) {
            m_backend->dxData.device = sharedDevice;
        }
        else {
            // create new dx device
            m_backend->dxData.device = new DxDevice();
            spd::backend::CreateD3DDevice(*m_backend->dxData.device);
            m_ownsDevice = true; // this app is reposible for freeing device
        }
    }

    void Application::ShowWindow() {
        WindowData& windowData = m_backend->windowData;
        ::ShowWindow(windowData.hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(windowData.hwnd);
    }

    void Application::InitImGui() {
        ImGuiData& imGuiData = m_backend->imGuiData;

        IMGUI_CHECKVERSION();
        ImGuiContext* context = imGuiData.context = ImGui::CreateContext();
        ImGui::SetCurrentContext(context);
    }

    void Application::SetupImGui(const AppConfig& config) {
        ImGuiIO& io = ImGui::GetIO();

        if (config.noImGuiIni) {
            io.IniFilename = nullptr;
        }
        if (config.imguiCallback) {
            config.imguiCallback(io);
        }
    }

    void Application::StartRender() {
		MSG msg;
		while (::PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
            if (msg.message == WM_QUIT) {
                m_isRunning = false;
                return;
            }
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
    }

    void Application::Render() {
        WindowData& windowData = m_backend->windowData;
        ImGui::SetNextWindowPos({ 0.f, 0.f });
        ImGui::SetNextWindowSize({ (float)windowData.size.x, (float)windowData.size.y });

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove;

        if (m_config.useCustomTitleBar) {
            flags |= ImGuiWindowFlags_NoTitleBar;
        }

        bool* pOpen = m_config.useCustomTitleBar ? nullptr : &m_isRunning;
        ImGui::Begin(windowData.menuName, pOpen, flags);

        static int frameCounter = 1;
		if (m_root) {
            float titlebarHeight = Application::GetTitleBarHeight();
            
            // if we use a custom title bar, the root takes up the full window
            // the custom title bar will just be the first hbox inside the root
            if (m_config.useCustomTitleBar) {
                m_root->SetBaseSize(ImVec2(static_cast<float>(m_width), static_cast<float>(m_height)));
                m_root->SetPosition(ImVec2(0, 0));
            }
            else {
                m_root->SetBaseSize(ImVec2(static_cast<float>(m_width), static_cast<float>(m_height - titlebarHeight)));
                m_root->SetPosition(ImVec2(0, titlebarHeight));
            }

			m_root->Update();
            m_root->Arrange({ 0.f, 0.f });
			m_root->Render();
            spd::core::event_dispatcher::Flush();

            //LOG_D("memory usage: %llu bytes, total allocated (%llu)\n", GetBytesAllocated(), g_totalAllocated);
            //LOG_T("frame %d: --------------------------------------------------\n", frameCounter++);
		}

        ImGui::End();
        ImGui::PopStyleVar(2);
        
		ImGui::Render();
    }

    void Application::EndRender() {
		const float clear_color_with_alpha[4] = { 0.1f, 0.1f, 0.1f, 1.f };

		auto ctx = m_backend->dxData.device->pd3dDeviceContext;
		ctx->OMSetRenderTargets(1, &m_backend->dxData.pRenderTargetView, nullptr);
		ctx->ClearRenderTargetView(m_backend->dxData.pRenderTargetView, clear_color_with_alpha);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		m_backend->dxData.pSwapChain->Present(1, 0);
    }
}