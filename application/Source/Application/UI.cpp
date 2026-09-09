#define IMGUI_DEFINE_MATH_OPERATORS

#include "UI.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "../Game/Game.h"
#include "../Images.h"
#include "../Colors.h"
#include "../mh.h"

#include <filesystem>
#include <functional>

namespace UI
{
	int panel = 1;

	ImGuiLayer::ImGuiLayer() : m_renderer(nullptr) {}
	ImGuiLayer::~ImGuiLayer() { Cleanup(); }

	bool ImGuiLayer::Initialize(HWND hwnd, Renderer::DX11Renderer* renderer) {
		m_renderer = renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.IniFilename = nullptr;

		wchar_t executablePath[MAX_PATH];
		const DWORD executableLength = GetModuleFileNameW(nullptr, executablePath, MAX_PATH);

		if (executableLength == 0 or executableLength >= MAX_PATH)
		{
			return false;
		}

		const auto assets = std::filesystem::path(executablePath).parent_path() / "assets";

		if (not Images::LoadTextureFromFile(renderer->GetDevice(), (assets / "tiles.png").c_str(), &Images::Images[0])
			or not Images::LoadTextureFromFile(renderer->GetDevice(), (assets / "Mine.png").c_str(), &Images::Images[1]))
		{
			return false;
		}

		wchar_t windowsDirectory[MAX_PATH];
		const UINT length = GetWindowsDirectoryW(windowsDirectory, MAX_PATH);

		if (length == 0 or length >= MAX_PATH)
		{
			return false;
		}

		const auto fontPath = (std::filesystem::path(windowsDirectory) / "Fonts" / "segoeuib.ttf").u8string();

		for (float fontSize : { 30.0f, 60.0f, 48.0f })
		{
			if (not io.Fonts->AddFontFromFileTTF(reinterpret_cast<const char*>(fontPath.c_str()), fontSize))
			{
				return false;
			}
		}

		io.Fonts->Build();
		ImGuiStyle& style = ImGui::GetStyle();

		style.FrameBorderSize = 0;
		style.ChildBorderSize = 0;
		style.Colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.0f);

		if (!ImGui_ImplWin32_Init(hwnd)) return false;
		if (!ImGui_ImplDX11_Init(m_renderer->GetDevice(), m_renderer->GetContext()))
			return false;

