#include <string>
#include <memory>
#include <core/Application.h>
#include <ui/widgets/Label.h>
#include <ui/layouts/Vbox.h>
#include <utils/logger.h>

int main(int argc, char** argv) {
	logging::LoggerInit(nullptr, logging::LogLevel::Debug | logging::LogLevel::Error);

	spd::core::AppConfig config{
		.borderless = true,
		.noImGuiIni = true,
	};
	spd::core::Application app(L"my test app", 800, 600, nullptr, config);

	auto label1 = std::make_unique<spd::ui::Label>("nigga");
	app.SetRoot(spd::ui::MakeVBox(std::move(label1)));

	app.Run();

	logging::LoggerShutdown();
}