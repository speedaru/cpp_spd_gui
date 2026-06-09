#pragma once
#include <ui/Container.h>

namespace spd::backend {
	struct DxDevice;
	struct BackendData;
}

namespace spd::core {
	struct AppConfig {
		bool borderless = false;
	};

	class Application {
	public:
		Application(const std::wstring& title, int width, int height,
			spd::backend::DxDevice* sharedDevice = nullptr, AppConfig config = {});
		~Application();

		void SetRoot(std::unique_ptr<ui::Container> rootWidget);
		void Run();
		void Close();

		// retrieve device to make a second window
		spd::backend::DxDevice* GetDevice() const;

		void* GetBackend() const { return m_backend.get(); }

	private:
		void SetupSharedDevice(spd::backend::DxDevice* sharedDevice);

		void SetupWindow(AppConfig config);
		void InitImGui();
		void ShowWindow();

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
