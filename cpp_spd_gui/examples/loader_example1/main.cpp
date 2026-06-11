#include <string>
#include <memory>
#include "views.h"
#include "QuicksandSemiBold.hpp"

static void SetupFonts(ImGuiIO& io) {
	assets::quicksand = spd::core::Font::LoadFromMemory(
		QuicksandSemiBold, sizeof(QuicksandSemiBold),
		{ 14.f, 16.f, 18.f, 24.f, 32.f }
	);
}

int main(int argc, char** argv) {
	logging::LoggerInit(nullptr, logging::LogLevel::Debug | logging::LogLevel::Warn | logging::LogLevel::Error);

	spd::core::AppConfig config{
		.borderless = true,
		.noImGuiIni = true,
		.fontCallback = SetupFonts
	};
	spd::core::Application app(L"my test app", 800, 600, nullptr, config);

	auto root = std::move(views::CreateMasterLayout(app));

	app.SetRoot(std::move(root));
	app.Run();

	logging::LoggerShutdown();
}