#include "views.h"
//#include <core/Application.h>

#include <ui/widgets/Label.h>
#include <ui/widgets/Button.h>
#include <ui/widgets/TextBox.h>
#include <ui/widgets/Spacer.h>
#include <ui/widgets/NavBar.h>

#include <ui/layouts/ViewSwitcher.h>
#include <ui/layouts/Hbox.h>
#include <ui/layouts/Vbox.h>
using namespace spd;

namespace views {

    // Forward declaring our localized view builders to keep the code organized
    static std::unique_ptr<ui::Widget> BuildLoginPanel(ui::ViewSwitcher* masterDeck);
    static std::unique_ptr<ui::Widget> BuildDashboardPanel(core::Application& app);

    static void HandleLoginButton(ui::TextBox* licenseInput, ui::Label* statusLabel, ui::ViewSwitcher* masterDeck);

    std::unique_ptr<ui::Container> CreateMasterLayout(core::Application& app) {
        // The master root layout wrapper
        auto rootScene = ui::MakeVbox();
        rootScene->SetTag("root_scene");
        rootScene->m_style
            .SetFont(assets::quicksand.Get(18.f))
            .SetAnimationSpeed(10.f)
            .SetBorderColor({ 255, 205, 155, 60 }).SetBorderThickness(1.f);

        // ==========================================================
        // 1. GLOBAL TOP NAVIGATION BAR
        // ==========================================================
        auto navBar = rootScene->Add(ui::MakeNavBar());
        navBar->m_style.SetPadding({ 6.f }).SetSpacing(10.f).SetRounding(2.f);
        navBar->AddTitle("Temp Spoofer Loader");
        navBar->SetNavButtonSize({ 20.f, 0.f });
        navBar->AddMinButton([&app]() { app.Minimize(); });
        navBar->AddCloseButton([&app]() { app.Close(); });

        // ==========================================================
        // 2. MASTER DECK SWITCHER (Handles Application Screen States)
        // ==========================================================
        auto masterDeck = rootScene->Add(ui::MakeViewSwitcher());
        masterDeck->SetTag("master_deck");
        masterDeck->m_style.SetHgrow(true).SetVgrow(true);

        // Register our separate screens smoothly into the primary state controller
        masterDeck->AddView("login_screen", BuildLoginPanel(masterDeck));
        masterDeck->AddView("main_dashboard", BuildDashboardPanel(app));

        return rootScene;
    }

    // ==============================================================
    // SCREEN VIEW LAYOUT FACTORIES
    // ==============================================================

    static std::unique_ptr<ui::Widget> BuildLoginPanel(ui::ViewSwitcher* masterDeck) {
        auto loginForm = ui::MakeVbox();
        loginForm->SetTag("login_form_container");
        loginForm->SetBaseSize({ 250.f, 0.f });
        loginForm->m_style
            .SetAlignment(ui::Alignment::Center)
            .SetMargin({ 0.f, 0.f, 80.f, 0.f }) // Keeps your exact original centering layout offset
            .SetFont(assets::quicksand.Get(18.f));

        // Form Title text
        loginForm->Add(ui::MakeLabel("FasterPeak Loader", "login_header_label"))
            ->m_style.SetFont(assets::quicksand.Get(24.f)).SetPadding({ 0.f, 0.f, 20.f, 0.f });

        // Input Field Header
        loginForm->Add(ui::MakeLabel("License :"))
            ->m_style.SetAlignment(ui::Alignment::Left).SetPadding({4.f}).SetTextColor({255, 255, 255, 200});

        // Serial Key Input Box
        auto licenseInput = loginForm->Add(ui::MakeTextBox("Enter License...", "license_textbox"));
        licenseInput->SetText("valid-key-123");
        licenseInput->m_style
            .SetPadding({ 8.f }).SetRounding(4.f).SetHgrow(true)
            //.SetBgColor({ 25, 25, 25, 255 }).SetHoverColor({ 35, 35, 35, 255 }).SetActiveColor({ 45, 45, 45, 255 })
            .SetBorderThickness(1.f);

        // Submission Button Action Node
        auto loginButton = loginForm->Add(ui::MakeButton("Login", "btn_login"));
        loginButton->m_style.SetMargin({ 0.f, 8.f }).SetPadding({ 8.f }).SetRounding(4.f).SetHgrow(true);

        // Status Feedback String (Instantly capture raw pointer on allocation!)
        auto statusLabel = loginForm->Add(ui::MakeLabel("", "status_label"));
        statusLabel->m_style.SetPadding({ 6.f }).SetFont(assets::quicksand.Get(14.f));

        loginButton->OnClick([=]() { HandleLoginButton(licenseInput, statusLabel, masterDeck); });

        return loginForm;
    }

