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

	auto root = spd::ui::MakeVBox(
		spd::ui::MakeLabel("nigga"),
		spd::ui::MakeLabel("nigga2")
	);
	root->SetSpacing(8.f)->SetPadding({ 4.f, 24.f });

	app.SetRoot(std::move(root));
	app.Run();

	logging::LoggerShutdown();
}