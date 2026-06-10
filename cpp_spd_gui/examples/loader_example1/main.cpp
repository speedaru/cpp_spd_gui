#include <string>
#include <memory>
#include <core/Application.h>
#include <core/Font.h>
#include <utils/logger.h>
#include "QuicksandSemiBold.hpp"

#include <ui/layouts/Box.h>

#include <ui/widgets/Label.h>
#include <ui/widgets/Button.h>
#include <ui/widgets/TextBox.h>
#include <ui/widgets/Spacer.h>
#include <ui/widgets/NavBar.h>

namespace assets {
    spd::core::Font quicksand;
    spd::core::Font firaCode;
}

static void SetupFonts(ImGuiIO& io) {
	assets::quicksand = spd::core::Font::LoadFromMemory(
		QuicksandSemiBold, sizeof(QuicksandSemiBold),
		{ 14.f, 16.f, 18.f, 24.f, 32.f }
	);
}

std::unique_ptr<spd::ui::Container> Test(spd::core::Application& app) {
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
	vbox1->SetBaseSize({ 200.f, 200.f });
	vbox1->m_style
		.SetAlignment(spd::ui::Alignment::Top)
		.SetSpacing(8.f)
		.SetPadding({ 8.f })
		.SetTextColor({ 255, 255, 0, 255 })
		.SetBgColor({ 30, 30, 30, 255 })
		.SetHoverColor({ 50, 50, 50, 255 })
		.SetActiveColor({ 100, 100, 100, 255 })
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

	auto hbox1 = vbox1->Add(spd::ui::MakeHbox());
	hbox1->SetTag("hbox1");
	hbox1->SetBaseSize({ 0.f, 70.f });
	hbox1->m_style
		.SetPadding({ 0.f })
		.SetSpacing(8.f)
		.SetBorderColor({ 255, 0, 0, 205 })
		.SetBorderSize(1.f);

	hbox1->Add(spd::ui::MakeLabel("click me:", "click me"))
		->m_style
			.SetAlignment(spd::ui::Alignment::Top)
			.SetBorderColor({ 0, 255, 255, 205 })
			.SetBorderSize(1.f);

	hbox1->Add(spd::ui::MakeButton("button1", "button1"))
		->OnClick([] { LOG_D("clicked button\n"); })
		->m_style
			.SetPadding(6.f)
			.SetRounding(4.f)
			.SetVgrow(true)
			.SetBorderColor({ 255, 255, 0, 155 })
			.SetBorderSize(1.f);

	vbox1->Add(spd::ui::MakeLabel("apple2", "label4"))
		->m_style
			.SetBorderColor({ 0, 255, 255, 155 })
			.SetBorderSize(1.f);

	vbox1->Add(spd::ui::MakeButton("button1", "button2"))
		->OnClick([] { LOG_D("clicked button2\n"); })
		->m_style
			.SetPadding(4.f)
			.SetBorderColor({ 255, 255, 0, 155 })
			.SetBorderSize(1.f);

	return std::move(root);
}

std::unique_ptr<spd::ui::Container> LoginForm(spd::core::Application& app) {
	auto root = spd::ui::MakeVbox();
	root->SetTag("root");
	root->m_style
		.SetAlignment(spd::ui::Alignment::Top)
		.SetFont(assets::quicksand.Get(18.f))
		.SetBorderColor({ 0, 255, 0, 255 })
		.SetBorderSize(1.f);

	auto nav = root->Add(spd::ui::MakeNavBar());
	nav->m_style
		.SetFrameBgColor({ 255, 155, 105, 105 })
		.SetBgColor({ 255, 155, 105, 155 })
		.SetHoverColor({ 255, 155, 105, 205 })
		.SetActiveColor({ 255, 155, 105, 255 });
	nav->AddTitle("FasterPeak Hub");
	
	nav->SetNavButtonSize({ 20.f, 0.f });
	nav->AddMinButton([&app]() { app.Minimize(); });
	nav->AddCloseButton([&app]() { app.Close(); });

	auto mainVbox = root->Add(spd::ui::MakeVbox());
	mainVbox->m_style
		.SetVgrow(true)
		.SetHgrow(true)
		.SetBorderColor({ 255, 155, 105, 155 })
		.SetBorderSize(1.f);

	auto vbox = mainVbox->Add(spd::ui::MakeVbox());
	vbox->SetBaseSize({ 250.f, 0.f });
	vbox->m_style
		.SetAlignment(spd::ui::Alignment::Center)
		.SetMargin({ 0.f, 0.f, 80.f, 0.f })
		.SetSpacing(12.f)
		.SetBorderColor({ 255, 0, 255, 155 })
		.SetBorderSize(1.f);

	// Title
	vbox->Add(spd::ui::MakeLabel("FasterPeak Loader"))
		->m_style
			.SetFont(assets::quicksand.Get(24.f))
			.SetPadding({ 0.f, 0.f, 20.f, 0.f });

	// The input field
	auto licenseBox = vbox->Add(spd::ui::MakeTextBox("Enter License..."));
	licenseBox->m_style
		.SetPadding({ 8.f })
		.SetBgColor({ 25, 25, 25, 255 })
		.SetHoverColor({ 35, 35, 35, 255 })
		.SetActiveColor({ 45, 45, 45, 255 })
		.SetRounding(4.f)
		.SetHgrow(true)
		.SetBorderColor({ 100, 100, 100, 255 })
		.SetBorderSize(1.f);

	// The Login Button
	vbox->Add(spd::ui::MakeButton("Login", "btn_login"))
		->OnClick([licenseBox]() {
			std::string key = licenseBox->GetText();
			LOG_D("Attempting login with key: %s\n", key.c_str());
		})
		->m_style
		.SetPadding({ 8.f })
		.SetRounding(4.f)
		.SetHgrow(true);

	return std::move(root);
}

int main(int argc, char** argv) {
	logging::LoggerInit(nullptr, logging::LogLevel::Debug | logging::LogLevel::Error);

	spd::core::AppConfig config{
		.borderless = true,
		.noImGuiIni = true,
		.fontCallback = SetupFonts
	};
	spd::core::Application app(L"my test app", 800, 600, nullptr, config);

	auto root = std::move(LoginForm(app));

	app.SetRoot(std::move(root));
	app.Run();

	logging::LoggerShutdown();
}