		return true;
	}

	void ImGuiLayer::BeginFrame() {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::Render() {
		RenderMainWindow();
	}

	void ImGuiLayer::EndFrame() {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::Cleanup() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::RenderMainWindow() {
		game::update();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		const ImGuiViewport* vp = ImGui::GetMainViewport();
		const ImVec2 s = vp->Size;

		ImGui::SetNextWindowPos(vp->WorkPos);
		ImGui::SetNextWindowSize(vp->WorkSize);

		ImGui::Begin("##.", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);

		const ImGuiWindow* w = ImGui::GetCurrentWindow();
		ImDrawList* dl = ImGui::GetWindowDrawList();

		ImVec2 ct = s / 2;

		dl->AddRectFilled(ImVec2(), s, Colors::Canvas, 0);

		switch (panel) {
		case 1:
		{
			static int settings_offset = 200;

			dl->AddRectFilled(ImVec2(0, 0), ImVec2(s.x, 60), IM_COL32_WHITE, 0);
			dl->AddLine(ImVec2(0, 61), ImVec2(s.x, 61), Colors::Border, 2);
			//dl->AddText(io.Fonts->Fonts[2], 48, ImVec2(60, 7), IM_COL32(0, 0, 0, 255), std::to_string(game::br.get_mine_count() - game::flags).c_str());
			//dl->AddImage((ImTextureID)(uintptr_t)Images::Images[1].Texture, ImVec2(12, 12), ImVec2(48, 48), ImVec2(), ImVec2(1,1), IM_COL32_BLACK);

			dl->AddRectFilled(ImVec2(0, s.y), ImVec2(s.x, s.y - settings_offset), IM_COL32_WHITE, 0);

			static bool dragging = false;

			std::uint32_t border_color = Colors::Border;

			if (ImGui::IsMouseHoveringRect(ImVec2(0, s.y - settings_offset - 5), ImVec2(s.x, s.y - settings_offset + 5)))
			{
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					dragging = true;
				}

				border_color = Colors::ActiveBorder;
			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && dragging)
			{
				int g = static_cast<int>(ImGui::GetMousePos().y);

				//if (g < s.y - 100 and g > 100)
				settings_offset = static_cast<int>(s.y - g);

				if (settings_offset > s.y - 100)
					settings_offset = static_cast<int>(s.y - 100);
				if (settings_offset < 1)
					settings_offset = 1;
			}
			else
			{
				dragging = false;
			}

			dl->AddLine(ImVec2(0, s.y - settings_offset - 1), ImVec2(s.x, s.y - settings_offset - 1), border_color, 2);

			ImGui::SetCursorPos(ImVec2(0, s.y - settings_offset - 1));

			ImVec2 c1 = { 30, s.y - settings_offset + 20 };
			ImVec2 c2 = { 380, s.y - settings_offset + 20 };
			ImVec2 c3 = { s.x - 230, s.y - settings_offset + 20 };

			dl->AddText(io.Fonts->Fonts[2], 48, c1, IM_COL32_BLACK, "Game");
			dl->AddText(io.Fonts->Fonts[2], 48, c2, IM_COL32_BLACK, "Solver");

			ImGui::PushFont(io.Fonts->Fonts[0]);

			mh::my_int_slider("Size X", (int*)&game::size.y, 3, 50, 0, c1 + ImVec2(0, 60), c1 + ImVec2(200, 110), dl, "%d");
			mh::my_int_slider("Size Y", (int*)&game::size.x, 3, 50, 1, c1 + ImVec2(0, 130), c1 + ImVec2(200, 180), dl, "%d");
			mh::my_int_slider("Mines", (int*)&game::mines, 1, game::size.x * game::size.y, 2, c1 + ImVec2(0, 200), c1 + ImVec2(200, 250), dl, "%d");
			mh::my_checkbox("Show mines", c1 + ImVec2(0, 270), &game::hint, dl);

			//mh::my_checkbox("Visualization", c2 + ImVec2(0, 60), &game::visualization, dl);
			//mh::my_checkbox("Auto Solve", c2 + ImVec2(0, 100), &game::solving, dl);
			//mh::my_int_slider("Speed", (int*)&game::speed, 1, 10, 3, c2 + ImVec2(0, 130), c2 + ImVec2(200, 180), dl, " % d");

			mh::my_checkbox("Auto Solve", c2 + ImVec2(0, 60), &game::solving, dl);
			mh::my_checkbox("Q-Solve", c2 + ImVec2(0, 220), &game::sl.q_, dl);
			mh::my_checkbox("CS-Solve", c2 + ImVec2(0, 260), &game::sl.cs_, dl);
			mh::my_int_slider("Speed", (int*)&game::speed, 1, 10, 3, c2 + ImVec2(0, 90), c2 + ImVec2(200, 140), dl, " % d");

			if (mh::my_button("Step", { c2 + ImVec2(0, 160), c2 + ImVec2(200, 200) }, dl, not game::solving) and not game::solving)
			{
				game::should_step = true;
			}

			if (game::solving)
			{
				dl->AddRectFilled(c2 + ImVec2(0, 160), c2 + ImVec2(200, 200), Colors::DisabledOverlay, 4);
			}

			if (mh::my_button("Start", { c3, c3 + ImVec2(200, 40) }, dl, 1))
			{
				game::start();
			}

			ImGui::PopFont();

			Milkshake::Size size = game::br.get_size();

			ImRect canvas = { ImVec2(0, 61), ImVec2(s.x, s.y - settings_offset) };

			ImVec2 center = ImVec2((canvas.Min.x + canvas.Max.x) / 2, (canvas.Min.y + canvas.Max.y) / 2);

			static float cell_size = 40;



			ImVec2 corner = ImVec2(0, 61);



			ImVec2 nr_field_size = ImVec2(size.x * cell_size, size.y * cell_size);
			ImVec2 nr_content_size = ImVec2(s.x - corner.x, s.y - settings_offset - corner.y);
			ImVec2 start = ImVec2(nr_content_size.x / 2 - nr_field_size.x / 2, nr_content_size.y / 2 - nr_field_size.y / 2);

			if (nr_field_size.x > nr_content_size.x || nr_field_size.y > nr_content_size.y)
				start = { 0, 0 };

			ImGui::SetCursorPos(corner);

			if (ImGui::BeginChild("##Container", nr_content_size, ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::SetCursorPos(start);

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

				bool field_visible = false;

				if (game::started)
					field_visible = ImGui::BeginChild("##Field", nr_field_size, ImGuiChildFlags_Borders | ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

				if (field_visible)
				{
					for (uint32_t y = 0; y < size.y; y++)
					{
						for (uint32_t x = 0; x < size.x; x++)
						{
							Milkshake::Cell* T = game::br.get_cell({ x, y });

							Milkshake::Value vl = T->get_value();
							Milkshake::State st = T->get_state();
							uint8_t idx = 0;

							if (static_cast<int>(st) == 0)
								idx = static_cast<int>(vl);
							else if (game::hint and T->is_mine() and st == Milkshake::State::HIDDEN)
								idx = 12;
							else
								idx = 8 + static_cast<int>(st);

							ImVec2 uv0 = ImVec2(idx * 100 / (float)Images::Images[0].width, 0);
							ImVec2 uv1 = ImVec2((idx + 1) * 100 / (float)Images::Images[0].width, 1);

							ImVec2 gk = ImGui::GetCursorPos();
							ImGui::BeginChild(std::format("##img {} {}", std::to_string(y), std::to_string(x)).c_str(), ImVec2(cell_size, cell_size));
							//ImGui::TextUnformatted(std::format("{}\t{}\n", (int)st, (int)vl).c_str());
							ImGui::EndChild();

							std::uint32_t cl = IM_COL32_WHITE;

							if (ImGui::IsItemHovered())
							{
								//ImGui::SetItemTooltip ( "Revealed: %d", game::board.get_cell ( { x, y } )->get_state() == Milkshake::State::REVEALED);

								cl = Colors::CellHover;
							}

							if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) or T->is_safe()) and game::playing)
							{
								if (st != Milkshake::State::FLAGGED)
									game::reveal(T, true, true);
							}

							if (T->is_safe())
							{
								T->set_safe(false);
								printf(".");
								game::reveal(T, true, true);
							}

							if (ImGui::IsItemClicked(ImGuiMouseButton_Right) and game::playing and st != Milkshake::State::REVEALED)
							{
								if (st == Milkshake::State::FLAGGED)
								{
									T->set_state(Milkshake::State::HIDDEN);
									game::flags--;
								}
								else
								{
									T->set_state(Milkshake::State::FLAGGED);
									game::flags++;
								}
							}



							if (T->vs.render)
								cl = T->vs.fcl;

							ImGui::SetCursorPos(gk);
							ImGui::Image((ImTextureID)(intptr_t)Images::Images[0].Texture, ImVec2(cell_size, cell_size), uv0, uv1, ImGui::ColorConvertU32ToFloat4(cl));



							if (x < size.x - 1) ImGui::SameLine();
						}
					}

				}
				else
				{
					/*ImGui::PushFont(io.Fonts->Fonts[2]);
					ImVec2 ts = ImGui::CalcTextSize("Press Start to begin.");
					ImGui::SetCursorPos(ImVec2(nr_content_size.x / 2 - ts.x / 2, nr_content_size.y / 2 - ts.y / 2));
					ImGui::TextUnformatted("Press Start to begin.");
					ImGui::PopFont();*/
				}

				if (game::started)
					ImGui::EndChild();

				ImGui::PopStyleVar(3);
			}

			ImGui::EndChild();

			break;
		}

		default:
			break;
		}

		ImGui::End();
	}
}
