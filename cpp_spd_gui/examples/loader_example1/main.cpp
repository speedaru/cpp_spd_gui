#include <core/Application.h>

int main(int argc, char** argv) {
	spd::core::AppConfig config{
		.borderless = true,
	};
	spd::core::Application app(L"my test app", 800, 600, nullptr, config);
	app.Run();
}