    static std::unique_ptr<ui::Widget> BuildDashboardPanel(core::Application& app) {
        auto dashboardWorkspace = ui::MakeHbox();
        dashboardWorkspace->SetTag("dashboard_workspace_hbox");
        dashboardWorkspace->m_style.SetHgrow(true).SetVgrow(true).SetAlignment(ui::Alignment::TopLeft);

        // ==========================================
        // 1. LEFT SIDEBAR MENU
        // ==========================================
        auto sidebar = dashboardWorkspace->Add(ui::MakeVbox());
        sidebar->SetTag("dashboard_sidebar_vbox");
        sidebar->m_style
            .SetAlignment(ui::Alignment::Top).SetVgrow(true)
            .SetPadding({ 8.f })
            .SetSpacing({ 8.f })
            .SetBgColor({ 50, 50, 50, 255 });

        // ==========================================
        // 2. MAIN SUB-TAB DECK VIEW SWITCHER
        // ==========================================
        auto tabDeck = dashboardWorkspace->Add(ui::MakeViewSwitcher());
        tabDeck->SetTag("dashboard_tab_deck");
        tabDeck->m_style.SetHgrow(true).SetVgrow(true);

        // Compile and map sub-views directly into our interior layout deck
        auto homeTab = tabDeck->AddView("home_tab", ui::MakeVbox());
        homeTab->Add(ui::MakeLabel("FasterPeak Spoofer", "main_label_home"));

        auto b1Tab = tabDeck->AddView("b1_tab", ui::MakeVbox());
        b1Tab->Add(ui::MakeLabel("B1 tab", "main_label_b1"));

        auto b2Tab = tabDeck->AddView("b2_tab", ui::MakeVbox());
        b2Tab->Add(ui::MakeLabel("B2 tab", "main_label_b2"));

        // ==========================================
        // 3. SIDEBAR NAVIGATION CONTROLS
        // ==========================================

        auto makeNavButton = [sidebar](std::string&& text, const std::function<void()>& callback) {
            return sidebar->Add(ui::MakeButton(std::move(text)))
                ->OnClick(callback)
                ->SetBaseSize({ 40.f, 40.f })
                ->m_style
					.SetRounding(8.f)
					.SetFont(assets::quicksand.Get(32.f))
					.SetBorderColor({ 255, 155, 105, 255 }).SetBorderThickness(2.f);
		};
        
        makeNavButton("H", [tabDeck]() { tabDeck->SwitchTo("home_tab"); });
        makeNavButton("B1", [tabDeck]() { tabDeck->SwitchTo("b1_tab"); });
        makeNavButton("B2", [tabDeck]() { tabDeck->SwitchTo("b2_tab"); });

        return dashboardWorkspace;
    }

    void HandleLoginButton(ui::TextBox* licenseInput, ui::Label* statusLabel, ui::ViewSwitcher* masterDeck) {
		std::string key = licenseInput->GetText();

		if (key.empty()) {
			statusLabel->SetText("Error: License cannot be blank!");
			statusLabel->m_style.SetTextColor({ 255, 50, 50, 255 });
		}
		else if (key == "valid-key-123") {
			statusLabel->SetText("Valid License");
			statusLabel->m_style.SetTextColor({ 50, 255, 50, 255 });

			// Trigger state change up to the primary window shell layer
			masterDeck->SwitchTo("main_dashboard");
		}
		else {
			statusLabel->SetText("Invalid or Expired Serial Key");
			statusLabel->m_style.SetTextColor({ 255, 50, 50, 255 });
		}
    }
}