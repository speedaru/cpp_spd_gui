#include <string>
#include <memory>
#include <core/Application.h>
#include <ui/widgets/Label.h>
#include <ui/widgets/Button.h>
#include <ui/layouts/Vbox.h>
#include <utils/logger.h>

int main(int argc, char** argv) {
	logging::LoggerInit(nullptr, logging::LogLevel::Debug | logging::LogLevel::Error);

	spd::core::AppConfig config{
		.borderless = true,
		.noImGuiIni = true,
	};
	spd::core::Application app(L"my test app", 800, 600, nullptr, config);

	auto root = spd::ui::MakeVbox();
	root->SetTag("root");
	root->m_style
		.SetSpacing(12.f)
		.SetPadding({ 8.f })
		.SetBorderColor({ 0, 255, 0, 155 })
		.SetBorderSize(1.f);

	// normal label in root
	root->Add(spd::ui::MakeLabel("apple", "label1"))
		->m_style
			.SetAlignment(spd::ui::Alignment::Center)
			.SetPadding(24.f)
			.SetMargin(0.f)
			.SetBorderColor({ 0, 255, 255, 155 })
			.SetBorderSize(1.f);

	// sub box
	auto vbox1 = root->Add(spd::ui::MakeVbox());
	//auto vbox1 = spd::ui::MakeVbox();
	vbox1->SetTag("sub vbox");
	vbox1->SetBaseSize({ 200.f, 150.f });
	vbox1->m_style
		.SetAlignment(spd::ui::Alignment::BottomRight)
		.SetSpacing(8.f)
		.SetPadding({ 8.f })
		.SetMargin({ 0.f, 0.f, 0.f, 32.f })
		.SetBorderColor({ 255, 0, 0, 155 })
		.SetBorderSize(1.f);

	// labels in sub box
	vbox1->Add(spd::ui::MakeLabel("apple1", "label2"))
		->m_style
			.SetPadding({ 6.f })
			.SetBorderColor({ 0, 255, 255, 155 })
			.SetBorderSize(1.f);

	vbox1->Add(spd::ui::MakeLabel("apple1.75", "label3"))
		->m_style
			.SetBorderColor({ 0, 255, 255, 155 })
			.SetBorderSize(1.f);

	vbox1->Add(spd::ui::MakeButton("button1", "button1"))
		->OnClick([] { LOG_D("clicked button\n"); })
		->m_style
			.SetPadding(8.f)
			.SetBgColor({ 30, 30, 30, 255 })
			.SetHoverColor({ 50, 50, 50, 255 })
			.SetActiveColor({ 100, 100, 100, 255 })
			.SetRounding(4.f)
			.SetBorderColor({ 255, 255, 0, 155 })
			.SetBorderSize(1.f);

	vbox1->Add(spd::ui::MakeLabel("apple2", "label4"))
		->m_style
			.SetBorderColor({ 0, 255, 255, 155 })
			.SetBorderSize(1.f);

	app.SetRoot(std::move(root));
	app.Run();

	logging::LoggerShutdown();
}