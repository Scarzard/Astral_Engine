#include "Application.h"
#include "W_Configuration.h"
#include "ModuleWindow.h"

W_Configuration::W_Configuration(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

W_Configuration::~W_Configuration()
{
}

bool W_Configuration::Start()
{
	cpu_count = SDL_GetCPUCount();
	cache_size = SDL_GetCPUCacheLineSize();
	ram = SDL_GetSystemRAM();

	return true;
}

bool W_Configuration::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Configuration");
	//Draw Hierarchy stuff
	
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::InputText("Application name", TITLE, 20);
		ImGui::InputText("Organization name", ORGANIZATION, 40);

		//Framerate Histograms
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", App->fps_log[App->fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &App->fps_log[0], App->fps_log.size(), 0, title, 0.0f, 140.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %.1f", App->ms_log[App->ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &App->ms_log[0], App->ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		bool fullscreen = App->window->GetFullscreenWindow();
		bool resizable = false;
		bool borderless = App->window->GetBorderlessWindow();
		bool full_desktop = App->window->GetFullDesktopWindow();
		//Sliders
		ImGui::SliderFloat("Brightness", &brightness_slider, 0.0f, 1.0f);
		App->window->SetBrightness(brightness_slider);
		//When this is active, game window resizes automatically when Window header is toggled
		//ImGui::SliderInt("Width", &width_slider, 600, 1920);
		//App->window->SetWidth(width_slider);

		//ImGui::SliderInt("Height", &height_slider, 400, 1080);
		//App->window->SetHeigth(height_slider);

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->SetFullscreen(fullscreen);
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SetResizable(resizable);
			if (ImGui::IsItemHovered())
				/*ImGui::SetTooltip("Restart to apply");*/
				ImGui::SetTooltip("Doesn't work for now");

		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SetBorderless(borderless);
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop))
			App->window->SetFullScreenDesktop(full_desktop);

	}
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGuiIO& io = ImGui::GetIO();
		//Mouse Inputs
		if (ImGui::IsMousePosValid())
			ImGui::BulletText("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
		else	  
			ImGui::BulletText("Mouse pos: <INVALID>");

		ImGui::BulletText("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::BulletText("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::BulletText("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::BulletText("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::BulletText("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::BulletText("Mouse wheel: %.1f", io.MouseWheel);

		//Keyboard inputs
		//ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (0x%X) (%.02f secs)", i, i, io.KeysDownDuration[i]); }
		//ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
		//ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::Text("%d (0x%X)", i, i); }
		//ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
		//ImGui::Text("Chars queue:"); for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine(); ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		//ImGui::Text("SDL version : %c", SDL_GetVersion());
		ImGui::Separator();
		ImGui::Text("CPUs : %d (Cache: %d bytes)", cpu_count, cache_size);
		ImGui::Text("System RAM: %d MB", ram);
		ImGui::Separator();
	}


	ImGui::End();
	ImGui::PopStyleVar();

	return true;
}

update_status W_Configuration::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status W_Configuration::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool W_Configuration::CleanUp()
{
	return true;
}

