#include "Application.h"
#include "W_Configuration.h"
#include "ModuleWindow.h"
#include "ModuleEngineUI.h"

#include "Assimp/include/version.h"

#include "DevIL/include/il.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

//GPU detection 
#include "gpudetect/DeviceId.h"

W_Configuration::W_Configuration() : Window()
{
	App->LogInConsole("Created Configuration Window");
}

W_Configuration::~W_Configuration()
{
}

bool W_Configuration::Start()
{

	SDL_GetVersion(&linked);


	info.cpu_count = SDL_GetCPUCount();
	info.cache_size = SDL_GetCPUCacheLineSize();
	info.ram = SDL_GetSystemRAM();

	uint vendor, device_id;
	std::wstring brand;
	unsigned __int64 video_mem_budget;
	unsigned __int64 video_mem_usage;
	unsigned __int64 video_mem_available;
	unsigned __int64 video_mem_reserved;
	if (getGraphicsDeviceInfo(&vendor, &device_id, &brand, &video_mem_budget, &video_mem_usage, &video_mem_available, &video_mem_reserved))
	{
		//info.gpu_vendor = vendor;
		//info.gpu_device = device_id;
		sprintf_s(info.gpu_brand, 250, "%S", brand.c_str());
		info.vram_budget = float(video_mem_budget) / 1073741824.0f;
		info.vram_usage = float(video_mem_usage) / (1024.f * 1024.f * 1024.f);
		info.vram_available = float(video_mem_available) / (1024.f * 1024.f * 1024.f);
		info.vram_reserved = float(video_mem_reserved) / (1024.f * 1024.f * 1024.f);
	}

	return true;
}

bool W_Configuration::Draw()
{
	if (App->gui->config)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 0));

		ImGui::Begin("Configuration");
		//Draw Hierarchy stuff

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::Text("Application name: %s", TITLE);
			ImGui::Text("Organization name: %s", ORGANIZATION);


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
	
			ImGui::DragInt("Width", &width_slider, 1, 600, 1920);
			App->window->SetWindowSize(width_slider, height_slider);

			ImGui::DragInt("Height", &height_slider, 1, 400, 1080);
			App->window->SetWindowSize(width_slider, height_slider);

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
		if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_Leaf))
		{
			//ComponentCamera* camera = selected_GO->GetComponentCamera();
			ImGui::Text("Horizontal FOV:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.0f, 1.0f), "%f", App->camera->main_camera->frustum.horizontalFov);


			ImGui::Text("Vertical FOV:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.0f, 1.0f), "%f", App->camera->main_camera->frustum.verticalFov);

			float fov = App->camera->main_camera->frustum.verticalFov * RADTODEG;
			ImGui::Text("Set FOV:       ");
			ImGui::SameLine(); ImGui::PushItemWidth(100);
			if (ImGui::DragFloat("1", &fov, 1.0f))
				App->camera->main_camera->SetFOV(fov);

			float near_plane = App->camera->main_camera->frustum.nearPlaneDistance;
			ImGui::Text("Set Near Plane:");
			ImGui::SameLine(); ImGui::PushItemWidth(100);
			if (ImGui::DragFloat("2", &near_plane, 1.0F, -INFINITY, INFINITY))
			{
				App->camera->main_camera->SetNearPlane(near_plane);
			}

			float far_plane = App->camera->main_camera->frustum.farPlaneDistance;
			ImGui::Text("Set Far Plane: ");
			ImGui::SameLine(); ImGui::PushItemWidth(100);
			if (ImGui::DragFloat("3", &far_plane, 1.0F, -INFINITY, INFINITY))
			{
				App->camera->main_camera->SetFarPlane(far_plane);
			}
		}
		if (ImGui::CollapsingHeader("Renderer"))
		{
			if (ImGui::Checkbox("Depth test", &depth)) 
			{
				if (depth)
					glEnable(GL_DEPTH_TEST);
				else
					glDisable(GL_DEPTH_TEST);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Lighting", &light))
			{
				if (light)
					glEnable(GL_LIGHTING);
				else 
					glDisable(GL_LIGHTING);
			}
		
			if (ImGui::Checkbox("Wireframe", &wireframe))
			{
				if (wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			
				else 
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Cull face", &cullface))
			{
				if (cullface)
					glEnable(GL_CULL_FACE);
				else
					glDisable(GL_CULL_FACE);
			}

			if (ImGui::Checkbox("Vertex", &vertex))
			{
				if (vertex)
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			ImGui::SameLine();
			ImGui::Checkbox("Draw Bounding Boxes", &draw_aabb); 

			ImGui::Checkbox("Draw Plane", &draw_plane);

			ImGui::Checkbox("Draw QuadTree", &draw_quadtree);
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
			ImGui::BulletText("CPUs : %d (Cache: %d bytes)", info.cpu_count, info.cache_size);
			ImGui::BulletText("System RAM: %d MB", info.ram);
			ImGui::Separator();
			//ImGui::BulletText("GPU: vendor %d device %d", info.gpu_vendor, info.gpu_device);
			ImGui::BulletText("Brand: %s", info.gpu_brand);
			ImGui::BulletText("GPU RAM Budget: %.1f MB", info.vram_budget);
			ImGui::BulletText("GPU RAM Usage: %.1f MB", info.vram_usage);
			ImGui::BulletText("GPU RAM Available: %.1f MB", info.vram_available);
			ImGui::BulletText("GPU RAM Reserved: %.1f MB", info.vram_reserved);
		}

		if (ImGui::CollapsingHeader("Software"))
		{
			ImGui::BulletText("SDL: %d.%d.%d", linked.major, linked.minor, linked.patch);
			ImGui::BulletText("OpenGL %s", (const char*)glGetString(GL_VERSION));
			ImGui::BulletText("ImGui: %s", ImGui::GetVersion());
			ImGui::BulletText("Glew: %s", (const char*)glewGetString(GLEW_VERSION));
			ImGui::BulletText("Assimp: %d.%d.%d", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
			ImGui::BulletText("DevIL: %d", IL_VERSION);

		}

		ImGui::End();
		ImGui::PopStyleVar();
	}
	

	return true;
}

