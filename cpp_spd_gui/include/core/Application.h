#pragma once
#include <ui/Container.h>

namespace spd::backend {
	struct DxDevice;
	struct BackendData;
}

namespace spd::core {
	struct AppConfig {
		bool borderless = false;
		bool noImGuiIni = false;
	};

	class Application {
	public:
		Application(const std::wstring& title, int width, int height,
			spd::backend::DxDevice* sharedDevice = nullptr, const AppConfig& config = {});
		~Application();

		void SetRoot(std::unique_ptr<ui::Container> rootWidget);
		void Run();
		void Close();

		static float GetTitleBarHeight();

		// retrieve device to make a second window
		spd::backend::DxDevice* GetDevice() const;

		inline bool IsRunning() const { return m_isRunning; }
		inline void* GetBackend() const { return m_backend.get(); }

	private:
		void SetupSharedDevice(spd::backend::DxDevice* sharedDevice);

		void SetupWindow(const AppConfig& config);
		void ShowWindow();

		void InitImGui();
		void SetupImGui(const AppConfig& config);

		void StartRender();
		void Render();
		void EndRender();

	protected:
		std::unique_ptr<spd::backend::BackendData> m_backend;

	private:
		std::unique_ptr<ui::Container> m_root;

		bool m_isRunning;
		int m_width;
		int m_height;
		bool m_ownsDevice; // Tracks memory ownership for cleanup
	};
}
