#include <string>
#include <memory>
#include "views.h"
#include "QuicksandSemiBold.hpp"

#include <ui/widgets/Label.h>
#include <ui/layouts/Vbox.h>
#include <ui/layouts/ViewSwitcher.h>
#include <utils/imgui_utils.h>

static void SetupImGui(ImGuiIO& io) {
	assets::quicksand = spd::core::Font::LoadFromMemory(
		QuicksandSemiBold, sizeof(QuicksandSemiBold),
		{ 14.f, 16.f, 18.f, 24.f, 32.f }
	);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.098f, 0.098f, 0.098f, 1.f);

	constexpr Color borderColor = { 255, 45, 21, 185 };
	colors[ImGuiCol_Border] = COLOR_TO_IMVEC4(borderColor);

	colors[ImGuiCol_Border] = utils::GetDefaultImGuiColor(ImGuiCol_Border);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.41f, 0.04f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.41f, 0.04f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.41f, 0.04f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.41f, 0.04f, 1.00f);
	constexpr Color buttonColor = { 255, 104, 10, 102 };
	colors[ImGuiCol_Button] = COLOR_TO_IMVEC4(buttonColor);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.41f, 0.04f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(1.00f, 0.41f, 0.04f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.41f, 0.04f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.41f, 0.04f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.41f, 0.04f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.41f, 0.04f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.41f, 0.04f, 0.95f);
	colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.41f, 0.04f, 0.80f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.41f, 0.04f, 0.35f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.41f, 0.04f, 1.00f);

	colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.20f, 0.02f, 0.54f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.62f, 0.20f, 0.02f, 1.00f);

	colors[ImGuiCol_SliderGrab] = ImVec4(0.92f, 0.37f, 0.03f, 1.00f);

	colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.33f, 0.04f, 1.00f);

	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.82f, 0.31f, 0.00f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.82f, 0.31f, 0.00f, 1.00f);

	colors[ImGuiCol_Tab] = ImVec4(0.65f, 0.24f, 0.06f, 0.86f);

	colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.31f, 0.14f, 1.00f);

	colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.08f, 0.00f, 0.97f);

	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.47f, 0.24f, 0.08f, 1.00f);
}

int main(int argc, char** argv) {
	logging::LoggerInit(nullptr,
		logging::LogLevel::Debug
		| logging::LogLevel::Info
		| logging::LogLevel::Warn
		| logging::LogLevel::Error
		//| logging::LogLevel::Trace
	);

	spd::core::AppConfig config{
		.borderless = true,
		.noImGuiIni = true,
		.imguiCallback = SetupImGui
	};
	spd::core::Application app(L"my test app", 800, 600, nullptr, config);

	auto root = std::move(views::CreateMasterLayout(app));

	app.SetRoot(std::move(root));
	app.Run(std::chrono::milliseconds(500));

	logging::LoggerShutdown();
}